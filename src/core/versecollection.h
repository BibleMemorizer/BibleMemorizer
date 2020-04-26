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

#include <list>
#include "core/verse.h"
#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>

namespace bmemcore
{
/**
@author Jeremy Erickson
*/
class VerseCollection : public QObject
{
    Q_OBJECT
public:
    VerseCollection();
    VerseCollection(const QString& fileName, int& status);
    Verse* addNewVerse();
    bool containsVerse(Verse *verse) const;
    bool removeVerse(Verse *verse);
    void setCategories(QStringList categories);
    const QStringList& getCategories() const;
    void addCategory(const QString& category);
    bool containsCategory(const QString& category) const;
    bool removeCategory(const QString& category, bool changeVerses);
    bool renameCategory(const QString& oldCategory, const QString& newCategory, bool changeVerses);
    void saveFile(const QString& fileName);
    const std::list<Verse*>& getVerses() const;
    bool hasChanged() const;
    virtual ~VerseCollection();
public slots:
    void verseChanged(const Verse& verse, Verse::ChangeType whatChanged);
signals:
    void verseAdded(Verse* newVerse);
    void categoryAdded(const QString& category);
    void categoryRemoved(const QString& category);
    void categoryRenamed(const QString& oldCategory, const QString& newCategory);
private:
    std::list<Verse*> mVerses;
    QStringList mCategories;
    bool mChanged;
};

}
