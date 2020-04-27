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

#include "core/filtersearchatom.h"
#include "core/verse.h"
#include <qstring.h>

namespace bmemcore
{

FilterSearchAtom::FilterSearchAtom(FieldType field, SearchType search,\
        const QString& text)
:mField(field), mSearch(search), mText(text)
{}

FilterSearchAtom::FilterSearchAtom()
:mField(FIELD_REFERENCE), mSearch(SEARCH_CONTAINS), mText("")
{}


FilterSearchAtom::~FilterSearchAtom()
{}


bool FilterSearchAtom::allows(const Verse& verse)
{
    QString actText;
    switch (mField)
    {
    case FIELD_REFERENCE:
        actText = verse.getReference();
        break;
    case FIELD_BOOK:
        actText = verse.getBook();
        break;
    case FIELD_CHAPTER:
        actText = verse.getChapter();
        break;
    case FIELD_VERSES:
        actText = verse.getVerses();
        break;
    case FIELD_TRANSLATION:
        actText = verse.getTranslation();
        break;
    case FIELD_TEXT:
        actText = verse.getText();
        break;
    case FIELD_CATEGORIES:
        actText = verse.getCategories().join("; ");
        break;
    }
    switch (mSearch)
    {
    case SEARCH_CONTAINS:
        if (actText.contains(mText, Qt::CaseInsensitive))
            return true;
        else
            return false;
        break;
    case SEARCH_NO_CONTAINS:
        if (!actText.contains(mText, Qt::CaseInsensitive))
            return true;
        else
            return false;
        break;
    case SEARCH_MATCHES:
        if (actText.toLower() == mText.toLower())
            return true;
        else
            return false;
    case SEARCH_NO_MATCHES:
        if (actText.toLower() != mText.toLower())
            return true;
        else
            return false;
        break;
    case SEARCH_STARTSWITH:
        if (actText.startsWith(mText, Qt::CaseInsensitive))
            return true;
        else
            return false;
        break;
    case SEARCH_NO_STARTSWITH:
        if (!actText.startsWith(mText, Qt::CaseInsensitive))
            return true;
        else
            return false;
        break;
    case SEARCH_ENDSWITH:
        if (actText.endsWith(mText, Qt::CaseInsensitive))
            return true;
        else
            return false;
        break;
    case SEARCH_NO_ENDSWITH:
        if (!actText.endsWith(mText, Qt::CaseInsensitive))
            return true;
        else
            return false;
        break;
    }
    qFatal("unhandled SearchType");
}

bool FilterSearchAtom::valid()
{
    return true;
}

void FilterSearchAtom::setField(FieldType newField)
{
    mField = newField;
}

FilterSearchAtom::FieldType FilterSearchAtom::getField()
{
    return mField;
}

void FilterSearchAtom::setSearch(SearchType newSearch)
{
    mSearch = newSearch;
}

FilterSearchAtom::SearchType FilterSearchAtom::getSearch()
{
    return mSearch;
}

void FilterSearchAtom::setText(const QString& newText)
{
    mText = newText;
}

QString FilterSearchAtom::getText()
{
    return mText;
}

}

