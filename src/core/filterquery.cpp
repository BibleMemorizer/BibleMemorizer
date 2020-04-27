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

#include "core/filterquery.h"
#include "core/filterinvalid.h"
#include "core/filterallowall.h"
#include "core/filternot.h"
#include "core/filterand.h"
#include "core/filteror.h"
#include "core/filtersearchatom.h"
#include "core/filtercategory.h"
#include "core/filtertestament.h"
#include "core/verse.h"
#include <qstring.h>
#include <stack>
using namespace std;

namespace bmemcore
{

FilterQuery::FilterQuery(const QString& newQuery)
{
    mFilter = constructTree(newQuery);
    mString = newQuery;
}

FilterQuery::~FilterQuery()
{
	delete mFilter;
}

QString FilterQuery::sourceString(){
    return mString;
}

bool FilterQuery::allows(const Verse& theVerse)
{
	return mFilter->allows(theVerse);
}

bool FilterQuery::valid()
{
    return mFilter->valid();
}

SearchFilter* FilterQuery::constructTree(const QString& expression){
    std::stack<SearchFilter*> stack;
    QString currExpr = expression.trimmed();
    QString currExprLower = currExpr.toLower();
    while (currExpr.length() > 0){
        SearchFilter* currToken = nullptr;
        int newIndex = 0;
        if (currExpr.startsWith("(")){
            //Find closing parentheses, evaluate with recursion.
            int parenDepth = 1;
            bool quoteMode = false;
            bool escapeMode = false;
            int i;
            for (i = 1; i < currExpr.length(); i++){
                if (!quoteMode){
                    if (currExpr[i] == '('){
                        parenDepth++;
                    }
                    else if (currExpr[i] == ')'){
                        parenDepth--;
                    }
                }
                if (!escapeMode && currExpr[i] == '\"'){
                    quoteMode = !quoteMode;
                }
                if (quoteMode && currExpr[i] == '\\'){
                    escapeMode = !escapeMode;
                }
                else if (quoteMode && currExpr[i] != '\\'){
                    escapeMode = false;
                }
                if (parenDepth == 0){
                    break;
                }
            }
            if (parenDepth != 0){
                //Unbalanced parentheses in file - cleanup and report invalid
                //expression.
                clearStack(stack, currToken);
                return new FilterInvalid();
            }
            currToken = constructTree(currExpr.mid(1, i - 1));
            newIndex = i + 1;
        }
        else if (currExprLower.startsWith("not ")){
            currToken = new FilterNot();
            newIndex = 4;
        }
        else if (currExprLower.startsWith("or ")){
            if (stack.empty()){
                return new FilterInvalid();
            }

            currToken = new FilterOr(stack.top());
            stack.pop();
            newIndex = 3;
        }
        else if (currExprLower.startsWith("and ")){
            if (stack.empty()){
                return new FilterInvalid();
            }

            currToken = new FilterAnd(stack.top());
            stack.pop();
            newIndex = 4;
        }
        else if (currExprLower.startsWith("true ")){
            currToken = new FilterAllowAll();
            newIndex = 5;
        }
        else if (currExprLower == "true"){
            currToken = new FilterAllowAll();
            newIndex = 4;
        }
        else if (currExprLower.startsWith("in category ")){
            QString desiredText = getQuote(currExpr.mid(12), &newIndex);
            if (newIndex == -1){
                clearStack(stack, currToken);
                return new FilterInvalid();
            }

            newIndex += 12;
            currToken = new FilterCategory(desiredText);
        }
        else if (currExprLower.startsWith("testament equals ") ||
                 currExprLower.startsWith("testament matches ")){
            int len = 17;
            if (currExprLower.startsWith("testament matches ")){
                len = 18;
            }
            QString desiredText = getQuote(currExprLower.mid(len), &newIndex);
            if (newIndex == -1){
                clearStack(stack, currToken);
                return new FilterInvalid();
            }

            newIndex += len;
            currToken = new FilterTestament(desiredText);
        }
        else{
            FilterSearchAtom* ourAtom = new FilterSearchAtom();
            if (currExprLower.startsWith("reference ")){
                ourAtom->setField(FilterSearchAtom::FIELD_REFERENCE);
                newIndex = 10;
            }
            else if (currExprLower.startsWith("book ")){
                ourAtom->setField(FilterSearchAtom::FIELD_BOOK);
                newIndex = 5;
            }
            else if (currExprLower.startsWith("chapter ")){
                ourAtom->setField(FilterSearchAtom::FIELD_CHAPTER);
                newIndex = 8;
            }
            else if (currExprLower.startsWith("verse ")){
                ourAtom->setField(FilterSearchAtom::FIELD_VERSES);
                newIndex = 6;
            }
            else if (currExprLower.startsWith("translation ")){
                ourAtom->setField(FilterSearchAtom::FIELD_TRANSLATION);
                newIndex = 12;
            }
            else if (currExprLower.startsWith("text ")){
                ourAtom->setField(FilterSearchAtom::FIELD_TEXT);
                newIndex = 5;
            }
            else if (currExprLower.startsWith("categories ")){
                ourAtom->setField(FilterSearchAtom::FIELD_CATEGORIES);
                newIndex = 11;
            }
            else{
                clearStack(stack, currToken);
                delete ourAtom;
                return new FilterInvalid();
            }
            currExprLower = currExprLower.mid(newIndex).trimmed();
            if (currExprLower.startsWith("contains ")){
                ourAtom->setSearch(FilterSearchAtom::SEARCH_CONTAINS);
                newIndex += 9;
            }
            else if (currExprLower.startsWith("matches ")){
                ourAtom->setSearch(FilterSearchAtom::SEARCH_MATCHES);
                newIndex += 8;
            }
            else if (currExprLower.startsWith("equals ")){
                ourAtom->setSearch(FilterSearchAtom::SEARCH_MATCHES);
                newIndex += 7;
            }
            else if (currExprLower.startsWith("startswith ")){
                ourAtom->setSearch(FilterSearchAtom::SEARCH_STARTSWITH);
                newIndex += 11;
            }
            else if (currExprLower.startsWith("starts with ")){
                ourAtom->setSearch(FilterSearchAtom::SEARCH_STARTSWITH);
                newIndex += 12;
            }
            else if (currExprLower.endsWith("endswith ")){
                ourAtom->setSearch(FilterSearchAtom::SEARCH_ENDSWITH);
                newIndex += 9;
            }
            else if (currExprLower.endsWith("ends with ")){
                ourAtom->setSearch(FilterSearchAtom::SEARCH_ENDSWITH);
                newIndex += 10;
            }
            else{
                clearStack(stack, currToken);
                return new FilterInvalid();
            }
            currExpr = currExpr.mid(newIndex).trimmed();
            QString desiredText = getQuote(currExpr, &newIndex);
            if (newIndex == -1){
                clearStack(stack, currToken);
                return new FilterInvalid();
            }
            ourAtom->setText(desiredText);
            currToken = ourAtom;
        }
        if (!stack.empty()){
            if (FilterNot* notFilter = dynamic_cast<FilterNot*>(stack.top())){
                if (!notFilter->full()){
                    notFilter->setFilter(currToken);
                }
            }
            else if (FilterAnd* andFilter = dynamic_cast<FilterAnd*>(stack.top())){
                if (!andFilter->full()){
                    andFilter->setFilter2(currToken);
                }
            }
            else if (FilterOr* orFilter = dynamic_cast<FilterOr*>(stack.top())){
                if (!orFilter->full()){
                    orFilter->setFilter2(currToken);
                }
            }
            else{
                stack.push(currToken);
            }
        }
        else{
            stack.push(currToken);
        }
        currExpr = currExpr.mid(newIndex).trimmed();
        currExprLower = currExpr.toLower();
    }
    if (stack.empty()){
        return new FilterInvalid();
    }

    SearchFilter *toReturn = stack.top();
    stack.pop();
    if (!stack.empty()){
        clearStack(stack, nullptr);
        delete toReturn;
        return new FilterInvalid();
    }
    return toReturn;
}

void FilterQuery::clearStack(std::stack<SearchFilter*>& stack, SearchFilter* currToken){
    bool deletedToken = false;
    while (!stack.empty()){
        if (stack.top() == currToken){
            deletedToken = true;
        }
        delete stack.top();
        stack.pop();
    }
    if (currToken && !deletedToken){
        delete currToken;
    }
}

QString FilterQuery::getQuote(const QString& findIn, int * len){
    QString desiredText;
    if (findIn.startsWith("\"")){
        bool escapeMode = false;
        int i;
        for (i = 1; i < findIn.length(); i++){
            if (findIn[i - 1] != '\\'){
                escapeMode = false;
            }
            if (findIn[i] == '\"' && !escapeMode){
                break;
            }
            if (findIn[i] == '\\'){
                escapeMode = !escapeMode;
            }
            else{
                desiredText += findIn[i];
            }
            if (!escapeMode && findIn[i] == '\\'){
                desiredText += findIn[i];
            }
        }
        *len = i + 1;
        if (findIn[i] != '\"'){
            *len = -1;
        }
    }
    else{
        int i;
        for (i = 0; i < findIn.length(); i++){
            if (findIn[i] == ' '){
                break;
            }
        }
        *len = i + 1;
        desiredText = findIn.left(i);
    }
    return desiredText;
}

}

