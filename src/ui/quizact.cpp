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

#include "ui/quizact.h"
#include "core/verse.h"
#include <qlabel.h>
#include <qpushbutton.h>

using bmemcore::Verse;
using bmemcore::CorrectionWord;

namespace bmemui
{

QuizAct::QuizAct(Verse* verse, QWidget* parent)
:QWidget(parent), mVerse(verse), mNumCorrect(0), mNumAttempts(0)
{
    setupUi(this);

    mNextButton->hide();
    initVerse();
}

QuizAct::QuizAct(std::vector<Verse*> verses, QWidget* parent)
:QWidget(parent), mVerse(verses[0]), mCurrentVerse(0), mNumCorrect(0),
    mNumAttempts(0), mVerses(verses)
{
    setupUi(this);

    mCheckButton->hide();
    mCorrectButton->setText(tr("&Finished"));
    mClearButton->hide();
    mCorrectButton->setDefault(true);
    if (mVerses.size() == 1)
    {
        mNextButton->setText(tr("&Done"));
    }
    mVerseEdit->setFocus();
    initVerse();
}

void QuizAct::initVerse()
{
    connect(mVerse, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)),
            this, SLOT(verseChanged(const Verse&, Verse::ChangeType)));
    mVerseLabel->setText(mVerse->getReference() + " (" +
        mVerse->getTranslation() + ")");
}

void QuizAct::verseChanged(const Verse&, Verse::ChangeType)
{
    mVerseLabel->setText(mVerse->getReference() + " (" +
            mVerse->getTranslation() + ")");
}

void QuizAct::mCheckButton_clicked()
{
    mNumAttempts++;
    if (mVerse->checkText(mVerseEdit->toPlainText()))
    {
        mResultLabel->setText(tr("Correct."));
        mNumCorrect++;
    }
    else
    {
        mResultLabel->setText(tr("Incorrect."));
    }
    mNextButton->setEnabled(true);
}

void QuizAct::mCorrectButton_clicked()
{
    mCheckButton_clicked();
    std::list<bmemcore::CorrectionWord> corrections = mVerse->correctText(
            mVerseEdit->toPlainText());
    std::list<bmemcore::CorrectionWord>::iterator it = corrections.begin();
    QString html;
    while (it != corrections.end())
    {
        switch (it->getWordState())
        {
            case CorrectionWord::WORD_STATE_CORRECT:
                html += it->getWord();
                break;
            case CorrectionWord::WORD_STATE_MISSING:
                html += "<b>";
                html += it->getWord();
                html += "</b>";
                break;
            case CorrectionWord::WORD_STATE_WRONG:
                html += "<font color=red>";
                html += it->getWord();
                html += "</font>";
                break;
            case CorrectionWord::WORD_STATE_ERRORS:
                std::vector<bmemcore::CorrectionWord::LetterState> states =
                        it->getLetterStates();
                QString word = it->getWord();
                for (int i = 0; i < word.length(); i++)
                {
                    switch (states[i])
                    {
                        case CorrectionWord::LETTER_STATE_CORRECT:
                            html += word[i];
                            break;
                        case CorrectionWord::LETTER_STATE_MISSING:
                            html += "<b>";
                            html += word[i];
                            html += "</b>";
                            break;
                        case CorrectionWord::LETTER_STATE_WRONG:
                            html += "<font color=red>";
                            html += word[i];
                            html += "</font>";
                            break;
                    }
                }
                break;
        }
        html += " ";
        ++it;
    }
    mVerseEdit->setText(html);
    mVerseEdit->setReadOnly(true);
    mCorrectButton->setEnabled(false);
    if (mNextButton->isVisible())
    {
        mNextButton->setFocus();
    }
}

void QuizAct::mClearButton_clicked()
{
    mVerseEdit->setText("");
    mVerseEdit->setReadOnly(false);
    mCorrectButton->setEnabled(true);
    mResultLabel->setText("");
}

void QuizAct::mNextButton_clicked()
{
    mCurrentVerse++;
    if (mCurrentVerse >= mVerses.size())
    {
        emit finished(mNumCorrect, mNumAttempts);
    }
    else
    {
        disconnect(mVerse, 0, this, 0);
        mVerse = mVerses[mCurrentVerse];
        initVerse();
        mClearButton_clicked();
        mNextButton->setEnabled(false);
        if (mCurrentVerse == mVerses.size() - 1)
        {
            mNextButton->setText(tr("&Done"));
        }
        emit nextVerse(mCurrentVerse + 1, mVerses.size());
        mVerseEdit->setFocus();
    }
}

}

