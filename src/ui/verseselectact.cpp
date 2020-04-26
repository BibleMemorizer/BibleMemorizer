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

#include "ui/verseselectact.h"
#include "core/verse.h"
#include "core/versecollection.h"
#include "core/filterquery.h"
#include <list>
#include <map>
#include <utility>
#include <iostream>
#include <qinputdialog.h>
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qstring.h>
#include <qstringlist.h>
using namespace bmemcore;

namespace bmemui{

VerseSelectAct::VerseSelectAct(QWidget* parent, bool multiSelect,
    VerseCollection* coll, SearchFilter* requiredFilter)
:VerseSelectUI(parent, "Verse Select"), mVerseCollection(coll),
        mSearchFilter(new FilterQuery("true")),
        mSearchQuery(new FilterQuery("true")),
        mRequiredFilterMode(false)
{
    if (multiSelect){
        mVerseListBox->setSelectionMode(BQListBox::Extended);
    }
    else{
        mVerseListBox->setSelectionMode(BQListBox::Single);
    }
    mVerseListBox->clear();
    if (requiredFilter){
        mRequiredFilterMode = true;
        mSearchButton->hide();
        mFilterComboBox->hide();
        mSearchFilter = requiredFilter;
    }
    changeVerseCollection(coll);
}

std::list<Verse*> VerseSelectAct::getSelectedVerses()
{
    std::list<Verse*> toReturn;
    for (int i = 0; i < mVerseListBox->count(); i++){
        BQListBoxItem* curr = mVerseListBox->item(i);
        if (curr->isSelected()){
            toReturn.push_back(verses[curr]);
        }
    }
    return toReturn;
}

void VerseSelectAct::selectAll()
{
    mVerseListBox->selectAll(true);
}

void VerseSelectAct::selectNone()
{
    mVerseListBox->selectAll(false);
}

Verse* VerseSelectAct::getSelectedVerse()
{
    std::list<Verse*> verses = getSelectedVerses();
    if (verses.empty()){
        return 0;
    }
    else{
        return *verses.begin();
    }
}

void VerseSelectAct::changeVerseCollection(VerseCollection* coll){
    if (mVerseCollection != coll){
        disconnect(mVerseCollection, 0, this, 0);
    }
    items.clear();
    verses.clear();
    mVerseListBox->clear();
    mVerseCollection = coll;
    connect(mVerseCollection, SIGNAL(verseAdded(Verse*)), this,
            SLOT(verseAdded(Verse*)));
    connect(mVerseCollection, SIGNAL(categoryAdded(const QString &)), this,
            SLOT(categoryAdded(const QString&)));
    connect(mVerseCollection, SIGNAL(categoryRemoved(const QString&)), this,
            SLOT(categoryRemoved(const QString&)));
    connect(mVerseCollection, SIGNAL(categoryRenamed(const QString&,
            const QString&)), this, SLOT(categoryRenamed(const QString&,
            const QString&)));
    const std::list<Verse*>& verses = coll->getVerses();
    for (std::list<Verse*>::const_iterator it = verses.begin(); it != 
            verses.end(); it++){
        connect(*it, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)),
                this, SLOT(verseChanged(const Verse&, Verse::ChangeType)));
        items[*it] = 0;
    }
    mFilterComboBox->clear();
    mFilterComboBox->insertItem(tr("All Verses"), 0);
    mFilterComboBox->insertItem(tr("From Search"), 1);
    mFilterComboBox->insertStringList(mVerseCollection->getCategories(), 2);
    recreateList();
}

void VerseSelectAct::verseAdded(Verse* verse){
    connect(verse, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)), this,
            SLOT(verseChanged(const Verse&, Verse::ChangeType)));
    if (mSearchFilter->allows(*verse)){
        insert(verse);
    }
    else{
        items[verse] = 0;
    }
}

