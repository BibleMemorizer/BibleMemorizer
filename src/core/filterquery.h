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

#ifndef BMEMCOREFILTERQUERY_H
#define BMEMCOREFILTERQUERY_H

#include "core/searchfilter.h"
#include <qstring.h>
#include <stack>

namespace bmemcore
{

/**
@author Jeremy Erickson
*/
class FilterQuery : public SearchFilter
{
public:
    FilterQuery(const QString&);

    virtual ~FilterQuery();

    virtual bool allows(const Verse&);

    bool valid();

	QString sourceString();
	

protected:
    SearchFilter* constructTree(const QString&);
    QString getQuote(const QString& findIn, int * len);
    void clearStack(std::stack<SearchFilter*>& stack, SearchFilter* currToken);
    SearchFilter* mFilter;
	QString mString;
};

}
#endif
