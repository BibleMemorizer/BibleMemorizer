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

#include "correctionword.h"

namespace bmemcore

{

CorrectionWord::CorrectionWord(const QString& correct, const QString& given)
{
    if (correct == given)
    {
        mWord = correct;
        for (int i = 0; i < correct.length(); i++)
        {
            mLetterStates.push_back(LETTER_STATE_CORRECT);
        }
        mPuncError = false;
        mCapError = false;
        mSpellingError = false;
        mOtherError = false;
        mWordState = WORD_STATE_CORRECT;
        return;
    }
    else if (correct == "")
    {
        mWord = given;
        mPuncError = false;
        mOtherError = false;
        for (int i = 0; i < given.length(); i++)
        {
            mLetterStates.push_back(LETTER_STATE_WRONG);
            if (given[i].isPunct())
            {
                mPuncError = true;
            }
            else
            {
                mOtherError = true;
            }
        }
        mCapError = false;
        mSpellingError = false;
        mWordState = WORD_STATE_WRONG;
        return;
    }
    else if (given == "")
    {
        mWord = correct;
        mPuncError = false;
        mOtherError = false;
        for (int i = 0; i < correct.length(); i++)
        {
            mLetterStates.push_back(LETTER_STATE_MISSING);
            if (correct[i].isPunct())
            {
                mPuncError = true;
            }
            else
            {
                mOtherError = true;
            }
        }
        mCapError = false;
        mSpellingError = false;
        mWordState = WORD_STATE_MISSING;
        return;
    }
    bool otherError = false;
    int numCommon = 0;
    //We are iterating through both at once, adding characters as either
    //correct, missing, or wrong (i.e. only in user text)
    int it1 = 0;
    int it2 = 0;
    //Used as a search distance when not found.
    //This is guaranteed to be a bigger distance than for any actual search.
    int combinedSize = correct.length() + given.length();
    int maxSize = correct.length();
    if (given.length() > maxSize)
        maxSize = given.length();
    while (it1 < correct.length() && it2 < given.length())
    {
        //When both are the same, we have a correct character.
        if (correct[it1] == given[it2])
        {
            mWord.append(correct[it1]);
            mLetterStates.push_back(LETTER_STATE_CORRECT);
            ++it1;
            ++it2;
            numCommon++;
        }
        else if (correct[it1].toLower() == given[it2].toLower())
        {
            mWord.append(given[it2]);
            mLetterStates.push_back(LETTER_STATE_WRONG);
            mWord.append(correct[it1]);
            mLetterStates.push_back(LETTER_STATE_MISSING);
            mCapError = true;
            ++it1;
            ++it2;
            //We count capitalization errors as common letters for purposes
            //of detecting spelling errors.
            numCommon++;
        }
        else
        {
            //Here we are determining how far the correct character is in the
            //user's text, and vice versa.
            int it1Find = it1;
            int it2Find = it2;
            int twoInOne = 0;
            int oneInTwo = 0;
            //Iterate until we find it.
            while (it1Find < correct.length() && correct[it1Find].toLower() !=
                    given[it2].toLower())
            {
                ++twoInOne;
                ++it1Find;
            }
            //If we didn't find it, use the proper search distance.
            if (it1Find == correct.length())
                twoInOne = combinedSize;
            //Same procedure as above.
            while (it2Find < given.length() && given[it2Find].toLower() !=
                        correct[it1].toLower())
            {
                ++oneInTwo;
                ++it2Find;
            }
            if (it2Find == given.length())
                oneInTwo = combinedSize;
            //Letter from user's text is closer, approach it in correct text.
            if (twoInOne < oneInTwo)
            {
                mWord.append(correct[it1]);
                mLetterStates.push_back(LETTER_STATE_MISSING);
                if (correct[it1].isPunct())
                {
                    mPuncError = true;
                }
                else
                {
                    otherError = true;
                }
                ++it1;
            }
            //Letter from correct text is closer, approach it in user's text.
            //Alternatively, they might have been at equal distances, or
            //neither found.  In this case, defaulting to this shows crossing
            //out before the correct letter.
            else
            {
                mWord.append(given[it2]);
                mLetterStates.push_back(LETTER_STATE_WRONG);
                if (given[it2].isPunct())
                {
                    mPuncError = true;
                }
                else
                {
                    otherError = true;
                }
                ++it2;
            }
        }
    }
    //Pick up anything we missed.
    while (it1 < correct.length())
    {
        mWord.append(correct[it1]);
        mLetterStates.push_back(LETTER_STATE_MISSING);
        if (correct[it1].isPunct())
        {
            mPuncError = true;
        }
        else
        {
            otherError = true;
        }
        ++it1;
    }
    while (it2 < given.length())
    {
        mWord.append(given[it2]);
        mLetterStates.push_back(LETTER_STATE_WRONG);
        if (given[it2].isPunct())
        {
            mPuncError = true;
        }
        else
        {
            otherError = true;
        }
        ++it2;
    }
    if (otherError){
        if ((double)numCommon/(double)maxSize > .6)
        {
            mSpellingError = true;
            mOtherError = false;
        }
        else
        {
            mSpellingError = false;
            mOtherError = true;
        }
    }
    else
    {
        mSpellingError = false;
        mOtherError = false;
    }
    mWordState = WORD_STATE_ERRORS;
}

bool CorrectionWord::getPuncError() const
{
    return mPuncError;
}

bool CorrectionWord::getCapError() const
{
    return mCapError;
}

bool CorrectionWord::getSpellingError() const
{
    return mSpellingError;
}

bool CorrectionWord::getOtherError() const
{
    return mOtherError;
}

const QList<CorrectionWord::LetterState>&
CorrectionWord::getLetterStates() const
{
    return mLetterStates;
}

QString CorrectionWord::getWord() const
{
    return mWord;
}

CorrectionWord::WordState CorrectionWord::getWordState() const
{
    return mWordState;
}

}
