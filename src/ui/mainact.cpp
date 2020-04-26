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

#include "ui/mainact.h"
#include "ui/verseact.h"
#include "ui/aboutact.h"
#include "core/verse.h"
#include "core/filterallowall.h"
#include "core/filtercategory.h"
#include "core/filterquery.h"
#include "core/versecollection.h"
#include "ui/preferencesact.h"
#include "ui/quizcreateact.h"
#include "core/bibleplugin.h"
#include "core/biblepluginmeta.h"
#include "core/settings.h"
#include <qsplitter.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qapplication.h>
#include <qmap.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qmessagebox.h>
#include <qinputdialog.h>
#include <qdom.h>
#include <qtextstream.h>
#include <qclipboard.h>
#include <qlayout.h>
#include <qimage.h>
#include <qsplitter.h>
#include <qfiledialog.h>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace bmemcore;
namespace bmemui
{

//Icon data for non-Mac
static const unsigned char icon_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x01,
    0xf1, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xa5, 0x93, 0x4d, 0x6b, 0x13,
    0x51, 0x14, 0x86, 0x9f, 0x3b, 0x19, 0x93, 0xce, 0x94, 0x90, 0x69, 0x48,
    0x89, 0xb1, 0x60, 0xa9, 0x85, 0xfa, 0x41, 0xab, 0x59, 0xf9, 0x01, 0x6a,
    0x37, 0x22, 0xb8, 0x51, 0xdc, 0xeb, 0xc2, 0x85, 0x22, 0x74, 0xeb, 0x2f,
    0xa8, 0x3f, 0x40, 0x5d, 0xb9, 0x88, 0x5f, 0x5d, 0x89, 0x5d, 0x4b, 0x2b,
    0x82, 0x8b, 0xb6, 0x08, 0x2a, 0x6a, 0x41, 0xb0, 0x56, 0x02, 0x8e, 0x31,
    0x51, 0x4c, 0xdb, 0x31, 0xa9, 0x19, 0xa7, 0x93, 0x26, 0x99, 0xe3, 0x22,
    0x95, 0x06, 0x6b, 0xd3, 0x82, 0x2f, 0x9c, 0xd5, 0x3d, 0xf7, 0x39, 0x2f,
    0xef, 0xbd, 0x47, 0x89, 0x08, 0xff, 0x23, 0x1d, 0x20, 0x14, 0xbe, 0x26,
    0x89, 0xdd, 0xc7, 0x88, 0xef, 0x82, 0x78, 0x0f, 0xc4, 0x76, 0x82, 0x69,
    0x81, 0xd2, 0x60, 0xa5, 0x0c, 0xd9, 0x17, 0x33, 0x7c, 0x7c, 0x7e, 0x53,
    0x6d, 0x0a, 0xd8, 0x11, 0x3e, 0x81, 0x95, 0x3c, 0x47, 0x6a, 0x2f, 0xf4,
    0xec, 0x6f, 0x56, 0xb2, 0x1f, 0xb4, 0x10, 0x2c, 0x7c, 0x02, 0xa7, 0x50,
    0xdc, 0xd4, 0x81, 0x06, 0xa0, 0x54, 0x73, 0x9a, 0xa6, 0x41, 0x57, 0x79,
    0x8a, 0x7d, 0x73, 0x97, 0xe8, 0x88, 0x82, 0x11, 0x83, 0x50, 0xf8, 0x4f,
    0x57, 0x1b, 0x80, 0x04, 0x10, 0x34, 0x20, 0xe1, 0x4e, 0x91, 0xb4, 0xc7,
    0x38, 0xd4, 0xff, 0x84, 0x60, 0x32, 0x83, 0x3b, 0x9f, 0xa7, 0xe6, 0x83,
    0xd4, 0xb7, 0x00, 0x34, 0x1a, 0x50, 0xf3, 0x21, 0x1d, 0x7d, 0xc0, 0xe9,
    0xb3, 0x93, 0xec, 0x39, 0xec, 0x70, 0x34, 0x3e, 0x4a, 0xed, 0xe5, 0x33,
    0x5c, 0xa7, 0x79, 0xd6, 0x36, 0xc4, 0x7a, 0x15, 0xfc, 0x5f, 0x30, 0x5e,
    0xb8, 0x4f, 0xe1, 0x71, 0x86, 0x8b, 0x8c, 0x72, 0xef, 0x67, 0x1e, 0xba,
    0xc0, 0xfb, 0x0e, 0x55, 0x6f, 0x0b, 0x00, 0xc0, 0xca, 0x32, 0xb8, 0x3f,
    0xc0, 0x0f, 0x83, 0x34, 0x60, 0xe9, 0x4b, 0x33, 0x9b, 0x95, 0x0a, 0xf8,
    0x95, 0x6d, 0x00, 0x5c, 0x07, 0x3a, 0xa2, 0xe0, 0x99, 0xcd, 0x3c, 0x96,
    0x72, 0x10, 0xb3, 0x1f, 0x72, 0x7c, 0xee, 0x36, 0xe7, 0xcb, 0xef, 0xb9,
    0xaa, 0x46, 0x30, 0x0d, 0x43, 0x4e, 0x1a, 0x06, 0x8b, 0x9e, 0xc7, 0x2b,
    0xdf, 0x57, 0x19, 0x91, 0x75, 0x40, 0xa3, 0x0e, 0xcb, 0x45, 0xf8, 0x1c,
    0x19, 0xe0, 0xe9, 0xc4, 0x29, 0x16, 0xbf, 0xae, 0x72, 0xf9, 0xf5, 0x75,
    0x2e, 0x78, 0x1f, 0xc0, 0x30, 0x38, 0x32, 0x70, 0x40, 0x12, 0xbd, 0xbd,
    0xa4, 0x2c, 0x0b, 0x1c, 0x07, 0x73, 0x76, 0x56, 0x00, 0xa5, 0xb7, 0xda,
    0xf1, 0x2b, 0xf0, 0xc6, 0x1e, 0x66, 0xbe, 0x34, 0x8c, 0x1e, 0xf1, 0xa0,
    0xba, 0xe6, 0x3d, 0x95, 0x62, 0x68, 0x70, 0x10, 0xd2, 0x69, 0xb0, 0x2c,
    0xc8, 0x66, 0xa9, 0xe5, 0x72, 0xeb, 0xaf, 0xd0, 0xaa, 0x55, 0x0f, 0x4a,
    0xdf, 0xc0, 0xc9, 0x9b, 0xdc, 0x08, 0xae, 0xe0, 0x02, 0x84, 0x42, 0xa0,
    0xeb, 0x10, 0x0e, 0x43, 0x67, 0x27, 0xb6, 0xe7, 0x31, 0x51, 0x2a, 0xa9,
    0x96, 0x0c, 0x66, 0x36, 0x84, 0x13, 0xd4, 0xe1, 0x2d, 0x07, 0x19, 0x21,
    0xcd, 0xd8, 0x82, 0x0d, 0xb9, 0x1c, 0x68, 0x1a, 0x12, 0x89, 0x70, 0x67,
    0x7a, 0x9a, 0xf1, 0xe2, 0xda, 0xef, 0x14, 0x91, 0xb6, 0x35, 0x04, 0xdc,
    0x05, 0x11, 0xd3, 0x14, 0xe9, 0xeb, 0x93, 0x5b, 0xdd, 0xdd, 0xd2, 0x7a,
    0x4f, 0xdf, 0x30, 0xfa, 0x2f, 0xbd, 0x13, 0xe1, 0x8c, 0x52, 0x2a, 0xf0,
    0x3c, 0xa9, 0xd8, 0x36, 0x8f, 0x40, 0xb5, 0x6e, 0xb0, 0xda, 0xee, 0x3a,
    0x2b, 0xa5, 0xf8, 0x57, 0xef, 0x6f, 0xdc, 0x04, 0xdf, 0x9d, 0xd3, 0xa2,
    0xaf, 0x60, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42,
    0x60, 0x82
};

