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

#ifndef BMEMCOREVERSE_H
#define BMEMCOREVERSE_H
class QDomElement;
class QDomDocument;
#include <qstring.h>
#include <qstringlist.h>
#include <qobject.h>
#include "core/ubook.h"
#include "core/correctionword.h"
#include <list>
#include <vector>

namespace bmemcore
{
class BiblePlugin;
class VerseCollection;


/**
@author Jeremy Erickson
*/
class Verse : public QObject
{
    Q_OBJECT
public:
    enum ChangeType {
        CHANGE_BOOK,
        CHANGE_UBOOK,
        CHANGE_CHAPTER,
        CHANGE_VERSES,
        CHANGE_TRANSLATION,
        CHANGE_TEXT,
        CHANGE_CATEGORIES,
        CHANGE_DESTROYED
    };
    /** Creates an empty verse.
      */
    Verse(VerseCollection* parent);
    Verse(VerseCollection* parent, const QDomElement&, const QStringList&
            versionInfo);
    void parseElement(const QDomElement&, const QStringList& versionInfo);
    QDomElement generateElement(QDomDocument* doc = 0) const;
    void setBookInfo(const QString&, BiblePlugin&);
    void setBookInfo(const uBookType, BiblePlugin&);
    void setBookInfo(const QString&, const uBookType);
    void setBook(const QString&);
    QString getBook() const;
    void setUBook(const uBookType);
    uBookType getUBook() const;
    void setChapter(const QString&);
    QString getChapter() const;
    void setVerses(const QString&);
    QString getVerses() const;
    void setTranslation(const QString&);
    QString getTranslation() const;
    void setText(const QString&);
    QString getText() const;
    void setCategories(const QStringList&);
    QStringList getCategories() const;
    void addCategory(const QString& newCategory);
    bool removeCategory(const QString& oldCategory);
    bool replaceCategory(const QString& oldCategory,
            const QString& newCategory);
    bool containsCategory(const QString&) const;
    QString getReference() const;
    void notifyKilled();
    void setText(BiblePlugin&);
    bool isTextAvailable(BiblePlugin&);
    int verseCount(BiblePlugin&);
    bool checkText(const QString& text);
    std::list<CorrectionWord> correctText(const QString& text);
    int compareTo(const Verse& other) const;

    ~Verse();
signals:
    void verseChanged(const Verse& verse, Verse::ChangeType change);
protected:
    QString mBook;
    uBookType mUBook;
    QString mChapter;
    QString mVerses;
    QString mTranslation;
    QString mText;
    QStringList mCategories;
    void notifyWatchers(ChangeType);
    QString getNodeText(QDomElement&);
    QStringList tokenize(const QString& text);
    std::list<CorrectionWord> classifyDifferences(QStringList::const_iterator,
            QStringList::const_iterator, QStringList::const_iterator,
            QStringList::const_iterator, int);
private:
    //Intentionally not implemented - Verse objects are noncopyable
    explicit Verse(const Verse&);
    Verse& operator= (const Verse&);
};

}

#endif
