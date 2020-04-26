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

#include "ui/quizmultiact.h"
#include "ui/quizact.h"
#include "ui/quizrefact.h"
#include "core/verse.h"
#include <qdialog.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <vector>
using namespace bmemcore;

namespace bmemui
{

QuizMultiAct::QuizMultiAct(std::vector<Verse*> verses, QuizMode mode, QWidget*
    parent, const char* name)
:QDialog(parent), mStatusLabel(new QLabel(this))
{
    if (mode == QuizMultiAct::MODE_TEXT)
    {
        mQuizWidget = new QuizAct(verses, this);
        setWindowTitle(tr("Text Quiz - BibleMemorizer"));
    }
    else if (mode == QuizMultiAct::MODE_REFERENCE)
    {
        mQuizWidget = new QuizRefAct(verses, this);
        setWindowTitle(tr("Reference Quiz - BibleMemorizer"));
    }
    mQuizWidget->setMinimumSize(400,250);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mStatusLabel);
    mainLayout->addWidget(mQuizWidget);
    connect(mQuizWidget, SIGNAL(finished(int, int)), this,
            SLOT(finished(int, int)));
    connect(mQuizWidget, SIGNAL(nextVerse(int, int)), this,
            SLOT(nextVerse(int, int)));
    nextVerse(1, verses.size());
}

void QuizMultiAct::nextVerse(int currVerse, int total)
{
    mStatusLabel->setText(tr("Question %1 of %2").arg(currVerse).arg(total));
}

void QuizMultiAct::finished(int numCorrect, int numTotal)
{
    float result = (float)numCorrect/(float)numTotal * (float)100.0;
    QMessageBox::information(this, tr("Quiz Finished"),
            tr("Quiz finished.\n"
               "%1/%2 questions correct (%3%)").arg(numCorrect).arg(numTotal).arg(result));
    accept();
}

}
