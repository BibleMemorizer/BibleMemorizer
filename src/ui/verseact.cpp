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

#include "ui/verseact.h"
#include "core/verse.h"
#include "core/bibleplugin.h"
#include "ui/quizact.h"
#include <qcombobox.h>
#include <qstring.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qtabwidget.h>
#include <qstringlist.h>
#include <qmessagebox.h>
using namespace bmemcore;

namespace bmemui
{

VerseAct::VerseAct(Verse *verse, BiblePlugin *plugin,
        const QStringList &categories, PageType whatPage,
        QTabWidget &tabParent, VerseCollection *coll,
        QWidget *parent, const char *name)
:QWidget(parent), mVerse(verse), mTabWidget(tabParent),
        mWatcherDead(false)
{
    setupUi(this);
    Q_UNUSED(name);

    mQuizPage = new QuizAct(mVerse);
    mWidgetStack->addWidget(mQuizPage);
    //mVerse->addWatcher(this);
    connect(mVerse, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)),
            this, SLOT(verseChanged(const Verse&, Verse::ChangeType)));
    connect((QObject*)coll, SIGNAL(categoryAdded(const QString&)), this,
            SLOT(addCategory(const QString&)));
    connect((QObject*)coll, SIGNAL(categoryRemoved(const QString&)), this,
            SLOT(removeCategory(const QString&)));
    connect((QObject*)coll, SIGNAL(categoryRenamed(const QString&,
            const QString&)),
            this, SLOT(renameCategory(const QString&, const QString&)));
    mModeComboBox->insertItem(MODE_VIEW, tr("View"));
    mModeComboBox->insertItem(MODE_EDIT, tr("Edit"));
    mModeComboBox->insertItem(MODE_QUIZ, tr("Quiz"));
    setPlugin(plugin);
    //using mStopRecursion to avoid triggering notifications
    //for other classes.
    mStopRecursion = true;
    mCategoryListBox->clear();
    mCategoryListBox->addItems(categories);
    choosePage(whatPage);
    updateReference();
    mChapterComboBox->setCurrentText(mVerse->getChapter());
    mVersesEdit->setText(mVerse->getVerses());
    mTranslationLabel->setText(mVerse->getTranslation());
    mTextViewEdit->setText(mVerse->getText());
    mTextEdit->setText(mVerse->getText());
    mCategoryListBox->clearSelection();
    QStringList vCategories = mVerse->getCategories();
    QStringList::const_iterator it = vCategories.begin();
    while (it != vCategories.end())
    {
        QList<QListWidgetItem*> items = mCategoryListBox->findItems(*it, Qt::MatchCaseSensitive | Qt::MatchExactly);
        foreach(QListWidgetItem* item, items)
        {
            item->setSelected(true);
        }
        it++;
    }
    mCategoriesLabel->setText(vCategories.join("; "));
    mStopRecursion = false;
}


void VerseAct::mModeComboBox_activated(int index)
{
    //Qt has to pass us an "int" rather than a "PageType",
    //but we know the value will be in the "PageType" enum.
    choosePage(static_cast<PageType>(index));
}