void VerseSelectAct::mSearchButton_clicked()
{
    bool ok;
    QString newQuery = QInputDialog::getText(tr("New Query"),
                        tr("Enter a new search query:\n\n"
                        "Examples:\ntext contains \"Jesus Christ\"\n"
                        "(reference contains \"3:16\" or reference contains "
                        "\"3:23\") and translation equals \"NIV\"\n"
                        "in category \"Salvation\"\n"
                        "testament equals \"ot\"\n"
                        "not chapter equals \"3\""),
                        QLineEdit::Normal, mSearchQuery->sourceString(), &ok,
                        this);
    if (ok)
    {
		FilterQuery* theQuery = new FilterQuery(newQuery);
        if (theQuery->valid())
        {
            mSearchQuery = theQuery;
            mFilterComboBox->setCurrentItem(1);
            recreateList();
        }
        else
        {
            QMessageBox::warning(this, "Bad pattern", "The pattern you "
                    "selected is invalid.");
            delete theQuery;
        }
    }
}

void VerseSelectAct::mFilterComboBox_activated(int item){
    if ((item == 1) && (mSearchQuery == 0)){
        mSearchButton_clicked();
    }
    else{
        recreateList();
    }
}

void VerseSelectAct::mVerseListBox_doubleClicked(BQListBoxItem* selected){
    emit verseActivated(verses[selected]);
}

void VerseSelectAct::categoryAdded(const QString& category){
    mFilterComboBox->insertItem(category);
}

void VerseSelectAct::categoryRemoved(const QString& category){
    for (int i = 2; i < mFilterComboBox->count(); i++)
    {
        if (mFilterComboBox->text(i) == category)
        {
            mFilterComboBox->removeItem(i);
            break;
        }
    }
}

void VerseSelectAct::categoryRenamed(const QString& oldName, const QString& newName){
    for (int i = 2; i < mFilterComboBox->count(); i++)
    {
        if (mFilterComboBox->text(i) == oldName)
        {
            mFilterComboBox->changeItem(newName, i);
        }
    }
}

void VerseSelectAct::verseChanged(const Verse& verse, Verse::ChangeType changeType)
{
    Verse* verseRef = const_cast<Verse*>(&verse);
    if (changeType == Verse::CHANGE_DESTROYED){
        disconnect(&verse, 0, this, 0);
        verses.erase(items[verseRef]);
        delete items[verseRef];
        items[verseRef] = 0;
    }
    else{
        bool selected = false;
        if (BQListBoxItem* listItem = items[verseRef]){
            selected = listItem->isSelected();
            verses.erase(listItem);
            items[verseRef] = 0;
            delete listItem;
        }
        if (mSearchFilter->allows(verse)){
            insert(verseRef);
            mVerseListBox->setSelected(items[verseRef], selected);
        }
        else{
            items[verseRef] = 0;
        }
    }
}

void VerseSelectAct::recreateList(){
    if (!mRequiredFilterMode)
    {
        if (mSearchFilter && mSearchFilter != mSearchQuery){
            delete mSearchFilter;
        }
#if QT_VERSION >= 0x040000
        switch (mFilterComboBox->currentIndex()){
#else
        switch (mFilterComboBox->currentItem()){
#endif
            case 0:
                mSearchFilter = new FilterQuery("true");
                break;
            case 1:
                mSearchFilter = mSearchQuery;
                break;
            default:
                QString catName = mFilterComboBox->currentText();
                catName.replace('\\', "\\\\");
                catName.replace('"', "\\\"");
                mSearchFilter = new FilterQuery ("in category \"" + catName +
                                "\"");
        }
    }
    const std::list<Verse*> verses = mVerseCollection->getVerses();
    for (std::list<Verse*>::const_iterator it = verses.begin(); it !=
            verses.end(); ++it){
        if (mSearchFilter->allows(*(*it))){
            if (!items[*it]){
                insert(*it);
            }
        }
        else{
            if (items[*it]){
                delete items[*it];
                items[*it] = 0;
            }
        }
    }
}

void VerseSelectAct::insert(Verse* verse){
    BQListBoxItem* newItem = new BQListBoxText(verse->getReference());
    if (items[verse]){
        verses.erase(items[verse]);
        delete items[verse];
    }
    items[verse] = newItem;
    verses[newItem] = verse;
    int index;
    for (index = 0; index < mVerseListBox->count(); index++){
        Verse* nextVerse = verses[mVerseListBox->item(index)];
        if (nextVerse && verse->compareTo(*nextVerse) < 0){
            break;
        }
    }
    mVerseListBox->insertItem(newItem, index);
}

void VerseSelectAct::mVerseListBox_selectionChanged(){
    emit selectionChanged(!(getSelectedVerses().empty()));
}

void VerseSelectAct::forceRefresh()
{
    recreateList();
}

}