MainAct::MainAct(QWidget *parent, const char *name)
:QMainWindow(parent), currWidget(NULL), mVerses(new VerseCollection()),
        mVerseSelection(new VerseSelectAct(mNavFrame, false, mVerses))
{
    setupUi(this);

    setWindowTitle(name);

    mCategoryNoticeLabel->setWordWrap(true);
#ifndef Q_OS_MAC
    QPixmap img;
    img.loadFromData( icon_data, sizeof( icon_data ), "PNG" );
    setWindowIcon(img);
#endif
    initVerseCollection();
    QVBoxLayout *newLayout = new QVBoxLayout(mNavFrame);
    newLayout->setMargin(11);
    newLayout->setSpacing(6);
    newLayout->addWidget((QWidget*)mAddButton);
    newLayout->addWidget((QWidget*)mVerseSelection);
    newLayout->addWidget((QWidget*)openForLeadLabel);
    newLayout->addWidget((QWidget*)mOpenForComboBox);
    newLayout->addWidget((QWidget*)mOpenButton);
    newLayout->addWidget((QWidget*)mRemoveButton);
    bool worked = true;
    BiblePluginMeta meta = PreferencesAct::getMetaStatic(worked);
    mPlugin = new BiblePlugin(meta);
    mCategoryListBox->clear();
    mFileFilters = tr("Bible Verse Collection files (*.bvc)\n"
                      "All Files (*)");
    setCurrentFileName("");
    mOpenForComboBox->insertItem(VerseAct::MODE_VIEW, tr("View"));
    mOpenForComboBox->insertItem(VerseAct::MODE_EDIT, tr("Edit"));
    mOpenForComboBox->insertItem(VerseAct::MODE_QUIZ, tr("Quiz"));

    mMainSplitter->refresh();
    qApp->installEventFilter(this);
    connect(editMenu, SIGNAL(aboutToShow()), this, 
            SLOT(editMenu_aboutToShow()));
    connect(mVerseSelection, SIGNAL(selectionChanged(bool)), this,
            SLOT(mVerseSelection_selectionChanged(bool)));
    connect(mVerseSelection, SIGNAL(verseActivated(Verse*)), this,
            SLOT(mVerseSelection_verseActivated(Verse*)));
    connect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );
    if (Settings::getOpenLastFile() && QFile::exists(Settings::getLastFile())){
        openFile(Settings::getLastFile());
    }
}

