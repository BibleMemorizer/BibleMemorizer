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

#include "ui_PreferencesUI.h"
#include <vector>
#include <qstring.h>
namespace bmemcore
{
    class BiblePluginMeta;
}

namespace bmemui
{

class PreferencesAct: public QDialog, public Ui_PreferencesUI {
Q_OBJECT
public:
    PreferencesAct(QWidget *parent = 0);
    static bmemcore::BiblePluginMeta getMetaStatic(bool &success);
    bmemcore::BiblePluginMeta getMeta();
    void selectPluginTab();
    void hideCancel();
    virtual ~PreferencesAct();
public slots:
    virtual void mDirButton_clicked();
    virtual void mOKButton_clicked();
    virtual void mInfoButton_clicked();
    virtual void mPluginListBox_selected(int);
private:
    static std::vector<bmemcore::BiblePluginMeta> getPluginList(const 
            QString& path);
    QString mDirectory;
    std::vector<bmemcore::BiblePluginMeta> mFiles;
    inline bool populateList();
    bool populateList(const QString& path);
};

}