//We only handle our verse, so no need for "Verse&" argument.
void VerseAct::verseChanged(const Verse&, Verse::ChangeType whatChanged)
{
    //When one of the boxes has its contents changed here, this will
    //cause the corresponding property in the Verse object to change.
    //This will cause the notifier here to be called again, and
    //infinite recursion will result.  Here we stop the cycle from
    //happening.
    if (!mStopRecursion)
    {
        mStopRecursion = true;
        switch (whatChanged)
        {
        case Verse::CHANGE_BOOK:
            updateReference();
            mBookComboBox->setCurrentText(mVerse->getBook());
            mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
            break;
        case Verse::CHANGE_CHAPTER:
            updateReference();
            mChapterComboBox->setCurrentText(mVerse->getChapter());
            mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
            break;
        case Verse::CHANGE_VERSES:
            updateReference();
            mVersesEdit->setText(mVerse->getVerses());
            mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
            break;
        case Verse::CHANGE_TRANSLATION:
            mTranslationLabel->setText(mVerse->getTranslation());
            mTranslationComboBox->setCurrentText(mVerse->getTranslation());
            mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
            break;
        case Verse::CHANGE_TEXT:
            mTextViewEdit->setText(mVerse->getText());
            mTextEdit->setText(mVerse->getText());
            break;
        case Verse::CHANGE_CATEGORIES:
            {
                mCategoryListBox->clearSelection();
                QStringList vCategories;
                vCategories = mVerse->getCategories();
                QStringList::const_iterator it = vCategories.begin();
                while (it != vCategories.end())
                {
                    QList<QListWidgetItem*> items = mCategoryListBox->findItems(*it, Qt::MatchCaseSensitive | Qt::MatchExactly);
                    foreach(QListWidgetItem* item, items)
                    {
                        item->setSelected(true);
                    }
                    it++;
                }
                mCategoriesLabel->setText(vCategories.join("; "));
            }
            break;
        case Verse::CHANGE_DESTROYED:
            closeMe(true);
            break;
        }
        mStopRecursion = false;
    }
}

void VerseAct::addCategory(const QString& newCategory)
{
    mStopRecursion = true;
    mCategoryListBox->addItem(newCategory);
    mCategoryListBox->updateGeometry();
    mStopRecursion = false;
}

void VerseAct::renameCategory(const QString& oldCategory,
        const QString& newCategory)
{
    mStopRecursion = true;
    QList<QListWidgetItem*> items = mCategoryListBox->findItems(oldCategory, Qt::MatchCaseSensitive | Qt::MatchExactly);
    foreach(QListWidgetItem* item, items)
    {
        item->setText(newCategory);
    }
    mCategoryListBox->updateGeometry();
    mStopRecursion = false;
}

void VerseAct::removeCategory(const QString& oldCategory)
{
    mStopRecursion = true;
    QList<QListWidgetItem*> items = mCategoryListBox->findItems(oldCategory, Qt::MatchCaseSensitive | Qt::MatchExactly);
    foreach(QListWidgetItem* item, items)
    {
        mCategoryListBox->removeItemWidget(item);
    }
    mCategoryListBox->updateGeometry();
    mStopRecursion = false;
}

VerseAct::~VerseAct()
{
    if (mVerse != 0)
        disconnect(mVerse, 0, this, 0);
        //mVerse->removeWatcher(this);
    mQuizPage->deleteLater();
}

void VerseAct::mBookComboBox_textChanged(const QString& newBook)
{
    //We don't need to handle changes from the notify function.
    //In addition, doing so might cause recursion for other classes.
    if (!mStopRecursion)
    {
        mStopRecursion = true;
        mVerse->setBookInfo(newBook, *mPlugin);
        updateReference();
        mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
        QString oldText = mChapterComboBox->currentText();
        mChapterComboBox->clear();
        int numBooks = mPlugin->getNumChapters(mVerse->getBook(),
                mVerse->getUBook());
        for (int i = 1; i <= numBooks; i++)
        {
            mChapterComboBox->addItem(QString::number(i));
        }
        mChapterComboBox->setCurrentText(oldText);
        mStopRecursion = false;
    }
}

void VerseAct::mChapterComboBox_textChanged(const QString& newChapter)
{
    //See comment in mBookComboBox_textChanged for a rationale for the
    //mStopRecursion usage.
    if (!mStopRecursion)
    {
        mStopRecursion = true;
        mVerse->setChapter(newChapter);
        updateReference();
        mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
        mStopRecursion = false;
    }
}

void VerseAct::mVersesEdit_textChanged(const QString& newVerses)
{
    //See comment in mBookComboBox_textChanged for a rationale for the
    //mStopRecursion usage.
    if (!mStopRecursion)
    {
        mStopRecursion = true;
        mVerse->setVerses(newVerses);
        updateReference();
        mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
        mStopRecursion = false;
    }
}