void MainAct::changeVerseCollection(VerseCollection* newCollection)
{
    mVerseSelection->changeVerseCollection(newCollection);
    if (mVerses != 0)
    {
        delete mVerses;
    }
    mVerses = newCollection;
    initVerseCollection();
}

void MainAct::initVerseCollection()
{
    mCategoryListBox->clear();
    mCategoryListBox->addItems(mVerses->getCategories());
    connect(mVerses, SIGNAL(categoryAdded(const QString&)), this,
            SLOT(categoryAdded(const QString&)));
    connect(mVerses, SIGNAL(categoryRenamed(const QString&, const QString&)),
            this, SLOT(categoryRenamed(const QString&, const QString&)));
    connect(mVerses, SIGNAL(categoryRemoved(const QString&)), this,
            SLOT(categoryRemoved(const QString&)));
}

void MainAct::mAddButton_clicked()
{
    Verse* newVerse = mVerses->addNewVerse();
    newVerse->setTranslation(Settings::getDefaultTranslation());
    openVerse(newVerse, VerseAct::MODE_EDIT);
}

void MainAct::fileExitAction_activated()
{
        qApp->closeAllWindows();
}

void MainAct::closeEvent(QCloseEvent* evt){
    if (promptSave()){
        Settings::setLastFile(mCurrentFileName);
        evt->accept();
    }
    else{
        evt->ignore();
    }
}

void MainAct::helpAboutAction_activated()
{
    AboutAct(this).exec();
}

void MainAct::setCurrentFileName(const QString &newFile)
{
    mCurrentFileName = newFile;
    QString shownName = newFile;
    if (shownName == "")
    {
        shownName = tr("Untitled");
    }
    setWindowTitle(tr("BibleMemorizer") + " - " + shownName);
}

