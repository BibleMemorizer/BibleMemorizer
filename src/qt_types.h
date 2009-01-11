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

#ifndef BMEMUIQT_TYPES_H
#define BMEMUIQT_TYPES_H

#include <qglobal.h>

#if QT_VERSION >= 0x040000
#include <Q3MainWindow>
#include <Q3ListBox>
#include <Q3FileDialog>
#include <Q3GroupBox>
#include <Q3WidgetStack>
#include <Q3TextEdit>
#include <Q3Action>
#include <QFocusEvent>
#include <QMenu>
#else
#include <qmainwindow.h>
#include <qlistbox.h>
#include <qfiledialog.h>
#include <qgroupbox.h>
#include <qwidgetstack.h>
#include <qtextedit.h>
#include <qaction.h>
#include <qpopupmenu.h>
#endif

namespace bmemui
{
#if QT_VERSION >= 0x040000
    typedef Q3MainWindow BQMainWindow;
    typedef Q3ListBox BQListBox;
    typedef Q3ListBoxItem BQListBoxItem;
    typedef Q3ListBoxText BQListBoxText;
    typedef Q3FileDialog BQFileDialog;
    typedef Q3GroupBox BQGroupBox;
    typedef Q3TextEdit BQTextEdit;
    typedef QMenu BQPopupMenu;
    const QFlags<QIODevice::OpenModeFlag> BIO_ReadOnly = QIODevice::ReadOnly;
    const QFlags<QIODevice::OpenModeFlag> BIO_WriteOnly = QIODevice::WriteOnly;
    const int B_CaseSensitive = Q3ListBox::CaseSensitive;
    const int B_ExactMatch = Q3ListBox::ExactMatch;
#define BQTextEdit_TypeName "Q3TextEdit"
#else
    typedef QMainWindow BQMainWindow;
    typedef QListBox BQListBox;
    typedef QListBoxItem BQListBoxItem;
    typedef QListBoxText BQListBoxText;
    typedef QFileDialog BQFileDialog;
    typedef QGroupBox BQGroupBox;
    typedef QTextEdit BQTextEdit;
    typedef QPopupMenu BQPopupMenu;
    const int BIO_ReadOnly = IO_ReadOnly;
    const int BIO_WriteOnly = IO_WriteOnly;
    const int B_CaseSensitive = Qt::CaseSensitive;
    const int B_ExactMatch = Qt::ExactMatch;
#define BQTextEdit_TypeName "QTextEdit"
#endif
}

#endif

