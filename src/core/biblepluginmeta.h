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

#ifndef BMEMCOREBIBLEPLUGINMETA_H
#define BMEMCOREBIBLEPLUGINMETA_H
#include <qstring.h>
#include <qobject.h>
namespace bmemcore
{

/**
@author Jeremy Erickson
*/
class BiblePluginMeta : public QObject
{
    Q_OBJECT
public:
    /**
     * Creates a dummy BiblePluginMeta.
     */
    BiblePluginMeta();
    /**
     * Explicit copy constructor and operator= for QObject compatibility
     */
    BiblePluginMeta(const BiblePluginMeta&);
    BiblePluginMeta& operator=(const BiblePluginMeta&);
    /**
     * Creates a new BiblePluginMeta.
     * @param fileName Name of file with information about the plugin.
     */
    BiblePluginMeta(const QString& fileName);
    /**
     * @return The file name of the plugin, without .plugin
     */
    QString getBaseName() const;
    /**
     * @return The filename of the library itself (e.g. .so or .dll)
     */
    QString getFileName() const;
    /**
     * @return The filename of the information file.
     */
    QString getMetaFileName() const;
    /**
     * @return The user-displayable name of the plugin.
     * This shows up in the plugin list.
     */
    QString getName() const;
    /**
     * @return The short name of the plugin.  Used e.g. in the "Get from X"
     * button on the UI.
     */
    QString getShortName() const;
    /**
     * @return The version of the plugin itself.
     */
    QString getVersion() const;
    /**
     * @return A description of the plugin.  May be multiple lines or include
     * HTML.
     */
    QString getDescription() const;
    /**
     * @return The version of the plugin's interface with BibleMemorizer.
     */
    QString getInterfaceVersion() const;
    /**
     * @return The names of the authors of the plugin.
     */
    QString getAuthors() const;
    /**
     * @return The URL of the plugin's web site.
     */
    QString getWebSite() const;
    /**
     * @return Copyright information for the plugin.
     */
    QString getCopyright() const;
    /**
     * @return Name of the license used for the plugin (should be short, e.g.
     * "GPL")
     */
    QString getLicenseName() const;
    /**
     * @return Full text of the license.  This will almost certainly contain
     * multiple lines, and it may contain HTML.
     */
    QString getLicense() const;
    /**
     * Compares whether two objects are identical by checking whether they
     * refer to the same metadata file.
     */
    bool operator==(const BiblePluginMeta& other) const;
    ~BiblePluginMeta();
private:
    QString mFileName;
    QString mMetaFileName;
    QString mName;
    QString mShortName;
    QString mVersion;
    QString mDescription;
    QString mInterfaceVersion;
    QString mAuthors;
    QString mWebSite;
    QString mCopyright;
    QString mLicenseName;
    QString mLicense;
};

}

#endif