bool MainAct::promptSave()
{
    if (mVerses->hasChanged())
    {
        int choice = QMessageBox::warning(this, tr("BibleMemorizer"),
            tr("The current collection has been modified.\n"
            "Do you want to save the changes?"),
            QMessageBox::Yes | QMessageBox::Default,
            QMessageBox::No,
            QMessageBox::Cancel | QMessageBox::Escape);
        if (choice == QMessageBox::Yes)
        {
            return save();
        }
        else if (choice == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainAct::save()
{
     if (mCurrentFileName == "")
     {
        return saveAs();
     }
     else
     {
        mVerses->saveFile(mCurrentFileName);
        return true;
     }
}

bool MainAct::saveAs()
{
    QString fileName = "";
    bool keepGoing = true;
    while (keepGoing)
    {
        QString passName = mCurrentFileName;
        if (passName == "")
            passName = ".";
        fileName = QFileDialog::getSaveFileName(this, QString(), passName, mFileFilters);
        if (fileName.isEmpty())
            return false;
        if (!fileName.endsWith(".bvc", Qt::CaseInsensitive)){
            fileName.append(".bvc");
        }
        if (QFile::exists(fileName)){
            int choice = QMessageBox::warning(this, tr("BibleMemorizer"),
                tr("The selected file already exists.\n"
                "Do you want to overwrite it?"),
                QMessageBox::Yes | QMessageBox::Default,
                QMessageBox::No,
                QMessageBox::Cancel | QMessageBox::Escape); 
            if (choice == QMessageBox::Yes)
            {
                keepGoing = false;
            }
            else if (choice == QMessageBox::No)
            {
                //do nothing, we will keep going.
            }
            else if (choice == QMessageBox::Cancel)
            {
                return false;
            }
        }
        else
        {
            keepGoing = false;
        }
    }
    if (!fileName.isEmpty())
    {
        mVerses->saveFile(fileName);
        setCurrentFileName(fileName);
    }
    return true;
}

void MainAct::openFile(const QString &fileName)
{
    int status = 0;
    VerseCollection* newCollection = new VerseCollection(fileName, status);
    switch (status)
    {
    case 1:
        QMessageBox::critical(this, tr("BibleMemorizer - Load Error"),
                    tr("The selected file could not be loaded."));
        delete newCollection;
        break;
    case 2:
        QMessageBox::warning(this, tr("BibleMemorizer - Load Warning"),
                    tr("The selected file indicates that it conforms to a "
                       "newer version of the verse collection format\n"
                       "than this version of BibleMemorizer uses.  Some "
                       "features may not work correctly."));
    default:
        changeVerseCollection(newCollection);
        setCurrentFileName(fileName);
    }
}

void MainAct::mOpenButton_clicked()
{
    Verse* verse = mVerseSelection->getSelectedVerse();
    if (verse != 0)
    {
        openVerse(verse, static_cast<VerseAct::PageType>
                (mOpenForComboBox->currentIndex()));
    }
}

void MainAct::mVerseSelection_selectionChanged(bool verseSelected)
{
    mOpenButton->setEnabled(verseSelected);
    mRemoveButton->setEnabled(verseSelected);
}

void MainAct::fileOpenAction_activated()
{
    if (promptSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, QString(), ".", mFileFilters);
        if (!fileName.isNull())
        {
            openFile(fileName);
        }
    }
}

void MainAct::fileSaveAction_activated()
{
    save();
}

void MainAct::fileSaveAsAction_activated()
{
    saveAs();
}

void MainAct::mRemoveButton_clicked()
{
    int choice = QMessageBox::warning(this, tr("BibleMemorizer"),
        tr("Are you sure you want to remove this verse?"),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No | QMessageBox::Escape);
    if (choice == QMessageBox::Yes)
    {
        Verse* verse = mVerseSelection->getSelectedVerse();
        bool removed = mVerses->removeVerse(verse);
        if (!removed)
            delete verse;
    }
}

void MainAct::fileNewAction_activated()
{
    if (promptSave())
    {
        changeVerseCollection(new VerseCollection());
        setCurrentFileName("");
    }
}

void MainAct::mAddCategoryButton_clicked()
{
    bool ok;
    QString newCat = QInputDialog::getText(this,
            tr("BibleMemorizer"), 
            tr("Enter the name of the new category:"),
            QLineEdit::Normal, QString(), &ok);
    newCat = newCat.simplified();
    if (ok && !newCat.isEmpty() && !mVerses->containsCategory(newCat)) 
    {
        mVerses->addCategory(newCat);
    }
    else if (mVerses->containsCategory(newCat))
    {
        QMessageBox::information(this, tr("BibleMemorizer"), 
                tr("You have already created that category"));
        mCategoryListBox->setCurrentItem(mCategoryListBox->findItems(newCat,
                Qt::MatchCaseSensitive | Qt::MatchExactly).first());
    }
}

void MainAct::categoryAdded(const QString& category)
{
    mCategoryListBox->addItem(category);
}

void MainAct::mRenameCategoryButton_clicked()
{
    bool ok;
    QString oldCat = mCategoryListBox->currentItem()->text();
    QString newCat = QInputDialog::getText(
            this,
            tr("BibleMemorizer"), 
            tr("Enter the new name for this category:"),
            QLineEdit::Normal, QString(), &ok);
    newCat = newCat.simplified();
    if (ok && !newCat.isEmpty() && !mVerses->containsCategory(newCat) &
            !newCat.contains(';')) 
    {
        mVerses->renameCategory(oldCat, newCat, true);
    }
    else if (mVerses->containsCategory(newCat))
    {
        QMessageBox::information(this, tr("BibleMemorizer"), 
                tr("You have already created that category"));
        mCategoryListBox->setCurrentItem(mCategoryListBox->findItems(newCat, 
                Qt::MatchCaseSensitive | Qt::MatchExactly).first());
    }
    else if (newCat.contains(';'))
    {
        QMessageBox::information(this, tr("BibleMemorizer"), 
                tr("Semicolons are used internally and cannot be in "
                "category names."));
    }
}

void MainAct::categoryRenamed(const QString& oldName, const QString& newName)
{
    QList<QListWidgetItem*> items = mCategoryListBox->findItems(oldName, Qt::MatchExactly | Qt::MatchCaseSensitive);
    items[0]->setText(newName);
}

void MainAct::mRemoveCategoryButton_clicked()
{
    QString oldCat = mCategoryListBox->currentItem()->text();
    int choice = QMessageBox::warning(this, tr("BibleMemorizer"),
        tr("Are you sure you want to remove this category?\n\n"
           "No verses will be deleted, but the category will be\n"
           "removed from all category lists."),
        QMessageBox::Yes | QMessageBox::Default,
        QMessageBox::No | QMessageBox::Escape);
    if (choice == QMessageBox::Yes)
    {
        mVerses->removeCategory(oldCat, true);
    }
}

void MainAct::categoryRemoved(const QString& category)
{
    QList<QListWidgetItem*> items = mCategoryListBox->findItems(category, Qt::MatchExactly | Qt::MatchCaseSensitive);
    foreach(QListWidgetItem *item, items)
    {
        mCategoryListBox->removeItemWidget(item);
    }
}

MainAct::~MainAct()
{
}

void MainAct::mCategoryListBox_selectionChanged()
{
    if (mCategoryListBox->currentItem())
    {
        mRenameCategoryButton->setEnabled(true);
        mRemoveCategoryButton->setEnabled(true);
    }
    else
    {
        mRenameCategoryButton->setEnabled(false);
        mRemoveCategoryButton->setEnabled(false);
    }
}

void MainAct::preferencesSettingsAction_activated()
{
    PreferencesAct pluginSelect;
    int result = pluginSelect.exec();
    if (result == QDialog::Accepted)
    {
        BiblePlugin* newPlugin = new BiblePlugin(pluginSelect.getMeta());
        for (int i = 0; i < mTabWidget->count(); i++)
        {
            QWidget *nextPage = mTabWidget->widget(i);
            if (VerseAct* nextWidget = dynamic_cast<VerseAct*>(nextPage))
            {
                nextWidget->setPlugin(newPlugin);
            }
        }
        delete mPlugin;
        mPlugin = newPlugin;
    }
}

void MainAct::mVerseSelection_verseActivated(Verse* verse)
{
    if (verse != 0)
    {
        openVerse(verse, static_cast<VerseAct::PageType>
                (mOpenForComboBox->currentIndex()));
    }
}

void MainAct::openVerse(Verse* verse, VerseAct::PageType pageType)
{
    if (verse != 0)
    {
        if (mVersePaneMap[verse] &&
                (mTabWidget->indexOf(mVersePaneMap[verse]) != -1)){
            VerseAct* act = mVersePaneMap[verse];
            act->choosePage(pageType);
            mTabWidget->setCurrentWidget(mVersePaneMap[verse]);
        }
        else{
            VerseAct* newAct = new VerseAct(verse, mPlugin,
                    mVerses->getCategories(), pageType, *mTabWidget, mVerses);
            mTabWidget->addTab(newAct, verse->getReference());
            mTabWidget->setCurrentWidget(newAct);
            mVersePaneMap[verse] = newAct;
        }
    }
    else
    {
        qWarning("Null verse.");
    }
}

void MainAct::quizTextAction_activated()
{
    doQuiz(QuizMultiAct::MODE_TEXT);
}

void MainAct::quizReferenceAction_activated()
{
    doQuiz(QuizMultiAct::MODE_REFERENCE);
}

void MainAct::doQuiz(QuizMultiAct::QuizMode mode)
{
    disconnect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );
    hide();
    QuizCreateAct* createAct = new QuizCreateAct(mVerses, mode, this);
    createAct->exec();
    show();
    connect( qApp, SIGNAL( lastWindowClosed() ), qApp, SLOT( quit() ) );
}

void MainAct::editMenu_aboutToShow()
{
    disconnect(editCopyAction, SIGNAL(activated()), 0, 0);
    disconnect(editCutAction, SIGNAL(activated()), 0, 0);
    disconnect(editPasteAction, SIGNAL(activated()), 0, 0);
    disconnect(editUndoAction, SIGNAL(activated()), 0, 0);
    disconnect(editRedoAction, SIGNAL(activated()), 0, 0);
    if ((currWidget != 0) && (currWidget->inherits("QTextEdit") ||
            currWidget->inherits("QLineEdit") ||
            (currWidget->inherits("QComboBox") &&
            static_cast<QComboBox*>(currWidget)->lineEdit() != NULL)))
    {
        QWidget* receiver = NULL;
        if (currWidget->inherits("QTextEdit"))
        {
            QTextEdit* currEdit = static_cast<QTextEdit*>(currWidget);
            receiver = currEdit;
            // TODO:
            //editCopyAction->setEnabled(currEdit->hasSelectedText());
            //editCutAction->setEnabled(currEdit->hasSelectedText() &&
            //        !currEdit->isReadOnly());
            //editPasteAction->setEnabled(!currEdit->isReadOnly());
            //editUndoAction->setEnabled(currEdit->isUndoAvailable());
            //editRedoAction->setEnabled(currEdit->isRedoAvailable());
        }
        else if (currWidget->inherits("QComboBox") ||
                currWidget->inherits("QLineEdit"))
        {
            QLineEdit* currEdit;
            if (currWidget->inherits("QComboBox"))
            {
                currEdit = static_cast<QComboBox*>(currWidget)->lineEdit();
            }
            else
            {
                currEdit = static_cast<QLineEdit*>(currWidget);
            }
            receiver = currEdit;
            editCopyAction->setEnabled(currEdit->hasSelectedText());
            editCutAction->setEnabled(currEdit->hasSelectedText() &&
                    !currEdit->isReadOnly());
            editPasteAction->setEnabled(!currEdit->isReadOnly());
            editUndoAction->setEnabled(currEdit->isUndoAvailable());
            editRedoAction->setEnabled(currEdit->isRedoAvailable());
        }
        editPasteAction->setEnabled(editPasteAction->isEnabled() && 
                !qApp->clipboard()->text(QClipboard::Clipboard).isEmpty());
        connect(editCopyAction, SIGNAL(activated()), receiver, SLOT(copy()));
        connect(editCutAction, SIGNAL(activated()), receiver, SLOT(cut()));
        connect(editPasteAction, SIGNAL(activated()), receiver, SLOT(paste()));
        connect(editUndoAction, SIGNAL(activated()), receiver, SLOT(undo()));
        connect(editRedoAction, SIGNAL(activated()), receiver, SLOT(redo()));
    }
    else
    {
        editCopyAction->setEnabled(false);
        editCutAction->setEnabled(false);
        editPasteAction->setEnabled(false);
        editUndoAction->setEnabled(false);
        editRedoAction->setEnabled(false);
    }
}

bool MainAct::eventFilter(QObject* watched, QEvent* e)
{
    if (e->type() == QEvent::FocusIn)
    {
        if (watched->inherits("QWidget") && !watched->inherits("QMenuBar"))
        {
            currWidget = static_cast<QWidget*>(watched); 
        }
    }
    return false;
}

}

