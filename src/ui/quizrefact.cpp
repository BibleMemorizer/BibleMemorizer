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
#include <vector>
#include <qstring.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "ui/quizrefact.h"
#include "core/verse.h"

namespace bmemui
{

QuizRefAct::QuizRefAct(std::vector<Verse*> verses, QWidget* parent)
:QWidget(parent), mVerse(verses[0]), mVerses(verses), mCurrentVerse(0),
        mNumCorrect(0), mNumAttempts(0)
{
    setupUi(this);

    initVerse();
    mFinishedButton->setDefault(true);
    if (mVerses.size() == 1)
    {
        mNextButton->setText(tr("&Done"));
    }
    mReferenceText->setFocus();
}

void QuizRefAct::initVerse()
{
    connect(mVerse, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)),
            this, SLOT(verseChanged(const Verse&, Verse::ChangeType)));
    mVerseTextEdit->setText(mVerse->getText());
}

void QuizRefAct::verseChanged(const Verse&, Verse::ChangeType)
{
    mVerseTextEdit->setText(mVerse->getText());
}

void QuizRefAct::mFinishedButton_clicked()
{
    mNumAttempts++;
    QString correct = mVerse->getReference();
    QString given = mReferenceText->text();
    
    //Be forgiving about case
    correct = correct.toLower();
    given = given.toLower();
    
    //Be forgiving about numbered books
    correct.replace(QRegExp("^i "), "1");
    given.replace(QRegExp("^i "), "1");
    correct.replace(QRegExp("^ii "), "2");
    given.replace(QRegExp("^ii "), "2");
    correct.replace(QRegExp("^iii "), "3");
    given.replace(QRegExp("^iii "), "3");
    correct.replace(QRegExp("^1 "), "1");
    given.replace(QRegExp("^1 "), "1");
    correct.replace(QRegExp("^2 "), "2");
    given.replace(QRegExp("^2 "), "2");
    correct.replace(QRegExp("^3 "), "3");
    given.replace(QRegExp("^3 "), "3");

    QStringList correctParts = correct.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
    QStringList givenParts = given.split(QRegExp("\\s+"), Qt::SkipEmptyParts);

    bool match = false;
    if (!correctParts.empty() && !givenParts.empty())
    {
        match = (givenParts[0].startsWith(correctParts[0].left(3)));
        int correctIndex = 1;
        int givenIndex = 1;
        while (correctIndex < correctParts.size() &&
                givenIndex < givenParts.size())
        {
            //Ignore multiple words of book name.
            if (correctParts[correctIndex].contains(QRegExp("^\\D")))
            {
                correctIndex++;
            }
            else if (givenParts[givenIndex].contains(QRegExp("^\\D")))
            {
                givenIndex++;
            }
            else
            {
                match = match && (givenParts[givenIndex] ==
                                      correctParts[correctIndex]);
                correctIndex++;
                givenIndex++;
            }
        }
        match = match && (correctIndex == correctParts.size()) &&
                             (givenIndex == givenParts.size());
    }
    if (match)
    {
        mResultLabel->setText(tr("Correct."));
        mNumCorrect++;
    }
    else
    {
        mResultLabel->setText(tr("Incorrect."));
    }
    mReferenceText->setText(mVerse->getReference());
    mReferenceText->setReadOnly(true);
    mNextButton->setEnabled(true);
    mNextButton->setFocus();
    mFinishedButton->setEnabled(false);
}

void QuizRefAct::mNextButton_clicked()
{
    mCurrentVerse++;
    if (mCurrentVerse >= mVerses.size())
    {
        emit finished(mNumCorrect, mNumAttempts);
    }
    else
    {
        disconnect(mVerse, nullptr, this, nullptr);
        mVerse = mVerses[mCurrentVerse];
        initVerse();
        mReferenceText->setText("");
        mReferenceText->setReadOnly(false);
        mFinishedButton->setEnabled(true);
        mResultLabel->setText("");
        mNextButton->setEnabled(false);
        if (mCurrentVerse == mVerses.size() - 1)
        {
            mNextButton->setText(tr("&Done"));
        }
        emit nextVerse(mCurrentVerse + 1, mVerses.size());
        mReferenceText->setFocus();
    }
}

}
