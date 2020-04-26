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

#include "ui_QuizCreateUI.h"
#include "core/filterlist.h"
#include "ui/quizmultiact.h"
#include <vector>
#include <qstring.h>
namespace bmemcore
{
    class VerseCollection;
    class Verse;
}

namespace bmemui
{
class VerseSelectAct;

class QuizCreateAct: public QDialog, public Ui_QuizCreateUI {
Q_OBJECT
public:
    QuizCreateAct(bmemcore::VerseCollection* coll,
            QuizMultiAct::QuizMode mMode,
            QWidget *parent = 0, const char *name = 0);
    std::vector<bmemcore::Verse*> getVerses();
    QuizMultiAct::QuizMode getMode();
    virtual ~QuizCreateAct();
public slots:
    virtual void mAddButton_clicked();
    virtual void mRemoveButton_clicked();
    virtual void mSaveCategoryButton_clicked();
    virtual void mOKButton_clicked();
    virtual void verseActivated(bmemcore::Verse*);
protected:
    bmemcore::FilterList mFilter;
    VerseSelectAct* mLeft;
    VerseSelectAct* mRight;
    bmemcore::VerseCollection* mVerses;
    QuizMultiAct::QuizMode mMode;
};

}
