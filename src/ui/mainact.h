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

#include "ui_MainUI.h"
#include "core/versecollection.h"
#include "ui/verseact.h"
#include "ui/verseselectact.h"
#include "ui/quizmultiact.h"
#include <QEvent>
#include <QStringList>
#include <QMap>

namespace bmemcore
{
class Verse;
class SearchFilter;
class FilterQuery;
class FilterSearchQuery;
class BiblePlugin;
}
class QStringList;

namespace bmemui
{
class MainAct: public QMainWindow, public Ui_MainUI
{
    Q_OBJECT
public:
    MainAct(QWidget *parent = 0);
    virtual bool eventFilter(QObject* watched, QEvent* e);
    virtual ~MainAct();
public slots:
    virtual void fileExitAction_activated();
    virtual void mAddButton_clicked();
    virtual void helpAboutAction_activated();
    virtual void mVerseSelection_selectionChanged(bool);
    virtual void mOpenButton_clicked();
    virtual void fileNewAction_activated();
    virtual void mRemoveButton_clicked();
    virtual void mCategoryListBox_selectionChanged();
    virtual void fileSaveAsAction_activated();
    virtual void fileSaveAction_activated();
    virtual void fileOpenAction_activated();
    virtual void mRemoveCategoryButton_clicked();
    virtual void mRenameCategoryButton_clicked();
    virtual void mAddCategoryButton_clicked();
    virtual void preferencesSettingsAction_activated();
    virtual void mVerseSelection_verseActivated(Verse*);
    virtual void editMenu_aboutToShow();
    virtual void categoryAdded(const QString& category);
    virtual void categoryRemoved(const QString& category);
    virtual void categoryRenamed(const QString& oldName,
            const QString& newName);
    virtual void quizTextAction_activated();
    virtual void quizReferenceAction_activated();
protected:
    QWidget* currWidget;
    bmemcore::VerseCollection *mVerses;
    VerseSelectAct* mVerseSelection;
    QMap<bmemcore::Verse*,VerseAct*> mVersePaneMap;
    bmemcore::BiblePlugin *mPlugin;
    QString mCurrentFileName;
    QString mFileFilters;
    void setCurrentFileName(const QString&);
    bool promptSave();
    void closeEvent(QCloseEvent*);
    bool save();
    bool saveAs();
    void openFile(const QString&);
    void doQuiz(QuizMultiAct::QuizMode mode);
    inline void openVerse(bmemcore::Verse *verse, VerseAct::PageType);
private:
    void setSearchQuery(bmemcore::FilterQuery*);
    void changeVerseCollection(bmemcore::VerseCollection*);
    void initVerseCollection();
};

}
