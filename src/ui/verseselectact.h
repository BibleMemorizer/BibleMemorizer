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

#ifndef BMEMUIVERSESELECTACT_H
#define BMEMUIVERSESELECTACT_H

#include "qt_types.h"
#include "VerseSelectUI.h"
#include "core/verse.h"
#include <list>
#include <map>

namespace bmemcore{
    class VerseCollection;
    class SearchFilter;
    class FilterQuery;
}

namespace bmemui
{
using namespace bmemcore;
class VerseSelectAct : public VerseSelectUI {
Q_OBJECT
public:
    VerseSelectAct(QWidget* parent, bool multiSelect, VerseCollection* coll,
            SearchFilter* requiredFilter = 0);
    std::list<Verse*> getSelectedVerses();
    Verse* getSelectedVerse();
    void changeVerseCollection(VerseCollection* coll);
public slots:
    void mSearchButton_clicked();
    void mFilterComboBox_activated(int item);
    void mVerseListBox_doubleClicked(BQListBoxItem*);
    void mVerseListBox_selectionChanged();
    void selectAll();
    void selectNone();
    void forceRefresh();
protected slots:
    void verseAdded(Verse* verse);
    void categoryAdded(const QString& category);
    void categoryRemoved(const QString& category);
    void categoryRenamed(const QString& oldName, const QString& newName);
    void verseChanged(const Verse&, Verse::ChangeType);
signals:
    void verseActivated(Verse* verse);
    void selectionChanged(bool verseSelected);
protected:
    VerseCollection *mVerseCollection;
    std::map<Verse*, BQListBoxItem*> items;
    std::map<BQListBoxItem*, Verse*> verses;
    FilterQuery* mSearchQuery;
    SearchFilter* mSearchFilter;
    bool mRequiredFilterMode;
    void recreateList();
    void insert(Verse* verse);
};

}

#endif

