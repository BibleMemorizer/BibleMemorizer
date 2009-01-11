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

#ifndef BMEMUIQUIZMULTIACT_H
#define BMEMUIQUIZMULTIACT_H

#include <vector>
#include <qwidget.h>
#include <qdialog.h>
#include <qlabel.h>

namespace bmemcore
{
    class Verse;
}

namespace bmemui
{
class QuizMultiAct : public QDialog
{
    Q_OBJECT
public:
    enum QuizMode
    {
        MODE_TEXT,
        MODE_REFERENCE
    };
    QuizMultiAct(std::vector<bmemcore::Verse*> verses,
            QuizMultiAct::QuizMode mode,
            QWidget *parent = 0, const char* name = 0);
public slots:
    virtual void finished(int numCorrect, int numTotal);
    virtual void nextVerse(int currVerse, int total);
protected:
    QWidget* mQuizWidget;
    QLabel* mStatusLabel;
};

}

#endif
