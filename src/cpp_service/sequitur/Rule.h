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

#ifndef RULE_H
#define RULE_H

#include <vector>

#include "Range.h"

class Obj;

/**
 * \class Rule
 *
 * This class is used to easily represent a grammar rule. It contain 
 * the rule coverage vector (rule span intervals), its depth, length, usage,
 * and symbols.
 *
 * \author (last to touch it) $Author: Alysson Ribeiro da Silva
 *
 * Contact: alysson@singularitynet.io
 */
class Rule
{
  public:
    /**
      * This object constructor.
      */
    Rule() {}

    /**
      * This object destructor.
      */
    ~Rule() {}

    int _id;                          ///< Id used to represent this rule.
    int _usageCount;                  ///< This rule usage count.
    int _length;                      ///< This rule length in terms of rule span.
    int _depth;                       ///< This rule depth in the grammar tree.
    std::vector<Range> _ruleCoverage; ///< This rule coverage vector containg its ranges.
    std::vector<Obj *> _symbols;      ///< This rule symbols composed by sequitur's objects.
};

#endif