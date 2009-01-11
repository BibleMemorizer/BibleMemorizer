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

#ifndef BMEMCOREHELP_H
#define BMEMCOREHELP_H


class QString;

namespace bmemcore
{

/**
This class provides access to the help system.
 
@author Jeremy Erickson
*/


class Help
{
public:
    /** Shows a given page of help.  This function will
      * automatically locate a web browser and/or prompt the user
      * for information if necessary.
      * @param helpPage The page to show.
      * @return Whether the operation was successful.
      */
    static bool showPage(QString helpPage);
    /** Shows the main page of help.  This function will
      * automatically locate a web browser and/or prompt the user
      * for information if necessary.
      * @return Whether the operation was successful.
      */
    inline static bool showIndex();
};

}

#endif
