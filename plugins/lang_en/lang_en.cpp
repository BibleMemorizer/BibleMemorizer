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

#ifdef Q_WS_WIN
    #define DECORATION extern "C" __declspec(dllexport)
#else
    #define DECORATION extern "C"
#endif

#define TRUE 1
#define FALSE 0

#include "../bmemplugin.h"

#include <cstring>
using namespace std;

#ifdef Q_WS_WIN
int main()
{}
#endif

char * books[] = {
    "Genesis",
    "Exodus",
    "Leviticus",
    "Numbers",
    "Deuteronomy",
    "Joshua",
    "Judges",
    "Ruth",
    "1 Samuel",
    "2 Samuel",
    "1 Kings",
    "2 Kings",
    "1 Chronicles",
    "2 Chronicles",
    "Ezra",
    "Nehemiah",
    "Esther",
    "Job",
    "Psalms",
    "Proverbs",
    "Ecclesiastes",
    "Song Of Solomon",
    "Isaiah",
    "Jeremiah",
    "Lamentations",
    "Ezekiel",
    "Daniel",
    "Hosea",
    "Joel",
    "Amos",
    "Obadiah",
    "Jonah",
    "Micah",
    "Nahum",
    "Habakkuk",
    "Zephaniah",
    "Haggai",
    "Zechariah",
    "Malachi",
    "Matthew",
    "Mark",
    "Luke",
    "John",
    "Acts",
    "Romans",
    "1 Corinthians",
    "2 Corinthians",
    "Galatians",
    "Ephesians",
    "Philippians",
    "Colossians",
    "1 Thessalonians",
    "2 Thessalonians",
    "1 Timothy",
    "2 Timothy",
    "Titus",
    "Philemon",
    "Hebrews",
    "James",
    "1 Peter",
    "2 Peter",
    "1 John",
    "2 John",
    "3 John",
    "Jude",
    "Revelation"
};

char * translations[] = {
    "NIV",
    "ESV",
    "NET",
    "KJV",
    "NASB",
    "NKJV",
    "TNIV",
    "NLT",
    "Amplified",
    "WEB"
};

uBookType uBooks[] = {
    {OLD_TESTAMENT, 1},
    {OLD_TESTAMENT, 2},
    {OLD_TESTAMENT, 3},
    {OLD_TESTAMENT, 4},
    {OLD_TESTAMENT, 5},
    {OLD_TESTAMENT, 6},
    {OLD_TESTAMENT, 7},
    {OLD_TESTAMENT, 8},
    {OLD_TESTAMENT, 9},
    {OLD_TESTAMENT, 10},
    {OLD_TESTAMENT, 11},
    {OLD_TESTAMENT, 12},
    {OLD_TESTAMENT, 13},
    {OLD_TESTAMENT, 14},
    {OLD_TESTAMENT, 15},
    {OLD_TESTAMENT, 16},
    {OLD_TESTAMENT, 17},
    {OLD_TESTAMENT, 18},
    {OLD_TESTAMENT, 19},
    {OLD_TESTAMENT, 20},
    {OLD_TESTAMENT, 21},
    {OLD_TESTAMENT, 22},
    {OLD_TESTAMENT, 23},
    {OLD_TESTAMENT, 24},
    {OLD_TESTAMENT, 25},
    {OLD_TESTAMENT, 26},
    {OLD_TESTAMENT, 27},
    {OLD_TESTAMENT, 28},
    {OLD_TESTAMENT, 29},
    {OLD_TESTAMENT, 30},
    {OLD_TESTAMENT, 31},
    {OLD_TESTAMENT, 32},
    {OLD_TESTAMENT, 33},
    {OLD_TESTAMENT, 34},
    {OLD_TESTAMENT, 35},
    {OLD_TESTAMENT, 36},
    {OLD_TESTAMENT, 37},
    {OLD_TESTAMENT, 38},
    {OLD_TESTAMENT, 39},
    {NEW_TESTAMENT, 1},
    {NEW_TESTAMENT, 2},
    {NEW_TESTAMENT, 3},
    {NEW_TESTAMENT, 4},
    {NEW_TESTAMENT, 5},
    {NEW_TESTAMENT, 6},
    {NEW_TESTAMENT, 7},
    {NEW_TESTAMENT, 8},
    {NEW_TESTAMENT, 9},
    {NEW_TESTAMENT, 10},
    {NEW_TESTAMENT, 11},
    {NEW_TESTAMENT, 12},
    {NEW_TESTAMENT, 13},
    {NEW_TESTAMENT, 14},
    {NEW_TESTAMENT, 15},
    {NEW_TESTAMENT, 16},
    {NEW_TESTAMENT, 17},
    {NEW_TESTAMENT, 18},
    {NEW_TESTAMENT, 19},
    {NEW_TESTAMENT, 20},
    {NEW_TESTAMENT, 21},
    {NEW_TESTAMENT, 22},
    {NEW_TESTAMENT, 23},
    {NEW_TESTAMENT, 24},
    {NEW_TESTAMENT, 25},
    {NEW_TESTAMENT, 26},
    {NEW_TESTAMENT, 27}
};

