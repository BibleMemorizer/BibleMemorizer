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

#ifndef BMEMCOREBIBLEPLUGIN_H
#define BMEMCOREBIBLEPLUGIN_H
#include <qlibrary.h>
#include "core/ubook.h"
#include "core/biblepluginmeta.h"
#include <qstringlist.h>
#include <qstring.h>
#include <qobject.h>
#include <map>
namespace bmemcore
{

/**
@author Jeremy Erickson
*/
class BiblePlugin : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructs a new BiblePlugin.
     * @param meta BiblePluginMeta which refers to the proper plugin to use.
     */
    BiblePlugin(BiblePluginMeta meta, bool * success = 0);
    /**
     * @param translation Translation for which to get book list (e.g. if New
     * Testament only, may not contain Old Testament books, depending on
     * plugin.)  May be ignored.
     * @return List with all the books available in the translation.
     */
    QStringList getBooks(const QString& translation);
    /**
     * Used to determine whether the list of books has changed since the
     * previous list of books was loaded (from changing translation.)
     * @param translation The new translation to test for changes.
     * @return Whether the list changed.
     */
    bool booksChanged(const QString& translation);
    /**
     * Converts a uBook to a book name.
     */
    QString book(uBookType uBook);
    /**
     * Converts a book name to a uBook.
     */
    uBookType uBook(const QString& book);
    /**
     * @return Whether the plugin supports loading a translation list.
     */
    bool translationsAvailable();
    /**
     * @return List of available translations.
     */
    QStringList getTranslations();
    /**
     * Used to determine the number of chapters in a book.
     * Only one parameter need be valid (the plugin should return the first
     * information it finds that matches either.)
     * @param book Name of the book
     * @param uBook uBook for the same book.
     */
    int getNumChapters(const QString& book, uBookType uBook);
    /**
     * @return Whether the plugin can load verses.
     */
    bool verseLoaderAvailable();
    /**
     * Used to load a verse from plugins that allow this.
     * @param book The book for the verse.
     * @param uBook The uBook for the verse.
     * @param chapter The chapter.
     * @param verse Verse number (or range) within the chapter.
     * @param translation Translation from which to obtain verse.
     * @return The text of the verse or verses.
     */
    QString getVerse(const QString& book, uBookType uBook,
                     const QString& chapter, const QString& verses,
                     const QString& translation);
    /**
     * Checks whether a plugin can load a verse.
     * @param book The book for the verse.
     * @param uBook The uBook for the verse.
     * @param chapter The chapter.
     * @param verse Verse number (or range) within the chapter.
     * @param translation Translation from which to obtain verse.
     * @return Whether the plugin can find the text of the verse.
     */
    bool verseAvailable(const QString& book, uBookType uBook,
                        const QString& chapter, const QString& verses,
                        const QString& translation);
    /**
     * Checks how many verses are in a query.
     * @param book The book for the verse.
     * @param uBook The uBook for the verse.
     * @param chapter The chapter.
     * @param verse Verse number (or range) within the chapter.
     * @param translation Translation from which to obtain verse.
     * @return Whether the plugin can find the text of the verse.
     */
    int verseCount(const QString& book, uBookType uBook,
                        const QString& chapter, const QString& verses,
                        const QString& translation);
    /**
     * @return The BiblePluginMeta used to construct this plugin. 
     */
    BiblePluginMeta& getMeta();
    ~BiblePlugin();
protected:
    BiblePluginMeta mMeta;
    //QLibrary manages loading the plugin and calling functions.
    QLibrary mPlugin;
    //Data obtained from the plugin.
    QStringList mBooksList;
    QString mLastTranslation;
    QStringList mTranslationsList;
    static std::map<QString, int>& useCountMap();
    //Used to convert const char * const * to QStringList
    inline QStringList toStringList(const char * const *, int);
    //Used to convert between const char * and QString
    inline QString toString(const char *);
    //Typedefs for functions in plugin
    typedef void (*voidFunc)();
    typedef const char * const * (*bookListFunc)(int *, const char *);
    typedef int (*booksChangedFunc)(const char *, const char *);
    typedef const char * (*bookFromUFunc)(uBookType);
    typedef uBookType (*bookToUFunc)(const char *);
    typedef int (*boolFunc)();
    typedef const char * const * (*transListFunc)(int *);
    typedef int (*numChapFunc)(const char *, uBookType);
    typedef const char * (*verseGetFunc)(const char *, uBookType, const char *, const char *, const char *);
    typedef int (*verseAvailFunc)(const char *, uBookType, const char *, const char *, const char *);
    typedef int (*verseCountFunc)(const char *, uBookType, const char *, const char *, const char *);
    //The actual function addresses for the plugin.
    voidFunc p_initialize;
    voidFunc p_finalize;
    bookListFunc p_getBooks;
    booksChangedFunc p_booksChanged;
    bookFromUFunc p_book;
    bookToUFunc p_uBook;
    boolFunc p_translationsAvailable;
    transListFunc p_getTranslations;
    boolFunc p_verseLoaderAvailable;
    numChapFunc p_getNumChapters;
    verseGetFunc p_getVerse;
    verseAvailFunc p_verseAvailable;
    verseCountFunc p_verseCount;
};

}

#endif
