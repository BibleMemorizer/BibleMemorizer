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

#ifndef BMEMUIQUIZREFACT_H
#define BMEMUIQUIZREFACT_H

#include "ui_QuizRefUI.h"
#include "core/verse.h"
#include <vector>
using namespace bmemcore;

class QWidget;

namespace bmemui
{

class QuizRefAct: public QWidget, public Ui_QuizRefUI
{
    Q_OBJECT
public:
    QuizRefAct(std::vector<Verse*> verses, QWidget* parent = 0,
            const char *name = 0);
public slots:
    virtual void mFinishedButton_clicked();
    virtual void mNextButton_clicked();
    virtual void verseChanged(const Verse&, Verse::ChangeType);
signals:
    void finished(int numCorrect, int numTotal);
    void nextVerse(int currVerse, int total);
protected:
    void initVerse();
    bmemcore::Verse* mVerse;
    std::vector<Verse*> mVerses;
    int mCurrentVerse;
    int mNumCorrect;
    int mNumAttempts;
};

}

#endif
