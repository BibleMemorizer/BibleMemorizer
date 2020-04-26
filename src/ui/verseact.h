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

#ifndef BMEMUIVERSEACT_H
#define BMEMUIVERSEACT_H
class QTabWidget;
#include "ui_VerseUI.h"
#include <qstringlist.h>
#include "core/verse.h"
namespace bmemcore
{
//class Verse;
class BiblePlugin;
}

namespace bmemui
{
using namespace bmemcore;

class QuizAct;

class VerseAct: public Ui_VerseUI
{
    Q_OBJECT
public:
    enum PageType {
        MODE_VIEW,
        MODE_EDIT,
        MODE_QUIZ
    };
    VerseAct(bmemcore::Verse *verse, bmemcore::BiblePlugin *plugin,
            const QStringList &categories, PageType whatPage,
            QTabWidget &tabParent, bmemcore::VerseCollection *coll,
            QWidget *parent = 0, const char *name = 0);
    void setPlugin(bmemcore::BiblePlugin *plugin);
    void choosePage(PageType);
    ~VerseAct();
public slots:
    virtual void verseChanged(const Verse&, Verse::ChangeType);
    virtual void mModeComboBox_activated(int); //was (int index)
    virtual void mBookComboBox_textChanged(const QString&);
    virtual void mCategoriesListBox_selectionChanged();
    virtual void mTextEdit_textChanged();
    virtual void mTranslationComboBox_textChanged(const QString&);
    virtual void mVersesEdit_textChanged(const QString&);
    virtual void mCloseButton_clicked();
    virtual void mChapterComboBox_textChanged(const QString&);
    virtual void mVerseLoaderButton_clicked();
    virtual void addCategory(const QString&);
    virtual void renameCategory(const QString&, const QString&);
    virtual void removeCategory(const QString&);
protected:
    bmemcore::Verse *mVerse;
    bmemcore::BiblePlugin *mPlugin;
    QTabWidget &mTabWidget;
    inline void closeMe(bool verseDying = false);
private:
    bool mStopRecursion;
    bool mWatcherDead;
    QuizAct* mQuizPage;
    inline void updateReference();
};

}

#endif
