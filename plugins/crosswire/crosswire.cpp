/*
 * Copyright (C) 2005-2009 Jeremy Erickson
 *
 * This plugin is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to:
 * Free Software Foundation, Inc.
 * 59 Temple Place - Suite 330
 * Boston, MA  02111-1307, USA.
 */

#ifdef Q_WS_WIN
    #define DECORATION extern "C" __declspec(dllexport)
#else
    #define DECORATION extern "C"
#endif

#define TRUE 1
#define FALSE 0

#include "../bmemplugin.h"

#include <list>
#include <map>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#include <swmgr.h>
#include <swmodule.h>
#include <stringmgr.h>
#include "btstringmgr.h"
#include <encfiltmgr.h>
#include <versekey.h>
#include <listkey.h>

using namespace sword;

char ** books;
char ** translations;
list<string> booksList;
list<string> translationsList;
EncodingFilterMgr filterMgr(ENC_UTF8);
SWMgr* swordLib;

#ifdef Q_WS_WIN
int main()
{}
#endif

DECORATION void initialize()
{
    StringMgr::setSystemStringMgr(new BTStringMgr());
    VerseKey vk;
    swordLib = new SWMgr(&filterMgr);
/*    int ot = 0;
    int nt = 0;
    int a = 0;*/
    for (vk = TOP; !vk.Error(); vk.Book(vk.Book()+1)) {
        booksList.push_back(vk.getBookName());
    }
    for (ModMap::iterator it = swordLib->Modules.begin();
            it != swordLib->Modules.end(); it++) {
        if (!strcmp(it->second->Type(), "Biblical Texts"))
            translationsList.push_back(it->second->Name());
    }
    books = new char*[booksList.size()];
    int i = 0;
    for (list<string>::iterator it = booksList.begin(); it != booksList.end();
            it++)
    {
        char * encBook = new char[(*it).size() + 1];
        strcpy(encBook, (*it).c_str());
        books[i] = encBook;
        i++;
    }
    translations = new char*[translationsList.size()];
    i = 0;
    for (list<string>::iterator it = translationsList.begin();
            it != translationsList.end(); it++)
    {
        char * encTrans = new char[(*it).size() + 1];
        strcpy(encTrans, (*it).c_str());
        translations[i] = encTrans;
        i++;
    }
    //Disable footnotes, etc.
    swordLib->setGlobalOption("Footnotes", "Off");
    swordLib->setGlobalOption("Strong's Numbers", "Off");
    swordLib->setGlobalOption("Morphological Tags", "Off");
    swordLib->setGlobalOption("Lemmas", "Off");
    swordLib->setGlobalOption("Cross-references", "Off");
    swordLib->setGlobalOption("Textual Variants", "Off");
}

DECORATION void finalize()
{
    for(int i = 0; i < booksList.size(); i++)
    {
        delete books[i];
    }
    delete [] books;
    for (int i = 0; i < translationsList.size(); i++)
    {
        delete translations[i];
    }
    delete [] translations;
    //delete swordLib;
}

//Can accept a particular translation, in case a module has
//Different book availabilities on a per-translation basis.
DECORATION const char * const * getBooks(int * numBooks, const char *)
{
    *numBooks = booksList.size();
    return books;
}

//The two const char * parameters are for two translations.  If the translations
//have different book lists from each other, it should return TRUE.
//Otherwise, it should return FALSE.
//Here it will always return FALSE, since Sword as yet does not change book
//lists across modules.
DECORATION int booksChanged(const char *, const char *)
{
    return FALSE;
}

DECORATION const char * book(uBookType uBook)
{
    static VerseKey vk;
    switch (uBook.testament)
    {
    case OLD_TESTAMENT:
        vk.Testament(1);
        break;
    case NEW_TESTAMENT:
        vk.Testament(2);
        break;
    default:
        return "";
    }
    vk.Book(static_cast<char>(uBook.book));
    return vk.getBookName();
}

