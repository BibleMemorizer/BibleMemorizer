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

#pragma once

#include "core/searchfilter.h"
#include <list>
#include <vector>
#include "core/verse.h"

namespace bmemcore
{

/**
@author Jeremy Erickson
*/
class FilterList : public SearchFilter
{
    Q_OBJECT
public:
    FilterList();

    virtual ~FilterList();

    void addVerses(std::list<Verse*> verses);

    void removeVerses(std::list<Verse*> verses);

    std::vector<Verse*> getVerses();

    virtual bool allows(const Verse&);

    virtual bool valid();

public slots:
    virtual void verseChanged(const Verse&, Verse::ChangeType);

protected:
    std::vector<Verse*> mVerses;

};

}
