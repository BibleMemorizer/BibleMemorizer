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

#include "core/filterlist.h"
#include <list>
#include <vector>
#include <algorithm>

namespace bmemcore
{

FilterList::FilterList()
{}

FilterList::~FilterList()
{}

void FilterList::addVerses(std::list<Verse*> verses)
{
    for (std::list<Verse*>::iterator it = verses.begin(); it != verses.end();
            ++it)
    {
        if (!allows(*(*it)))
        {
            mVerses.push_back(*it);
        }
    }
}

void FilterList::removeVerses(std::list<Verse*> verses)
{
    for (std::list<Verse*>::iterator it = verses.begin(); it != verses.end();
            ++it)
    {
        std::vector<Verse*>::iterator findIt = find(mVerses.begin(),
                mVerses.end(), *it);
        if (findIt != mVerses.end())
        {
            mVerses.erase(findIt);
        }
    }
}

std::vector<Verse*> FilterList::getVerses()
{
    return mVerses;
}

void FilterList::verseChanged(const Verse& theVerse, Verse::ChangeType change)
{
    if (change == Verse::CHANGE_DESTROYED)
    {
        Verse* ptr = const_cast<Verse*>(&theVerse);
        std::vector<Verse*>::iterator findIt = find(mVerses.begin(),
                mVerses.end(), ptr);
        if (findIt != mVerses.end())
        {
            mVerses.erase(findIt);
        }
    }
}

bool FilterList::allows(const Verse& theVerse)
{
    Verse* pointer = const_cast<Verse*>(&theVerse);
    std::vector<Verse*>::iterator findIt = find(mVerses.begin(), mVerses.end(),
           pointer);
    return (findIt != mVerses.end());
}

bool FilterList::valid()
{
    return true;
}

}