int lengths[] = {
    50,
    40,
    27,
    36,
    34,
    24,
    21,
    4,
    31,
    24,
    22,
    25,
    29,
    36,
    10,
    13,
    10,
    42,
    150,
    31,
    12,
    8,
    66,
    52,
    5,
    48,
    12,
    14,
    3,
    9,
    1,
    4,
    7,
    3,
    3,
    3,
    2,
    14,
    4,
    28,
    16,
    24,
    21,
    28,
    16,
    16,
    13,
    6,
    6,
    4,
    4,
    5,
    3,
    6,
    4,
    3,
    1,
    13,
    5,
    5,
    3,
    5,
    1,
    1,
    1,
    22
};

DECORATION void initialize()
{
}

DECORATION void finalize()
{
}

//Can accept a particular translation, in case a module has
//Different book availabilities on a per-translation basis.
DECORATION const char * const * getBooks(int * numBooks, const char *)
{
    *numBooks = 66;
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
    for (int i = 0; i < 66; i++)
    {
        if ((uBooks[i].testament == uBook.testament) && (uBooks[i].book == uBook.book))
            return books[i];
    }
    return "";
}

DECORATION const uBookType uBook(const char * book)
{
    for (int i = 0; i < 66; i++)
    {
        if (!strcmp(book, books[i]))
            return uBooks[i];
    }
    uBookType toReturn = {0, 0};
    return toReturn;
}

//This plugin does have a module list.
DECORATION int translationsAvailable()
{
    return TRUE;
}

DECORATION const char * const * getTranslations(int * numTrans)
{
    *numTrans = 10;
    return translations;
}

DECORATION int getNumChapters(const char * book, uBookType uBook)
{
    for (int i = 0; i < 66; i++)
    {
        if (!strcmp(book, books[i]))
            return lengths[i];
        if ((uBook.testament == uBooks[i].testament) && (uBook.book ==
                uBooks[i].book))
            return lengths[i];
    }
    return 0;
}

//Sword includes support for importing verses.
DECORATION int verseLoaderAvailable()
{
    return FALSE;
}

DECORATION const char * getVerse(const char * vBook, uBookType uBook,
                                const char * vChapter, const char * vVerses,
                                const char * vTranslation)
{
    return "";
}

DECORATION int verseAvailable(const char * vBook, uBookType uBook,
                    const char * vChapter, const char * vVerses,
                    const char * vTranslation)
{
    return FALSE;
}

DECORATION int verseCount(const char * vBook, uBookType uBook,
                    const char * vChapter, const char * vVerses,
                    const char * vTranslation)
{
    return 0;
}
