/* 
 * MIT License
 * 
 * Copyright (c) 2018 SingularityNET
 *
 * Author: Alysson Ribeiro da Silva
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef OBJ_H
#define OBJ_H

#include "Range.h"
#include "Symbol.h"
#include "Rule.h"

class Obj
{
  public:
    /**
     */
    Obj()
    {
        _symbol = NULL;
        _rule = NULL;
    }

    /**
     */
    ~Obj()
    {
    }

    /**
     */
    bool isSymbol()
    {
        if (_symbol != NULL)
            return true;
        return false;
    }

    /**
     */
    bool isRule()
    {
        if (_rule != NULL)
            return true;
        return false;
    }

    /**
     */
    bool operator==(Obj &B)
    {
        if (isSymbol() && B.isSymbol())
        {
            if (this->_symbol->_symbol == B._symbol->_symbol)
                return true;
        }

        if (isRule() && B.isRule())
        {
            if (_rule == B._rule)
                return true;
        }

        return false;
    }

    Range _range;    ///<
    Symbol *_symbol; ///<
    Rule *_rule;     ///<
};

#endif