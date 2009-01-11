/*
 * Copyright (c) 2005-2009 Jeremy Erickson
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <qapplication.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qiodevice.h>
#include <qtabwidget.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include "qt_types.h"
#include "ui/preferencesact.h"
#include "core/settings.h"
#include "core/biblepluginmeta.h"
#include "core/bibleplugin.h"
#include "core/bmemversion.h"
#include "ui/plugininfoact.h"
#include <vector>
#include <iostream>
using namespace std;
using namespace bmemcore;

namespace bmemui
{

PreferencesAct::PreferencesAct(QWidget *parent, const char *name)
:PreferencesUI(parent, name), mDirectory(Settings::getPluginDir())
{
    populateList();
    QString file = Settings::getPluginFile();
    if (!file.isEmpty() && QFile::exists(file))
    {
        int currIndex = 0;
        for (std::vector<BiblePluginMeta>::iterator it = mFiles.begin();
                it != mFiles.end(); ++it)
        {
            if (it->getMetaFileName() == file)
            {
                mPluginListBox->setSelected(currIndex, true);
                break;
            }
            currIndex++;
        }
    }
    mUseLastCheckBox->setChecked(Settings::getOpenLastFile());
    mDefaultTranslationComboBox->setCurrentText(
            Settings::getDefaultTranslation());
    mPuncErrorCheckBox->setChecked(Settings::getCountPuncErrors());
    mCapErrorCheckBox->setChecked(Settings::getCountCapErrors());
    mSpellingErrorCheckBox->setChecked(Settings::getCountSpellingErrors());
}

BiblePluginMeta PreferencesAct::getMetaStatic(bool &success)
{
    success = true;
    QString tryFor = "";
    if (!Settings::getPluginFile().isEmpty())
    {
        bool success;
        BiblePluginMeta toReturn(Settings::getPluginFile());
        BiblePlugin tryPlugin(toReturn, &success);
        if (success)
        {
            return toReturn;
        }
        else
        {
            tryFor = toReturn.getBaseName();
        }
    }
    QString directory = Settings::getPluginDir();
    std::vector<BiblePluginMeta> files;
    files = getPluginList(directory);
    bool worked = true;
    if (files.empty())
    {
        //Where the executable resides, important to guessing a path.
        QString appDirPath = qApp->applicationDirPath();
        QDir attempt(appDirPath);
#ifdef Q_OS_WIN32
        //Guess probable on systems like Windows.  e.g.
        //app at C:\Program Files\BibleMemorizer\biblememorizer.exe,
        //plugins in C:\Program Files\BibleMemorizer\plugins
        attempt.cd("./plugins");
#else
#ifdef Q_OS_MAC
        //Guess correct for OSX bundle, e.g. app at
        //BibleMemorizer.app/Contents/MacOS/BibleMemorizer, plugins at
        //BibleMemorizer.app/Contents/Plugins
        attempt.cd("../Plugins");
#else
        //Guess probable on Unix-like systems.  e.g.
        //app at /usr/bin/biblememorizer, plugins in
        // /usr/lib/biblememorizer/plugins
        attempt.cd("../lib/biblememorizer/plugins");
#endif
#endif
        worked = attempt.exists();
        if (worked)
        {
            directory = attempt.absPath();
            files = getPluginList(directory);
            worked = !files.empty();
        }
        //Again, value of "worked" may have changed.
        if (worked)
        {
            //Plugins found, set settings for the future.
            Settings::setPluginDir(directory);
        }
        else
        {
            //All other attempts have failed, must prompt user.
            while (!worked)
            {
                int response = QMessageBox::warning(0,
                        tr("Plugin Directory Not Found"),
                        tr("The plugins for BibleMemorizer could not be "
                        "found.\n"
                        "Click OK to select a directory manually, or "
                        "Cancel to abort.\n\n"
                        "If you are seeing this message on the startup of "
                        "BibleMemorizer,\n"
                        "Cancelling will cause BibleMemorizer to exit.  If "
                        "you do not\n"
                        "know what to do, contact the person who gave you "
                        "BibleMemorizer\n"
                        "or post a message in the help forum at "
                        "http://biblememorizer.sf.net"),
                        QMessageBox::Ok | QMessageBox::Default,
                        QMessageBox::Cancel | QMessageBox::Escape);
                if (response == QMessageBox::Ok)
                {
                    directory = 
                            BQFileDialog::getExistingDirectory(appDirPath,
                            0, "Get Plugin Directory",
                            tr("Choose Plugin Directory"));
                    files = getPluginList(directory);
                    worked = !files.empty();
                }
                else
                {
                    success = false;
                    break;
                }
            }
            //Not the case if "break" occured.
            if (worked)
            {
                Settings::setPluginDir(directory);
            }
        }
    }
    if (worked)
    {
        //If we have a known plugin name, but was not found in the proper
        //location, try to find it now.
        if (!tryFor.isEmpty())
        {
            for (std::vector<BiblePluginMeta>::const_iterator it = 
                    files.begin(); it != files.end(); it++)
            {
                if (it->getBaseName() == tryFor)
                {
                    Settings::setPluginFile(it->getMetaFileName());
                    return *it;
                }
            }
        }
        PreferencesAct selector;
        selector.selectPluginTab();
        selector.hideCancel();
        int result = selector.exec();
        if (result == QDialog::Accepted)
        {
            return selector.getMeta();
        }
        else
        {
            success = false;
            BiblePluginMeta toReturn;
            return toReturn;
        }
    }
    else
    {
        success = false;
        BiblePluginMeta toReturn;
        return toReturn;
    }
}

BiblePluginMeta PreferencesAct::getMeta()
{
    return mFiles[mPluginListBox->index(mPluginListBox->selectedItem())];
}

void PreferencesAct::selectPluginTab(){
    mTabWidget->setCurrentPage(mTabWidget->indexOf(mPluginTab));
}

void PreferencesAct::hideCancel()
{
    mCancelButton->hide();
}

void PreferencesAct::mOKButton_clicked()
{
    Settings::setPluginDir(mDirectory);
    Settings::setPluginFile(mFiles[mPluginListBox->index(
            mPluginListBox->selectedItem())].getMetaFileName());
    Settings::setOpenLastFile(mUseLastCheckBox->isChecked());
    Settings::setDefaultTranslation(mDefaultTranslationComboBox->currentText());
    Settings::setCountPuncErrors(mPuncErrorCheckBox->isChecked());
    Settings::setCountCapErrors(mCapErrorCheckBox->isChecked());
    Settings::setCountSpellingErrors(mSpellingErrorCheckBox->isChecked());
    accept();
}

void PreferencesAct::mDirButton_clicked()
{
    QString newDirectory = BQFileDialog::getExistingDirectory(mDirectory,
        this, "Get Plugin Directory", tr("Choose Plugin Directory"));
    if (populateList(newDirectory))
    {
        mDirectory = newDirectory;
    }
    else
    {
        if (!newDirectory.isEmpty())
            int response = QMessageBox::warning(this,
                    tr("Bad Plugin Directory"),
                    tr("No valid plugins found in that directory.\n\n"
                    "Using previous directory."));
    }
}

bool PreferencesAct::populateList()
{
    return populateList(mDirectory);
}

bool PreferencesAct::populateList(const QString& path)
{
    //Assuming failure until proven successful
    bool toReturn = false;
    if (!path.isEmpty())
    {
        std::vector<BiblePluginMeta> files = getPluginList(path);
        if (!files.empty())
        {
            mFiles = files;
            mPluginListBox->clear();
            std::vector<BiblePluginMeta>::iterator it3 = mFiles.begin();
            while (it3 != mFiles.end())
            {
                mPluginListBox->insertItem((*it3).getName());
                it3++;
            }
            mPluginListBox->setSelected(0, true);
            toReturn = true;
        }
    }
    return toReturn;
}

std::vector<bmemcore::BiblePluginMeta> PreferencesAct::getPluginList(const
        QString& path)
{
    std::vector<BiblePluginMeta> files;
    if (!path.isEmpty())
    {
        //Includes the top directory and immediate subdirectories.
        QDir searchTopSubs(path, "*", QDir::Name | QDir::IgnoreCase,
            QDir::Dirs | QDir::Readable);
        //Store all searchable directories.
        std::vector<QDir> allSearches;
        //Add all the subdirectories.
        for (int i = 0; i < searchTopSubs.count(); i++)
        {
            allSearches.push_back(QDir(searchTopSubs.absFilePath(
                    searchTopSubs[i]), "*.plugin",
                    QDir::Name | QDir::IgnoreCase,
                    QDir::Files | QDir::Readable));
        }
        //Create a list of found files.
        std::vector<QDir>::iterator it1 = allSearches.begin();
        while (it1 != allSearches.end())
        {
            QStringList newEntries = (*it1).entryList();
            //Retrieve absolute versions of the paths, because
            //The files could be located in different directories.
            QStringList::iterator it2 = newEntries.begin();
            while (it2 != newEntries.end())
            {
                BiblePluginMeta newMeta((*it1).absFilePath(*it2));
                if (newMeta.getInterfaceVersion() ==
                        BMEM_SUPPORTED_PLUGIN_INTERFACE)
                {
                    files.push_back(newMeta);
                }
                it2++;
            }
            it1++;
        }
    }
    return files;
}

PreferencesAct::~PreferencesAct()
{
}

void PreferencesAct::mInfoButton_clicked()
{
    PluginInfoAct infoFrame(getMeta());
    infoFrame.exec();
}

void PreferencesAct::mPluginListBox_selected(int index)
{
    BiblePlugin ourPlugin(mFiles[index]);
    QString oldTranslation = mDefaultTranslationComboBox->currentText();
    mDefaultTranslationComboBox->clear();
    if (ourPlugin.translationsAvailable()){
        mDefaultTranslationComboBox->insertStringList(
                ourPlugin.getTranslations());
    }
    mDefaultTranslationComboBox->setCurrentText(oldTranslation);
}

}

