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

#include "core/versecollection.h"
#include "core/verse.h"
#include "core/bmemversion.h"
#include <qfile.h>
#include <qstringlist.h>
#include <qdom.h>
#include <QTextStream>
#include <algorithm>
#include <list>

namespace bmemcore
{

VerseCollection::VerseCollection()
    : mChanged(false)
{}

VerseCollection::VerseCollection(const QString& fileName, int& status)
{
    status = 1;
    QDomDocument doc;
    QFile file(fileName);
    bool success = doc.setContent(&file);
    QDomNode node = doc.firstChild();
    while (!node.isNull())
    {
        if (node.isElement() && (node.toElement().tagName() ==
                "versecollection"))
        {
            //We have the correct type of file.
            status = 0;
            QDomElement rootElement = node.toElement();
            QStringList versionInfo = QStringList::split(".",
                            rootElement.attribute("version", "1.0"));
            //Check for legacy category mode, with semicolon-separated values.
            bool legacyCats = false;
            if ((versionInfo[0].toInt() == 1) && (versionInfo[1].toInt() < 3))
            {
                legacyCats = true;
            }
            if ((versionInfo[0].toInt() > 1) || (versionInfo[1].toInt() > 3))
            {
                //Future version of format spec, warn user.
                status = 2;
            }
            QDomNode subNode = rootElement.firstChild();
            while (!subNode.isNull())
            {
                if (subNode.isElement())
                {
                    QDomElement elemSubNode = subNode.toElement();
                    if (elemSubNode.tagName() == "verse")
                        mVerses.push_back(new Verse(this, elemSubNode,
                                    versionInfo));
                    //Next line put second even though it appears first in the
                    //file because it occurs only once.
                    else if (elemSubNode.tagName() == "usercategories")
                    {
                      if (!legacyCats)
                      {
                        QDomNode subSubNode = elemSubNode.firstChild();
                        while (!subSubNode.isNull())
                        {
                            if (subSubNode.isElement())
                            {
                                QDomElement elemSubSubNode = 
                                        subSubNode.toElement();
                                if (elemSubSubNode.tagName() == "category")
                                {
                                    QString catName = "";
                                    QDomNode textNode =
                                        elemSubSubNode.firstChild();
                                    while (!textNode.isNull())
                                    {
                                        if (textNode.isText())
                                        {
                                            catName = textNode.toText().data();
                                            break;
                                        }
                                        textNode = textNode.nextSibling();
                                    }
                                    addCategory(catName);
                                }
                            }
                            subSubNode = subSubNode.nextSibling();
                        }
                      }
                      else //legacyCats mode for old files
                      {
                            QDomNode textNode = elemSubNode.firstChild();
                            QString categoriesJoined = "";
                            while (!textNode.isNull())
                            {
                                if (textNode.isText())
                                {
                                    categoriesJoined = textNode.toText().data();
                                    break;
                                }
                                textNode = textNode.nextSibling();
                            }
                            mCategories = QStringList::split(";",
                                    categoriesJoined);
                      }
                    }
                }
                subNode = subNode.nextSibling();
            }
        }
        node = node.nextSibling();
    }
    for (std::list<Verse*>::iterator it = mVerses.begin(); it!= mVerses.end();
                it++){
        connect(*it, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)),
                this, SLOT(verseChanged(const Verse&, Verse::ChangeType)));
    }
    mChanged = false;
}

Verse* VerseCollection::addNewVerse()
{
    Verse *toReturn = new Verse(this);
    connect(toReturn, SIGNAL(verseChanged(const Verse&, Verse::ChangeType)),
            this, SLOT(verseChanged(const Verse&, Verse::ChangeType)));
    mVerses.push_back(toReturn);
    mChanged = true;
    emit verseAdded(toReturn);
    return toReturn;
}

bool VerseCollection::containsVerse(Verse *verse) const
{
    std::list<Verse*>::const_iterator it = std::find(mVerses.begin(), 
            mVerses.end(), verse);
    return (it != mVerses.end());
}

bool VerseCollection::removeVerse(Verse  *verse)
{
    std::list<Verse*>::iterator it = std::find(mVerses.begin(), mVerses.end(),
            verse);
    if (it != mVerses.end())
    {
        mVerses.erase(it);
        verse->deleteLater();
        mChanged = true;
        return true;
    }
    else
        return false;
}

void VerseCollection::setCategories(QStringList categories)
{
    mCategories = categories;
    mChanged = true;
}

const QStringList& VerseCollection::getCategories() const
{
    return mCategories;
}

void VerseCollection::addCategory(const QString& category)
{
    if (!containsCategory(category))
    {
        mCategories.push_back(category);
        emit categoryAdded(category);
        mChanged = true;
    }
}

bool VerseCollection::containsCategory(const QString& category) const
{
    QStringList::const_iterator it = mCategories.find(category);
    return (it != mCategories.end());
}

bool VerseCollection::removeCategory(const QString& category, bool changeVerses)
{
    QStringList::iterator cIt = mCategories.find(category);
    if (cIt != mCategories.end())
    {
        mCategories.erase(cIt);
        if (changeVerses)
        {
            std::list<Verse*>::iterator vIt = mVerses.begin();
            while (vIt != mVerses.end())
            {
                (*vIt)->removeCategory(category);
                vIt++;
            }
        }
        mChanged = true;
        emit categoryRemoved(category);
        return true;
    }
    else
        return false;
}

bool VerseCollection::renameCategory(const QString& oldCategory,
        const QString& newCategory, bool changeVerses)
{
    QStringList::iterator cIt = mCategories.find(oldCategory);
    if (cIt != mCategories.end())
    {
        cIt = mCategories.erase(cIt);
        mCategories.insert(cIt, newCategory);
        if (changeVerses)
        {
            std::list<Verse*>::iterator vIt = mVerses.begin();
            while (vIt != mVerses.end())
            {
                (*vIt)->replaceCategory(oldCategory, newCategory);
                vIt++;
            }
        }
        mChanged = true;
        emit categoryRenamed(oldCategory, newCategory);
        return true;
    }
    else
        return false;
}

void VerseCollection::saveFile(const QString &fileName)
{
    QDomDocument doc;
    doc.appendChild(doc.createProcessingInstruction("xml",
            "version=\"1.0\" encoding=\"utf-8\""));
    QDomElement root = doc.createElement("versecollection");
    root.setAttribute("version", "1.3");
    root.setAttribute("generator", "BibleMemorizer " BMEM_VERSION_STR
            " (Qt)");
    doc.appendChild(root);
    QDomElement categories = doc.createElement("usercategories");
    root.appendChild(categories);
    for (QStringList::const_iterator it = mCategories.begin(); it !=
            mCategories.end(); ++it)
    {
        QDomElement cat = doc.createElement("category");
        QString catName = *it;
        QDomText catText = doc.createTextNode(catName);
        categories.appendChild(cat);
        cat.appendChild(catText);
    }
    std::list<Verse*>::const_iterator it = mVerses.begin();
    while (it != mVerses.end())
    {
        root.appendChild((*it)->generateElement(&doc));
        it++;
    }
    QFile file(fileName);
    file.open(bmemui::BIO_WriteOnly);
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
    mChanged = false;
}

void VerseCollection::verseChanged(const Verse&, Verse::ChangeType)
{
    mChanged = true;
}

const std::list<Verse*>& VerseCollection::getVerses() const
{
    return mVerses;
}

bool VerseCollection::hasChanged() const
{
    return mChanged;
}

VerseCollection::~VerseCollection()
{
    //Deleting children is automatically handled by Qt, so we don't have to.
}

}

