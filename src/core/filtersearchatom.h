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

#ifndef BMEMCOREFILTERSEARCHATOM_H
#define BMEMCOREFILTERSEARCHATOM_H

#include "core/searchfilter.h"
#include <qstring.h>

namespace bmemcore
{

/**
@author Jeremy Erickson
*/
class FilterSearchAtom : public SearchFilter
{
public:

    enum FieldType
    {
        FIELD_REFERENCE,
        FIELD_BOOK,
        FIELD_CHAPTER,
        FIELD_VERSES,
        FIELD_TRANSLATION,
        FIELD_TEXT,
        FIELD_CATEGORIES
    };

    enum SearchType
    {
        SEARCH_CONTAINS,
        SEARCH_NO_CONTAINS,
        SEARCH_MATCHES,
        SEARCH_NO_MATCHES,
        SEARCH_STARTSWITH,
        SEARCH_NO_STARTSWITH,
        SEARCH_ENDSWITH,
        SEARCH_NO_ENDSWITH
    };

    FilterSearchAtom(FieldType field, SearchType search, const QString& text);
    FilterSearchAtom();

    virtual ~FilterSearchAtom();

    virtual bool allows(const Verse& verse);

    virtual bool valid();

    void setField(FieldType newField);
    FieldType getField();
    void setSearch(SearchType newSearch);
    SearchType getSearch();
    void setText(const QString& newText);
    QString getText();

private:
    FieldType mField;
    SearchType mSearch;
    QString mText;
};

}
#endif
