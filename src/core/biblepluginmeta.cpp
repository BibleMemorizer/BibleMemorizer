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

#include "core/biblepluginmeta.h"
#include "qt_types.h"
#include <qstring.h>
#include <qfile.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qobject.h>
#include <qregexp.h>

namespace bmemcore
{

BiblePluginMeta::BiblePluginMeta()
:mName(tr("No Name")), mShortName(tr("N/A")), mVersion(tr("N/A")),
        mInterfaceVersion(tr("Unknown")), mAuthors(tr("Not Provided")),
        mLicenseName(tr("Unknown")), mLicense(tr("Not Provided")),
        QObject()
{}

BiblePluginMeta::BiblePluginMeta(const BiblePluginMeta& other)
:mFileName(other.mFileName), mMetaFileName(other.mMetaFileName),
    mName(other.mName), mShortName(other.mShortName), mVersion(other.mVersion),
    mDescription(other.mDescription),
    mInterfaceVersion(other.mInterfaceVersion), mAuthors(other.mAuthors),
    mWebSite(other.mWebSite), mCopyright(other.mCopyright),
    mLicenseName(other.mLicenseName), mLicense(other.mLicense),
    QObject()
{}

BiblePluginMeta& BiblePluginMeta::operator=(const BiblePluginMeta& other)
{
    mFileName = other.mFileName;
    mMetaFileName = other.mMetaFileName;
    mName = other.mName;
    mShortName = other.mShortName;
    mVersion = other.mVersion;
    mDescription = other.mDescription;
    mInterfaceVersion = other.mInterfaceVersion;
    mAuthors = other.mAuthors;
    mWebSite = other.mWebSite;
    mCopyright = other.mCopyright;
    mLicenseName = other.mLicenseName;
    mLicense = other.mLicense;
    return *this;
}

BiblePluginMeta::BiblePluginMeta(const QString& fileName)
:mMetaFileName(fileName), mName(tr("No Name")), mShortName(tr("Unknown")),
        mVersion(tr("(Version Not Specified)")),
        mDescription(tr("No Description Provided")),
        mInterfaceVersion(tr("Unknown")), mAuthors(tr("Not Provided")),
        mWebSite(tr("No Web Site")), mCopyright(tr("Unknown Copyright Status")),
        mLicenseName(tr("Unknown")),
        mLicense(tr("Not Provided")), QObject()
{
    QFile file(fileName);
    QFileInfo fileInfo(file);
    bool success = file.open(bmemui::BIO_ReadOnly);
    if (!success)
        return;
    QTextStream stream(&file);
    stream.setEncoding(QTextStream::UnicodeUTF8);
    while (!stream.atEnd())
    {
        QString nextLine = stream.readLine();
        if (nextLine.startsWith("FileName="))
        {
            mFileName = QDir::cleanDirPath(fileInfo.dirPath(true) + "/" + 
                    nextLine.mid(9));
        }
        if (nextLine.startsWith("Name="))
        {
            mName = nextLine.mid(5);
            if (mName.isEmpty())
                mName = tr("No Name");
        }
        else if (nextLine.startsWith("ShortName="))
        {
            mShortName = nextLine.mid(10);
            if (mShortName.isEmpty())
                mShortName = tr("Unknown");
        }
        else if (nextLine.startsWith("Version="))
        {
            mVersion = nextLine.mid(8);
            if (mVersion.isEmpty())
                mVersion = tr("(Version Not Specified)");
        }
        else if (nextLine.startsWith("Description="))
        {
            mDescription = nextLine.mid(12);
            if (mDescription.isEmpty())
                mVersion = tr("No Description Provided");
        }
        else if (nextLine.startsWith("InterfaceVersion="))
        {
            mInterfaceVersion = nextLine.mid(17);
            if (mInterfaceVersion.isEmpty())
                mInterfaceVersion = tr("Unknown");
        }
        else if (nextLine.startsWith("AuthorFile="))
        {
            QString authorFileName = fileInfo.dirPath(true) + "/" + 
                    nextLine.mid(11);
            if (!nextLine.mid(11).isEmpty() && QFile::exists(authorFileName))
            {
                QFile authorFile(authorFileName);
                authorFile.open(bmemui::BIO_ReadOnly);
                QTextStream authorStream(&authorFile);
                authorStream.setEncoding(QTextStream::UnicodeUTF8);
                mAuthors = authorStream.read();
                authorFile.close();
            }
            if (mAuthors.isEmpty())
                mAuthors = tr("Not Provided");
        }
        else if (nextLine.startsWith("WebSite="))
        {
            mWebSite = nextLine.mid(8);
            if (mWebSite.isEmpty())
                mWebSite = tr("No Web Site");
        }
        else if (nextLine.startsWith("Copyright="))
        {
            mCopyright = nextLine.mid(10);
            if (mCopyright.isEmpty())
                mCopyright = tr("Unknown Copyright Status");
        }
        else if (nextLine.startsWith("License="))
        {
            mLicenseName = nextLine.mid(8);
            if (mLicenseName.isEmpty())
                mLicenseName = tr("Unknown");
        }
        else if (nextLine.startsWith("LicenseFile="))
        {
            QString licenseFileName = fileInfo.dirPath(true) + "/" + 
                    nextLine.mid(12);
            if (!nextLine.mid(12).isEmpty() && QFile::exists(licenseFileName))
            {
                QFile licenseFile(licenseFileName);
                licenseFile.open(bmemui::BIO_ReadOnly);
                QTextStream licenseStream(&licenseFile);
                licenseStream.setEncoding(QTextStream::UnicodeUTF8);
                mLicense = licenseStream.read();
                licenseFile.close();
            }
            if (mLicense.isEmpty())
                mLicense = tr("Not Provided");
        }
    }
    file.close();
}

QString BiblePluginMeta::getBaseName() const
{
    //Matches any number of non-slash characters followed by .plugin at the end
    QRegExp nameExp("([^\\\\\\/]*)\\.plugin$");
    nameExp.search(mMetaFileName);
    return nameExp.cap(1);
}

QString BiblePluginMeta::getFileName() const
{
    return mFileName;
}

QString BiblePluginMeta::getMetaFileName() const
{
    return mMetaFileName;
}

QString BiblePluginMeta::getName() const
{
    return mName;
}

QString BiblePluginMeta::getShortName() const
{
    return mShortName;
}

QString BiblePluginMeta::getVersion() const
{
    return mVersion;
}

QString BiblePluginMeta::getDescription() const
{
    return mDescription;
}

QString BiblePluginMeta::getInterfaceVersion() const
{
    return mInterfaceVersion;
}

QString BiblePluginMeta::getAuthors() const
{
    return mAuthors;
}

QString BiblePluginMeta::getWebSite() const
{
    return mWebSite;
}

QString BiblePluginMeta::getCopyright() const
{
    return mCopyright;
}

QString BiblePluginMeta::getLicenseName() const
{
    return mLicenseName;
}

QString BiblePluginMeta::getLicense() const
{
    return mLicense;
}

bool BiblePluginMeta::operator==(const BiblePluginMeta& other) const
{
    return (getMetaFileName() == other.getMetaFileName());
}

BiblePluginMeta::~BiblePluginMeta()
{}


}

