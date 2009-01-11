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

#include "core/filteror.h"
#include "core/filterinvalid.h"

namespace bmemcore{

FilterOr::FilterOr(SearchFilter* firstFilter)
:mFilter1(firstFilter), m2Set(false)
{
    mFilter2 = new FilterInvalid();
}

bool FilterOr::full(){
    return m2Set;
}

void FilterOr::setFilter2(SearchFilter* filter){
    delete mFilter2;
    mFilter2 = filter;
    m2Set = true;
}

bool FilterOr::allows(const Verse& verse){
    return mFilter1->allows(verse) || mFilter2->allows(verse);
}

bool FilterOr::valid()
{
    return mFilter1->valid() && mFilter2->valid();
}

FilterOr::~FilterOr(){
    delete mFilter1;
    delete mFilter2;
}

}
