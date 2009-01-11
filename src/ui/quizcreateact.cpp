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

#include "ui/quizcreateact.h"
#include "core/filterlist.h"
#include "core/verse.h"
#include "core/versecollection.h"
#include "ui/verseselectact.h"
#include "qlayout.h"
#include "qmessagebox.h"
#include "qinputdialog.h"
#include <list>
#include <vector>
#include <algorithm>
using namespace bmemcore;

namespace bmemui
{

QuizCreateAct::QuizCreateAct(VerseCollection* coll,
        QuizMultiAct::QuizMode mode, QWidget *parent, const char * name)
:QuizCreateUI(parent, name), mLeft(new VerseSelectAct(this, true, coll)),
        mRight(new VerseSelectAct(this, true, coll, &mFilter)), mVerses(coll),
        mMode(mode)
{
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget((QWidget*)mLeft);
    leftLayout->addWidget((QWidget*)mSelectAllButton);
    leftLayout->addWidget((QWidget*)mSelectNoneButton);

    QVBoxLayout *middleButtons = new QVBoxLayout;
    middleButtons->addWidget((QWidget*)mAddButton);
    middleButtons->addWidget((QWidget*)mRemoveButton);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget((QWidget*)mSaveCategoryButton);
    rightLayout->addWidget((QWidget*)mRight);
    rightLayout->addWidget((QWidget*)mRightSelectAllButton);
    rightLayout->addWidget((QWidget*)mRightSelectNoneButton);
    
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addLayout(leftLayout);
    topLayout->addLayout(middleButtons);
    topLayout->addLayout(rightLayout);
    
    QHBoxLayout *bottomButtons = new QHBoxLayout;
    bottomButtons->addStretch(1);
    bottomButtons->addWidget((QWidget*)mOKButton);
    bottomButtons->addWidget((QWidget*)mCancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(11);
    mainLayout->setSpacing(6);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(bottomButtons);

    connect((QObject*)mSelectAllButton, SIGNAL(clicked()), (QObject*)mLeft,
            SLOT(selectAll()));
    connect((QObject*)mSelectNoneButton, SIGNAL(clicked()), (QObject*)mLeft,
            SLOT(selectNone()));
    connect((QObject*)mRightSelectAllButton, SIGNAL(clicked()),
            (QObject*)mRight, SLOT(selectAll()));
    connect((QObject*)mRightSelectNoneButton, SIGNAL(clicked()),
            (QObject*)mRight, SLOT(selectNone()));
}

void QuizCreateAct::mAddButton_clicked()
{
    mFilter.addVerses(mLeft->getSelectedVerses());
    mRight->forceRefresh();
}

void QuizCreateAct::verseActivated(Verse* verse)
{
    std::list<Verse*> verseList;
    verseList.push_back(verse);
    mFilter.addVerses(verseList);
    mRight->forceRefresh();
}

void QuizCreateAct::mRemoveButton_clicked()
{
    mFilter.removeVerses(mRight->getSelectedVerses());
    mRight->forceRefresh();
}

void QuizCreateAct::mSaveCategoryButton_clicked()
{
    bool ok;
    QString newCat = QInputDialog::getText(
            tr("BibleMemorizer"), 
            tr("Enter the name of the new or existing category:"),
            QLineEdit::Normal, QString::null, &ok, this );
    newCat = newCat.simplifyWhiteSpace();
    if (ok && !newCat.isEmpty() && !mVerses->containsCategory(newCat)) 
    {
        mVerses->addCategory(newCat);
    }
    else if (mVerses->containsCategory(newCat))
    {
        int choice = QMessageBox::question(this, tr("BibleMemorizer"), 
                tr("You have already created that category.\n"
                "Would you like to change its verse list\n"
                "to match this one?"), QMessageBox::Yes | QMessageBox::No,
                QMessageBox::Yes);
        if (choice != QMessageBox::Yes){
            ok = false;
        }
    }
    if (ok)
    {
        mVerses->addCategory(newCat);
        //Clear previous contents of category, in case it already existed.
        const std::list<Verse*>& all_verses = mVerses->getVerses();
        for (std::list<Verse*>::const_iterator it = all_verses.begin();
                it != all_verses.end(); ++it)
        {
            (*it)->removeCategory(newCat);
        }
        std::vector<Verse*> verses = mFilter.getVerses();
        for (std::vector<Verse*>::iterator it = verses.begin();
                it != verses.end(); ++it)
        {
            (*it)->addCategory(newCat);
        }
    }
}

QuizMultiAct::QuizMode QuizCreateAct::getMode()
{
    return mMode;
}

std::vector<Verse*> QuizCreateAct::getVerses()
{
    return mFilter.getVerses();
}

void QuizCreateAct::mOKButton_clicked()
{
    if (getVerses().size() == 0){
        reject();
        return;
    }
    std::vector<Verse*> verses = getVerses();
    //Randomly order the verses
    std::random_shuffle(verses.begin(), verses.end());
    hide();
    QuizMultiAct *multiAct = new QuizMultiAct(verses, mMode, this);
    multiAct->exec();
}

QuizCreateAct::~QuizCreateAct()
{
}

}
