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

#include "core/verse.h"
#include <qdom.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qregexp.h>
#include <qobject.h>
#include "core/bibleplugin.h"
#include "core/settings.h"
#include <list>

namespace bmemcore
{

Verse::Verse(VerseCollection* parent)
:QObject((QObject*)parent)
{
    mUBook.testament = 0;
    mUBook.book = 0;
}

Verse::Verse(VerseCollection* parent, const QDomElement &element, const
        QStringList& versionInfo)
:QObject((QObject*)parent)
{
    mUBook.testament = 0;
    mUBook.book = 0;
    parseElement(element, versionInfo);
}


void Verse::parseElement(const QDomElement &element, const QStringList&
        versionInfo)
{
    QDomNode node = element.firstChild();
    bool legacyCats = false;
    if ((versionInfo[0].toInt() == 1) && (versionInfo[1].toInt() < 3))
    {
        legacyCats = true;
    }
    while (!node.isNull())
    {
        if (node.isElement())
        {
            QDomElement elemNode = node.toElement();
            if (elemNode.tagName() == "reference")
            {
                QDomNode subNode = elemNode.firstChild();
                while (!subNode.isNull())
                {
                    if (subNode.isElement())
                    {
                        QDomElement elemSubNode = subNode.toElement();
                        if (elemSubNode.tagName() == "book")
                        {
                            setBook(getNodeText(elemSubNode));
                        }
                        else if (elemSubNode.tagName() == "ubook")
                        {
                            uBookType uBook;
                            QDomNode subSubNode = elemSubNode.firstChild();
                            while (!subSubNode.isNull())
                            {
                                if (subSubNode.isElement())
                                {
                                    QDomElement elemSubSubNode =
                                            subSubNode.toElement();
                                    if (elemSubSubNode.tagName() == "testament")
                                    {
                                        QString nodeText =
                                                getNodeText(elemSubSubNode).stripWhiteSpace();
                                        if (nodeText == "nt"){
                                            uBook.testament = NEW_TESTAMENT;
                                        }
                                        else if (nodeText == "ot"){
                                            uBook.testament = OLD_TESTAMENT;
                                        }
                                        else{
                                            uBook.testament = nodeText.toInt();
                                        }
                                    }
                                    else if (elemSubSubNode.tagName() == "book")
                                    {
                                        uBook.book =
                                            getNodeText(elemSubSubNode).toInt();
                                    }
                                }
                                subSubNode = subSubNode.nextSibling();
                            }
                            setUBook(uBook);
                        }
                        else if (elemSubNode.tagName() == "chapter")
                        {
                            setChapter(getNodeText(elemSubNode));
                        }
                        else if (elemSubNode.tagName() == "verses")
                        {
                            setVerses(getNodeText(elemSubNode));
                        }
                    }
                    subNode = subNode.nextSibling();
                }
            }
            else if (elemNode.tagName() == "translation")
            {
                setTranslation(getNodeText(elemNode));
            }
            else if (elemNode.tagName() == "text")
            {
                setText(getNodeText(elemNode));
            }
            else if (elemNode.tagName() == "categories")
            {
              if (!legacyCats)
              {
                QDomNode subNode = elemNode.firstChild();
                while (!subNode.isNull())
                {
                    if (subNode.isElement())
                    {
                        QDomElement elemSubNode = subNode.toElement();
                        if (elemSubNode.tagName() == "category")
                        {
                            QString catName = getNodeText(elemSubNode);
                            addCategory(catName);
                        }
                    }
                    subNode = subNode.nextSibling();
                }
              }
              else //legacyCats mode
              {
                    setCategories(QStringList::split(";",
                            getNodeText(elemNode)));
              }
            }
        }
        node = node.nextSibling();
    }
}


QDomElement Verse::generateElement(QDomDocument* doc) const
{
    bool createdDoc = (doc == 0);
    if (createdDoc)
        doc = new QDomDocument();
    QDomElement verseElement = doc->createElement("verse");
    QDomElement referenceElement = doc->createElement("reference");
    verseElement.appendChild(referenceElement);
    QDomElement bookElement = doc->createElement("book");
    QDomText bookText = doc->createTextNode(getBook());
    referenceElement.appendChild(bookElement);
    bookElement.appendChild(bookText);
    QDomElement uBookElement = doc->createElement("ubook");
    referenceElement.appendChild(uBookElement);
    QDomElement uBookTestamentElement = doc->createElement("testament");
    QString uBookTestamentString;
    if (getUBook().testament == NEW_TESTAMENT){
        uBookTestamentString = "nt";
    }
    else if (getUBook().testament == OLD_TESTAMENT){
        uBookTestamentString = "ot";
    }
    else{
        uBookTestamentString = QString::number(getUBook().testament);
    }
    QDomText uBookTestamentText = doc->createTextNode(uBookTestamentString);
    uBookElement.appendChild(uBookTestamentElement);
    uBookTestamentElement.appendChild(uBookTestamentText);
    QDomElement uBookBookElement = doc->createElement("book");
    QDomText uBookBookText = 
            doc->createTextNode(QString::number(getUBook().book));
    uBookElement.appendChild(uBookBookElement);
    uBookBookElement.appendChild(uBookBookText);
    QDomElement chapterElement = doc->createElement("chapter");
    QDomText chapterText = doc->createTextNode(getChapter());
    referenceElement.appendChild(chapterElement);
    chapterElement.appendChild(chapterText);
    QDomElement versesElement = doc->createElement("verses");
    QDomText versesText = doc->createTextNode(getVerses());
    referenceElement.appendChild(versesElement);
    versesElement.appendChild(versesText);
    QDomElement translationElement = doc->createElement("translation");
    QDomText translationText = doc->createTextNode(getTranslation());
    verseElement.appendChild(translationElement);
    translationElement.appendChild(translationText);
    QDomElement textElement = doc->createElement("text");
    QDomText textText = doc->createTextNode(getText());
    verseElement.appendChild(textElement);
    textElement.appendChild(textText);
    QDomElement categoriesElement = doc->createElement("categories");
    for (QStringList::const_iterator it = mCategories.begin();
            it != mCategories.end(); ++it)
    {
        QString catName = *it;
        QDomElement catElement = doc->createElement("category");
        QDomText catText = doc->createTextNode(catName);
        categoriesElement.appendChild(catElement);
        catElement.appendChild(catText);
    }
    verseElement.appendChild(categoriesElement);
    if (createdDoc)
        delete doc;
    return verseElement;
}

void Verse::setBookInfo(const QString& book, BiblePlugin& plugin)
{
    setBook(book);
    setUBook(plugin.uBook(book));
}

void Verse::setBookInfo(const uBookType uBook, BiblePlugin& plugin)
{
    setUBook(uBook);
    setBook(plugin.book(uBook));
}

void Verse::setBookInfo(const QString& book, const uBookType uBook)
{
    setBook(book);
    setUBook(uBook);
}

void Verse::setBook(const QString &newBook)
{
    mBook = newBook;
    notifyWatchers(CHANGE_BOOK);
}

QString Verse::getBook() const
{
    return mBook;
}

void Verse::setUBook(const uBookType uBook)
{
    mUBook = uBook;
    notifyWatchers(CHANGE_UBOOK);
}

uBookType Verse::getUBook() const
{
    return mUBook;
}

void Verse::setChapter(const QString &newChapter)
{
    mChapter = newChapter;
    notifyWatchers(CHANGE_CHAPTER);
}

QString Verse::getChapter() const
{
    return mChapter;
}

void Verse::setVerses(const QString &newVerses)
{
    mVerses = newVerses;
    notifyWatchers(CHANGE_VERSES);
}

QString Verse::getVerses() const
{
    return mVerses;
}

void Verse::setTranslation(const QString &newTranslation)
{
    mTranslation = newTranslation;
    notifyWatchers(CHANGE_TRANSLATION);
}

QString Verse::getTranslation() const
{
    return mTranslation;
}

void Verse::setText(const QString &newText)
{
    mText = newText;
    notifyWatchers(CHANGE_TEXT);
}

QString Verse::getText() const
{
    return mText;
}

void Verse::setCategories(const QStringList &newCategories)
{
    mCategories = newCategories;
    notifyWatchers(CHANGE_CATEGORIES);
}

QStringList Verse::getCategories() const
{
    return mCategories;
}

void Verse::addCategory(const QString& newCategory)
{
    QStringList::iterator it = mCategories.find(newCategory);
    if (it == mCategories.end())
    {
        mCategories.push_back(newCategory);
        notifyWatchers(CHANGE_CATEGORIES);
    }
}

bool Verse::removeCategory(const QString& oldCategory)
{
    QStringList::iterator it = mCategories.find(oldCategory);
    if (it != mCategories.end())
    {
        mCategories.erase(it);
        notifyWatchers(CHANGE_CATEGORIES);
        return true;
    }
    else
        return false;
}

bool Verse::replaceCategory(const QString& oldCategory, const QString&
        newCategory)
{
    QStringList::iterator it = mCategories.find(oldCategory);
    if (it != mCategories.end())
    {
        it = mCategories.erase(it);
        mCategories.insert(it, newCategory);
        return true;
    }
    else
        return false;
}

bool Verse::containsCategory(const QString& category) const
{
    QStringList::const_iterator it = mCategories.find(category);
    return (it != mCategories.end());
}

QString Verse::getReference() const
{
    QString toReturn;
    toReturn += getBook();
    toReturn += " ";
    if (!getChapter().stripWhiteSpace().isEmpty())
    {
        toReturn += getChapter();
        if (!getVerses().stripWhiteSpace().isEmpty())
        {
            toReturn += ":";
        }
    }
    toReturn += getVerses();
    toReturn = toReturn.simplifyWhiteSpace();
    if (toReturn.isEmpty())
    {
        toReturn = tr("<No Reference>");
    }
    return toReturn;
}

void Verse::setText(BiblePlugin& plugin)
{
    setText(plugin.getVerse(mBook, mUBook, mChapter, mVerses, mTranslation));
}

bool Verse::isTextAvailable(BiblePlugin& plugin)
{
    return plugin.verseAvailable(mBook, mUBook, mChapter, mVerses,
            mTranslation);
}

int Verse::verseCount(BiblePlugin& plugin)
{
    return plugin.verseCount(mBook, mUBook, mChapter, mVerses, mTranslation);
}

void Verse::notifyKilled()
{
    notifyWatchers(CHANGE_DESTROYED);
}

void Verse::notifyWatchers(ChangeType whatChanged)
{
    emit verseChanged(*this, whatChanged);
}

QString Verse::getNodeText(QDomElement &element)
{
    QDomNode node = element.firstChild();
    QString toReturn = "";
    while (!node.isNull())
    {
        if (node.isText())
        {
            toReturn = node.toText().data();
            break;
        }
        node = node.nextSibling();
    }
    return toReturn;
}

QStringList Verse::tokenize(const QString& text)
{
    return QStringList::split(QRegExp("\\s+"), text, false);
}

bool Verse::checkText(const QString& text)
{
    std::list<CorrectionWord> corrections = correctText(text);
    bool errors = false;
    bool countPuncErrors = Settings::getCountPuncErrors();
    bool countCapErrors = Settings::getCountCapErrors();
    bool countSpellingErrors = Settings::getCountSpellingErrors();
    for (std::list<CorrectionWord>::const_iterator it = corrections.begin();
            it != corrections.end(); ++it)
    {
        errors = errors ||
                 (it->getPuncError() && countPuncErrors) ||
                 (it->getCapError() && countCapErrors) ||
                 (it->getSpellingError() && countSpellingErrors) ||
                  it->getOtherError();
    }
    return !errors;
}

std::list<CorrectionWord> Verse::classifyDifferences(
        QStringList::const_iterator first_start,
        QStringList::const_iterator first_stop,
        QStringList::const_iterator second_start,
        QStringList::const_iterator second_stop,
        int combinedSize)
{
    QStringList::const_iterator it1 = first_start;
    QStringList::const_iterator it2 = second_start;
    std::list<CorrectionWord> toReturn;
    while (it1 != first_stop && it2 != second_stop)
    {
        CorrectionWord nextComp(*it1, *it2);
        if (!nextComp.getOtherError())
        {
            toReturn.push_back(nextComp);
            ++it1;
            ++it2;
        }
        else
        {
            QStringList::const_iterator it1find = it1;
            QStringList::const_iterator it2find = it2;
            int twoInOne = 0;
            int oneInTwo = 0;
            while (it1find != first_stop &&
                    CorrectionWord(*it2, *it1find).getOtherError())
            {
                ++twoInOne;
                ++it1find;
            }
            if (it1find == first_stop)
            {
                twoInOne = combinedSize;
            }
            while (it2find != second_stop &&
                    CorrectionWord(*it2find, *it2).getOtherError())
            {
                ++oneInTwo;
                ++it2find;
            }
            if (it2find == second_stop)
            {
                oneInTwo = combinedSize;
            }
            //*it1 value should be inserted
            if (twoInOne < oneInTwo)
            {
                toReturn.push_back(CorrectionWord(*it1, ""));
                ++it1;
            }
            else
            {
                toReturn.push_back(CorrectionWord("", *it2));
                ++it2;
            }
        }
    }
    while (it1 != first_stop)
    {
        toReturn.push_back(CorrectionWord(*it1, ""));
        ++it1;
    }
    while (it2 != second_stop)
    {
        toReturn.push_back(CorrectionWord("", *it2));
        ++it2;
    }
    return toReturn;
}

std::list<CorrectionWord> Verse::correctText(const QString& text)
{
    QStringList first = tokenize(mText);
    QStringList second = tokenize(text);
    //We are iterating through both at once, adding words as either
    //correct, missing, or wrong (i.e. only in user text)
    QStringList::const_iterator it1 = first.begin();
    QStringList::const_iterator it2 = second.begin();
    QStringList::const_iterator err1_start = it1;
    QStringList::const_iterator err1_stop = it1;
    QStringList::const_iterator err2_start = it2;
    QStringList::const_iterator err2_stop = it2;
    //Used as a search distance when not found.
    //This is guaranteed to be a bigger distance than for any actual search.
    int combinedSize = first.size() + second.size();
    std::list<CorrectionWord> toReturn;
    while (it1 != first.end() && it2 != second.end())
    {
        //When both are the same, we have a correct word.
        if (*it1 == *it2)
        {
            std::list<CorrectionWord> corrections =
                    classifyDifferences(err1_start, err1_stop, err2_start,
                                        err2_stop, combinedSize);
            for (std::list<CorrectionWord>::const_iterator it =
                    corrections.begin(); it != corrections.end(); ++it)
            {
                toReturn.push_back(*it);
            }
            toReturn.push_back(CorrectionWord(*it1, *it2));
            ++it1;
            ++it2;
            err1_start = it1;
            err1_stop = it1;
            err2_start = it2;
            err2_stop = it2;
        }
        else
        {
            //Here we are determining how far the correct word is in the
            //user's text, and vice versa.
            QStringList::const_iterator it1find = it1;
            QStringList::const_iterator it2find = it2;
            int twoInOne = 0;
            int oneInTwo = 0;
            //Iterate until we find it.
            while (it1find != first.end() && *it1find != *it2)
            {
                ++twoInOne;
                ++it1find;
            }
            //If we didn't find it, use the proper search distance.
            if (it1find == first.end())
                twoInOne = combinedSize;
            //Same procedure as above.
            while (it2find != second.end() && *it2find != *it1)
            {
                ++oneInTwo;
                ++it2find;
            }
            if (it2find == second.end())
                oneInTwo = combinedSize;
            //Word from user's text is closer, approach it in correct text.
            if (twoInOne < oneInTwo)
            {
                ++it1;
                err1_stop = it1;
            }
            //Word from correct text is closer, approach it in user's text.
            //Alternatively, they might have been at equal distances, or
            //neither found.  In this case, defaulting to this shows crossing
            //out before the correct word.
            else
            {
                ++it2;
                err2_stop = it2;
            }
        }
    }
    std::list<CorrectionWord> corrections = classifyDifferences(err1_start,
                first.end(), err2_start, second.end(), combinedSize);
    for (std::list<CorrectionWord>::iterator it = corrections.begin();
            it != corrections.end(); ++it)
    {
        toReturn.push_back(*it);
    }
    return toReturn;
}

int Verse::compareTo(const Verse& other) const
{
    if (getUBook().testament != other.getUBook().testament){
        return getUBook().testament - other.getUBook().testament;
    }
    else if (getUBook().book != other.getUBook().book){
        return getUBook().book - other.getUBook().book;
    }
    else if (getChapter() != other.getChapter()){
        return getChapter().toInt() - other.getChapter().toInt();
    }
    else{
        QString myVerses = getVerses().stripWhiteSpace();
        for (int i = 0; i < myVerses.length(); i++){
            if (!myVerses.ref(i).isDigit()){
                myVerses.remove(i, myVerses.length());
                break;
            }
        }
        QString otherVerses = other.getVerses().stripWhiteSpace();
        for (int i = 0; i < otherVerses.length(); i++){
            if (!otherVerses.ref(i).isDigit()){
                otherVerses.remove(i, otherVerses.length());
            }
        }
        return myVerses.toInt() - otherVerses.toInt();
    }
}

Verse::~Verse()
{
    notifyWatchers(CHANGE_DESTROYED);
}

}

