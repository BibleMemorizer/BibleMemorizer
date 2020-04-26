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

#include <vector>
#include <qstring.h>

namespace bmemcore
{

class CorrectionWord
{
public:
    enum WordState
    {
        WORD_STATE_CORRECT,
        WORD_STATE_MISSING,
        WORD_STATE_WRONG,
        WORD_STATE_ERRORS
    };
    enum LetterState
    {
        LETTER_STATE_CORRECT = WORD_STATE_CORRECT,
        LETTER_STATE_MISSING = WORD_STATE_MISSING,
        LETTER_STATE_WRONG = WORD_STATE_WRONG
    };
    CorrectionWord(const QString& correct, const QString& given);
    bool getPuncError() const;
    bool getCapError() const;
    bool getSpellingError() const;
    bool getOtherError() const;
    const std::vector<LetterState>& getLetterStates() const;
    QString getWord() const;
    WordState getWordState() const;
private:
    QString mWord;
    WordState mWordState;
    std::vector<LetterState> mLetterStates;
    bool mPuncError;
    bool mCapError;
    bool mSpellingError;
    bool mOtherError;
};

}
