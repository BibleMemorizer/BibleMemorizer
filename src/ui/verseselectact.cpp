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
:QWidget(parent), mVerseCollection(coll),
        mSearchQuery(new FilterQuery("true")),
        mSearchFilter(new FilterQuery("true")),
        mRequiredFilterMode(false)
{
    setupUi(this);

    if (multiSelect){
        mVerseListBox->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    else{
        mVerseListBox->setSelectionMode(QAbstractItemView::SingleSelection);
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
        QListWidgetItem* curr = mVerseListBox->item(i);
        if (curr->isSelected()){
            toReturn.push_back(verses[curr]);
        }
    }
    return toReturn;
}

void VerseSelectAct::selectAll()
{
    mVerseListBox->selectAll();
}

void VerseSelectAct::selectNone()
{
    mVerseListBox->clearSelection();
}

Verse* VerseSelectAct::getSelectedVerse()
{
    std::list<Verse*> verses = getSelectedVerses();
    if (verses.empty()){
        return nullptr;
    }
    return *verses.begin();
}

void VerseSelectAct::changeVerseCollection(VerseCollection* coll){
    if (mVerseCollection != coll){
        disconnect(mVerseCollection, nullptr, this, nullptr);
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
        items[*it] = nullptr;
    }
    mFilterComboBox->clear();
    mFilterComboBox->insertItem(0, tr("All Verses"));
    mFilterComboBox->insertItem(1, tr("From Search"));
    mFilterComboBox->insertItems(2, mVerseCollection->getCategories());
    recreateList();
}

void VerseSelectAct::verseAdded(Verse* verse){
    connect(verse, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)), this,
            SLOT(verseChanged(const Verse&, Verse::ChangeType)));
    if (mSearchFilter->allows(*verse)){
        insert(verse);
    }
    else{
        items[verse] = nullptr;
    }
}

void VerseSelectAct::mSearchButton_clicked()
{
    bool ok;
    QString newQuery = QInputDialog::getText(this, tr("New Query"),
                        tr("Enter a new search query:\n\n"
                        "Examples:\ntext contains \"Jesus Christ\"\n"
                        "(reference contains \"3:16\" or reference contains "
                        "\"3:23\") and translation equals \"NIV\"\n"
                        "in category \"Salvation\"\n"
                        "testament equals \"ot\"\n"
                        "not chapter equals \"3\""),
                        QLineEdit::Normal, mSearchQuery->sourceString(), &ok);
    if (ok)
    {
		FilterQuery* theQuery = new FilterQuery(newQuery);
        if (theQuery->valid())
        {
            mSearchQuery = theQuery;
            mFilterComboBox->setCurrentIndex(1);
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
    if ((item == 1) && (mSearchQuery == nullptr)){
        mSearchButton_clicked();
    }
    else{
        recreateList();
    }
}

void VerseSelectAct::mVerseListBox_doubleClicked(QListWidgetItem* selected){
    emit verseActivated(verses[selected]);
}

void VerseSelectAct::categoryAdded(const QString& category){
    mFilterComboBox->addItem(category);
}

void VerseSelectAct::categoryRemoved(const QString& category){
    for (int i = 2; i < mFilterComboBox->count(); i++)
    {
        if (mFilterComboBox->itemText(i) == category)
        {
            mFilterComboBox->removeItem(i);
            break;
        }
    }
}

void VerseSelectAct::categoryRenamed(const QString& oldName, const QString& newName){
    for (int i = 2; i < mFilterComboBox->count(); i++)
    {
        if (mFilterComboBox->itemText(i) == oldName)
        {
            mFilterComboBox->setItemText(i, newName);
        }
    }
}

void VerseSelectAct::verseChanged(const Verse& verse, Verse::ChangeType changeType)
{
    Verse* verseRef = const_cast<Verse*>(&verse);
    if (changeType == Verse::CHANGE_DESTROYED){
        disconnect(&verse, nullptr, this, nullptr);
        verses.erase(items[verseRef]);
        delete items[verseRef];
        items[verseRef] = nullptr;
    }
    else{
        bool selected = false;
        if (QListWidgetItem* listItem = items[verseRef]){
            selected = listItem->isSelected();
            verses.erase(listItem);
            items[verseRef] = nullptr;
            delete listItem;
        }
        if (mSearchFilter->allows(verse)){
            insert(verseRef);
            items[verseRef]->setSelected(selected);
        }
        else{
            items[verseRef] = nullptr;
        }
    }
}

void VerseSelectAct::recreateList(){
    if (!mRequiredFilterMode)
    {
        if (mSearchFilter && mSearchFilter != mSearchQuery){
            delete mSearchFilter;
        }
        switch (mFilterComboBox->currentIndex()){
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
                items[*it] = nullptr;
            }
        }
    }
}

void VerseSelectAct::insert(Verse* verse){
    QListWidgetItem* newItem = new QListWidgetItem(verse->getReference());
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
    mVerseListBox->insertItem(index, newItem);
}

void VerseSelectAct::mVerseListBox_selectionChanged(){
    emit selectionChanged(!(getSelectedVerses().empty()));
}

void VerseSelectAct::forceRefresh()
{
    recreateList();
}

}