DECORATION const uBookType uBook(const char * book)
{
    uBookType toReturn;
    VerseKey vk(book);
    switch (vk.Testament())
    {
    case 1: //Old Testament
        toReturn.testament = OLD_TESTAMENT;
        break;
    case 2: //New Testament
        toReturn.testament = NEW_TESTAMENT;
        break;
    default: //Unknown.  Apocrypha, perhaps?
        toReturn.testament = 64;
        break;
    }
    toReturn.book = vk.Book();
    return toReturn;
}

//Sword has a module list.
DECORATION int translationsAvailable()
{
    return TRUE;
}

DECORATION const char * const * getTranslations(int * numTrans)
{
    *numTrans = translationsList.size();
    return translations;
}

DECORATION int getNumChapters(const char * vBook, uBookType uBook){
    if (VerseKey(vBook).Error())
        vBook = book(uBook);
    VerseKey vk(vBook);
    vk = MAXCHAPTER;
    return vk.Chapter();
}

//Sword includes support for importing verses.
DECORATION int verseLoaderAvailable()
{
    return TRUE;
}

//No DECORATION because it's not supposed to be directly usable by
//BibleMemorizer.
inline string getReference(const char * vBook, uBookType uBook,
        const char * chapter, const char * verses)
{
    string toReturn;
    if (vBook == "" && (uBook.testament != 0 && uBook.book != 0))
    {
        vBook = book(uBook);
    }
    toReturn += vBook;
    toReturn += " ";
    if (chapter != "")
    {
        toReturn += chapter;
        toReturn += ":";
    }
    toReturn += verses;
    return toReturn;
}

inline ListKey getListKey(const char * vBook, uBookType uBook,
                          const char * vChapter, const char * vVerses,
                          const char * vTranslation)
{
    if (VerseKey(vBook).Error())
        vBook = book(uBook);
    string key = getReference(vBook, uBook, vChapter, vVerses);
    // simple 1 verse
    //book->setKey((const char *)key);

    // but we need to call sword's verse parses
    // to handle ranges and other stuff
    VerseKey parser;
    ListKey verses = parser.ParseVerseList(key.c_str(), parser, true);
    return verses;
}

DECORATION const char * getVerse(const char * vBook, uBookType uBook,
                                const char * vChapter, const char * vVerses,
                                const char * vTranslation)
{
    SWModule *mod = swordLib->getModule(vTranslation);
    ListKey verses = getListKey(vBook, uBook, vChapter, vVerses, vTranslation);
    static string versesText;
    versesText = "";
    int first = TRUE;
    int currChapter;
    for (verses = TOP; !verses.Error(); verses++) {
        VerseKey verseInfo(verses);
        if (!first)
        {
            if (currChapter == verseInfo.Chapter())
            {
                versesText += " ";
            }
            else
            {
                versesText += "\n\n";
            }
        }
        else
        {
            first = FALSE;
        }
        currChapter = verseInfo.Chapter();
        mod->setKey((const char *)verses);
        versesText += mod->StripText();  // StripText is no markup or frills
    }
    return versesText.c_str();
}

DECORATION int verseAvailable(const char * vBook, uBookType uBook,
                    const char * vChapter, const char * vVerses,
                    const char * vTranslation)
{
    SWModule *mod = swordLib->getModule(vTranslation);
    if (mod) {
        ListKey verses = getListKey(vBook, uBook, vChapter, vVerses,
                                    vTranslation);
        verses = TOP;
        return (!verses.Error());
    }
    return FALSE;
}

DECORATION int verseCount(const char * vBook, uBookType uBook,
                    const char * vChapter, const char * vVerses,
                    const char * vTranslation)
{
    SWModule *mod = swordLib->getModule(vTranslation);
    if (mod) {
        ListKey verses = getListKey(vBook, uBook, vChapter, vVerses,
                                    vTranslation);
        int count = 0;
        for (verses = TOP; !verses.Error(); verses++)
        {
            count++;
            if (count > 100)
                break;
        }
        return count;
    }
    return 0;
}