void VerseAct::mTranslationComboBox_textChanged(const QString& newTranslation)
{
    //See comment in mBookComboBox_textChanged for a rationale for the
    //mStopRecursion usage.
    if (!mStopRecursion)
    {
        mStopRecursion = true;
        mVerse->setTranslation(newTranslation);
        mTranslationLabel->setText(newTranslation);
        mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
        mStopRecursion = false;
    }
}

void VerseAct::mTextEdit_textChanged()
{
    //See comment in mBookComboBox_textChanged for a rationale for the
    //mStopRecursion usage.
    if (!mStopRecursion)
    {
        mStopRecursion = true;
        mVerse->setText(mTextEdit->toPlainText());
        mTextViewEdit->setText(mVerse->getText());
        mStopRecursion = false;
    }
}

void VerseAct::mCategoriesListBox_selectionChanged()
{
    //See comment in mBookComboBox_textChanged for a rationale for the
    //mStopRecursion usage.
    if (!mStopRecursion)
    {
        mStopRecursion = true;
        QStringList newCategories;
        foreach(QListWidgetItem *item, mCategoryListBox->selectedItems())
        {
            newCategories.append(item->text());
        }
        mVerse->setCategories(newCategories);
        mCategoriesLabel->setText(newCategories.join("; "));
        mStopRecursion = false;
    }
}


void VerseAct::mCloseButton_clicked()
{
    closeMe(false);
}

void VerseAct::choosePage(PageType index)
{
    switch (index)
    {
    case MODE_VIEW:
        mWidgetStack->setCurrentWidget(mViewPage);
        break;
    case MODE_EDIT:
        mWidgetStack->setCurrentWidget(mEditPage);
        break;
    case MODE_QUIZ:
        mWidgetStack->setCurrentWidget(mQuizPage);
        break;
    }
    if (mModeComboBox->currentIndex() != index)
    {
        mModeComboBox->setCurrentIndex(index);
    }
}

void VerseAct::closeMe(bool verseDying)
{
    mTabWidget.removeTab(mTabWidget.indexOf(this));
    if (!verseDying)
        disconnect(mVerse, 0, this, 0);
    mVerse = 0;
    deleteLater();
}

void VerseAct::updateReference()
{
    mTabWidget.setTabText(mTabWidget.indexOf(this), mVerse->getReference());
    mReferenceLabel->setText(mVerse->getReference());
}

void VerseAct::setPlugin(BiblePlugin* plugin)
{
    mStopRecursion = true;
    mPlugin = plugin;
    mTranslationComboBox->clear();
    mTranslationComboBox->addItems(mPlugin->getTranslations());
    mTranslationComboBox->setCurrentText(mVerse->getTranslation());
    mBookComboBox->clear();
    mBookComboBox->addItems(mPlugin->getBooks(""));
    mBookComboBox->setCurrentText(mVerse->getBook());
    if (mPlugin->verseLoaderAvailable()){
        mVerseLoaderButton->setText(tr("Get From %1").arg(
                mPlugin->getMeta().getShortName()));
        mVerseLoaderButton->setEnabled(mVerse->isTextAvailable(*mPlugin));
        mVerseLoaderButton->show();
    }
    else
        mVerseLoaderButton->hide();
    mStopRecursion = false;
}

void VerseAct::mVerseLoaderButton_clicked()
{
    int vc = mVerse->verseCount(*mPlugin);
    if (vc > 100)
    {
        int choice = QMessageBox::warning(this, tr("BibleMemorizer"),
               tr("The verse range you entered contains more than 100 verses\n"
                  "according to the plugin.  This may take a long time. If\n"
                  "this is not what you intended, check your verse range for\n"
                  "extra characters (letters, etc.)\n\n"
                  "Do you wish to proceed importing this text?"),
                    QMessageBox::Yes | QMessageBox::Default,
                    QMessageBox::No | QMessageBox::Escape);
        if (choice == QMessageBox::No)
        {
            return;
        }
    }
    mVerse->setText(*mPlugin);
}


}

