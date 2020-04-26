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

#include "core/bibleplugin.h"
#include "core/biblepluginmeta.h"
#include "core/ubook.h"
#include "core/bmemversion.h"
#include <qlibrary.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <qfile.h>
#include <map>

namespace bmemcore
{

BiblePlugin::BiblePlugin(BiblePluginMeta meta, bool* success)
    :mMeta(meta), mPlugin(meta.getFileName()), QObject()
{
    if (!mPlugin.load() || mMeta.getInterfaceVersion() != 
                                BMEM_SUPPORTED_PLUGIN_INTERFACE)
    {
        if (success)
        {
            *success = false;
        }
        else{
            if (mMeta.getInterfaceVersion() !=
                                BMEM_SUPPORTED_PLUGIN_INTERFACE)
            {
                QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                    "The plugin is incompatible with this version\n"
                    "of BibleMemorizer.  You may see further error\n"
                    "messages."), QMessageBox::Ok, QMessageBox::NoButton);
            }
            else
            {
                QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                    "The plugin could not be loaded.  You\n"
                    "may see further error messages."),
                    QMessageBox::Ok, QMessageBox::NoButton);
            }
        }
        p_initialize = NULL;
        p_finalize = NULL;
        p_getBooks = NULL;
        p_booksChanged = NULL;
        p_book = NULL;
        p_uBook = NULL;
        p_translationsAvailable = NULL;
        p_verseLoaderAvailable = NULL;
        return;
    }
    if (success)
    {
        *success = true;
    }
    //Resolve addresses.
    p_initialize = (voidFunc)(mPlugin.resolve("initialize"));
    p_finalize = (voidFunc)(mPlugin.resolve("finalize"));
    p_getBooks = (bookListFunc)(mPlugin.resolve("getBooks"));
    p_booksChanged = (booksChangedFunc)(mPlugin.resolve("booksChanged"));
    p_book = (bookFromUFunc)(mPlugin.resolve("book"));
    p_uBook = (bookToUFunc)(mPlugin.resolve("uBook"));
    p_translationsAvailable = 
            (boolFunc)(mPlugin.resolve("translationsAvailable"));
    if (translationsAvailable())
    {
        p_getTranslations = (transListFunc)(mPlugin.resolve("getTranslations"));
    }
    p_getNumChapters = (numChapFunc)(mPlugin.resolve("getNumChapters"));
    p_verseLoaderAvailable = 
            (boolFunc)(mPlugin.resolve("verseLoaderAvailable"));
    if (verseLoaderAvailable())
    {
        p_getVerse = (verseGetFunc)(mPlugin.resolve("getVerse"));
        p_verseAvailable = (verseAvailFunc)(mPlugin.resolve("verseAvailable"));
        p_verseCount = (verseCountFunc)(mPlugin.resolve("verseCount"));
    }
    useCountMap()[mMeta.getFileName()]++;
    //Initialize the plugin
    if (p_initialize != NULL && useCountMap()[mMeta.getFileName()] == 1){
        p_initialize();
    }
    else if (p_initialize == NULL){
        QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                "Either the plugin could not be loaded properly,\n"
                "or it does not contain an initializer function."),
                QMessageBox::Ok, QMessageBox::NoButton);
    }
}

QStringList BiblePlugin::getBooks(const QString& translation)
{
    if (mBooksList.empty() || booksChanged(translation))
    {
        if (p_getBooks != NULL){
            int size = 0;
            const char * const * bookList = p_getBooks(&size,
                    translation.toUtf8().data());
            mBooksList = toStringList(bookList, size);
        }
        else{
            QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                    "The plugin either could not be loaded properly,\n"
                    "or it does not allow retrieval of books."),
                    QMessageBox::Ok, QMessageBox::NoButton);
        }
    }
    return mBooksList;
}

bool BiblePlugin::booksChanged(const QString& translation)
{
    if (p_booksChanged != NULL){
        return static_cast<bool>(p_booksChanged(mLastTranslation.toUtf8().data(),
                translation.toUtf8().data()));
    }
    else{
        QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                "The plugin either could not be loaded properly,\n"
                "or it does not handle book name retrieval\n"
                "properly."), QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
}

QString BiblePlugin::book(uBookType uBook)
{
    if (p_book != NULL){
        return toString(p_book(uBook));
    }
    else{
        QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                "The plugin either could not be loaded properly,\n"
                "or it does not allow conversion from numeric\n"
                "book to name."),
                QMessageBox::Ok, QMessageBox::NoButton);
        return tr("Unable to retrieve book.");
    }
}

uBookType BiblePlugin::uBook(const QString& book)
{
    if (p_uBook != NULL){
        return p_uBook(book.toUtf8().data());
    }
    else{
        QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                "The plugin either could not be loaded properly,\n"
                "or it does not allow conversion from name to\n"
                "numeric book."), QMessageBox::Ok, QMessageBox::NoButton);
        uBookType newBook;
        newBook.book = 0;
        newBook.testament = 0;
        return newBook;
    }
}

