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

/**
 * \class Sequitur
 *
 * This class allows to generate free context grammar in real-time.
 *
 * \author (last to touch it) $Author: Alysson Ribeiro da Silva
 *
 * Contact: alysson@singularitynet.io
 */

#ifndef SEQUITUR_H
#define SEQUITUR_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <list>

#include "Rule.h"
#include "Obj.h"
#include "Symbol.h"
#include "Range.h"

using namespace std;

namespace timeSeries
{

class Sequitur
{
  public:
    Sequitur();
    ~Sequitur();

    /**
     */
    void insertSymbol(const char *pInSymbol, const int sample, const int windowSize, const bool numerosityReduction);

    /**
     */
    void printGrammar();

    /**
     */
    void printRulesUsageCount();

    /**
     */
    void expandGrammar(std::string &rOutString);

    /**
     */
    void densityCurve(vector<int> &rOutDensity);

  private:
    /**
     */
    void insertIntoGrammar(Rule *rule);

    /**
     */
    void eraseRule(Rule *rule);

    /**
     */
    void getLastDigram(Obj *&rOutLLast, Obj *&rOutLast);

    /**
     */
    int getFirstDigramRepetition();

    /**
     */
    Rule *checkForDigram();

    /**
     */
    void getDigramString(Obj *&rOutLLast, Obj *&rOutLast, string &rOutString);

    /**
     */
    bool generateRule();

    /**
     */
    void expandRuleCoverage(Rule *rule, set<int> &coverage);

    /**
     */
    void printRulesCoverage();

    /**
     */
    bool ruleCoverage(int sampleNumber, Rule *rule);

    /**
     */
    void expandGrammar(std::string &rOutString, Rule *pInRule);

    /**
     */
    int genId();

    /**
     */
    int _genId;

    int _baseRuleAddress;                  ///<
    Rule *_rBaseRule;                      ///<
    bool _flush;                           ///<
    Symbol *_previousSymbol;               ///<
    map<int, Rule *> _grammar;             ///<
    map<string, Rule *> _digramInRulesMap; ///<
};

} // namespace timeSeries

#endif