bool BiblePlugin::translationsAvailable()
{
    if (p_translationsAvailable != NULL){
        return static_cast<bool>(p_translationsAvailable());
    }
    else{
        QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                "The plugin either could not be loaded properly,\n"
                "or it does not allow its translation feature to\n"
                "be queried."), QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
}

QStringList BiblePlugin::getTranslations()
{
    if (translationsAvailable() && mTranslationsList.empty())
    {
        if (p_getTranslations != NULL){
            int size = 0;
            const char * const * translationList = p_getTranslations(&size);
            mTranslationsList = toStringList(translationList, size);
        }
        else{
            QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                    "The plugin either could not be loaded properly,\n"
                    "or it falsely reports that it can list\n"
                    "translations even though it can't."), QMessageBox::Ok,
                    QMessageBox::NoButton);
        }
    }
    return mTranslationsList;
}

int BiblePlugin::getNumChapters(const QString& book, uBookType uBook)
{
    if (p_getNumChapters != NULL){
        return p_getNumChapters(book.toUtf8().data(), uBook);
    }
    else{
        QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                "The plugin either could not be loaded properly,\n"
                "or it does not support retrieval of number of\n"
                "chapters."), QMessageBox::Ok, QMessageBox::NoButton);
        return 0;
    }
}

bool BiblePlugin::verseLoaderAvailable()
{
    if (p_verseLoaderAvailable != NULL){
        return static_cast<bool>(p_verseLoaderAvailable());
    }
    else{
        QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                "The plugin either could not be loaded properly,\n"
                "or it does not support querying whether it can\n"
                "load verse text."), QMessageBox::Ok, QMessageBox::NoButton);
        return false;
    }
}

QString BiblePlugin::getVerse(const QString& book, uBookType uBook,
        const QString& chapter, const QString& verses,
        const QString& translation)
{
    if (verseLoaderAvailable() && verseAvailable(book, uBook, chapter,
            verses, translation))
    {
        if (p_getVerse != NULL){
            return toString(p_getVerse(book.toUtf8().data(), uBook, chapter.toUtf8().data(),
                    verses.toUtf8().data(), translation.toUtf8().data())).trimmed();
        }
        else{
            QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                    "The plugin either could not be loaded properly,\n"
                    "or it falsely reports that it can load verse\n"
                    "texts even though it cannot."), QMessageBox::Ok,
                    QMessageBox::NoButton);
            return "";
        }
    }
    else
    {
        return "";
    }
}

bool BiblePlugin::verseAvailable(const QString& book, uBookType uBook, 
        const QString& chapter, const QString& verses,
        const QString& translation)
{
    if (verseLoaderAvailable())
    {
        if (p_verseAvailable != NULL){
            return static_cast<bool>(p_verseAvailable(book.toUtf8().data(), uBook,
                    chapter.toUtf8().data(), verses.toUtf8().data(), translation.toUtf8().data()));
        }
        else{
            QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                    "The plugin either could not be loaded properly,\n"
                    "or it falsely reports that it can query verse\n"
                    "texts even though it cannot."), QMessageBox::Ok,
                    QMessageBox::NoButton);
            return false;
        }
    }
    else
        return false;
}

int BiblePlugin::verseCount(const QString& book, uBookType uBook, 
        const QString& chapter, const QString& verses,
        const QString& translation)
{
    if (verseLoaderAvailable())
    {
        if (p_verseCount != NULL){
            return p_verseCount(book.toUtf8().data(), uBook,
                    chapter.toUtf8().data(), verses.toUtf8().data(), translation.toUtf8().data());
        }
        else{
            QMessageBox::warning(NULL, tr("Plugin Error"), tr(
                    "The plugin either could not be loaded properly,\n"
                    "or falsely reports that it can query verse\n"
                    "texts even though it cannot."), QMessageBox::Ok,
                    QMessageBox::NoButton);
            return 0;
        }
    }
    else
        return false;
}

BiblePluginMeta& BiblePlugin::getMeta()
{
    return mMeta;
}

QStringList BiblePlugin::toStringList(const char * const * toConvert, int size)
{
    QStringList toReturn;
    for (int i = 0; i < size; i++)
    {
        toReturn.push_back(toString(toConvert[i]));
    }
    return toReturn;
}

QString BiblePlugin::toString(const char * toConvert)
{
    return QString::fromUtf8(toConvert);
}

std::map<QString, int>& BiblePlugin::useCountMap(){
    static std::map<QString, int> toReturn;
    return toReturn;
}

BiblePlugin::~BiblePlugin()
{
    //Finalize the plugin.
    if (useCountMap()[mMeta.getFileName()] > 0){
        useCountMap()[mMeta.getFileName()]--;
    }
    if (p_finalize != NULL && useCountMap()[mMeta.getFileName()] == 0){
        p_finalize();
    }
}


}

