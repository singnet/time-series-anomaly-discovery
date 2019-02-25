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

/**
 * \class Sequitur
 *
 * This class allows to generate context-free grammars in polinomial time given input words.
 *
 * \author (last to touch it) $Author: Alysson Ribeiro da Silva
 *
 * Contact: alysson@singularitynet.io
 */
class Sequitur
{
  public:
    /**
      * This object constructor.
      */  
    Sequitur();
  
    /**
      * This object destructor.
      */
    ~Sequitur();

    /**
      * This method allows to insert symbols into the grammar being generated.
      */
    void insertSymbol(const char *pInSymbol, const int sample, const int windowSize, const bool numerosityReduction);

    /**
      * This method allows to print the so far generated grammar.
      */
    void printGrammar();

    /**
      * This method allows to print the rule usage count.
      */
    void printRulesUsageCount();

    /**
      * This method allows to expand the S symbol for the so far generated grammar.
      */
    void expandGrammar(std::string &rOutString);

    /**
      * This method generates a density curve for the generated grammar so far.
      */
    void densityCurve(vector<int> &rOutDensity);

  private:
    /**
      * This method allows to insert a rule into the grammar, it will automatically generate
      * a ID for it.
      */
    void insertIntoGrammar(Rule *rule);

    /**
      * This method allows to erase a rule that was crated for this grammar.
      */
    void eraseRule(Rule *rule);

    /**
      * This method allows to get the last generated digram.
      */
    void getLastDigram(Obj *&rOutLLast, Obj *&rOutLast);

    /**
      * This method find the first repetition of the last digram for each symbol insertion operation.
      * (TODO:needs to be replaced by a digram map)
      */
    int getFirstDigramRepetition();

    /**
      * This method check if a digram exists at least in one of the existant rules.
      */
    Rule *checkForDigram();

    /**
      * This method allows to transform a digram into a key string that can be
      * used in hash maps.
      */
    void getDigramString(Obj *&rOutLLast, Obj *&rOutLast, string &rOutString);

    /**
      * This method generates rules by enforcing the rule utility and digram uniqueness.
      */
    bool generateRule();

    /**
      * This method expand a rule and get all its coverage without repetition.
      */
    void expandRuleCoverage(Rule *rule, set<int> &coverage);

    /**
      * This method prints the coverage of a rule.
      */
    void printRulesCoverage();

    /**
      * This method checks if a sample belongs to the coverage of a rule.
      */
    bool ruleCoverage(int sampleNumber, Rule *rule);

    /**
      * This recursive method expands a symbol from the grammar and returns is string.
      */
    void expandGrammar(std::string &rOutString, Rule *pInRule);

    /** 
      * This method generates an ID with an integer counter.
      */
    int genId();

    int _genId;                            ///< Used to generate IDs.
    int _baseRuleAddress;                  ///< Used to store the S rule index ni the rules mapping.
    Rule *_rBaseRule;                      ///< Used to store the S base rule. (DEPRECATED)
    Symbol *_previousSymbol;               ///< Used to store the previous inserted symbol to control the numerosity reduction.
    map<int, Rule *> _grammar;             ///< Used to store the rules generated so far (the grammar).
    map<string, Rule *> _digramInRulesMap; ///< Used to store a reference to all rules that hold a exactly two symbols (digram).
};

} // namespace timeSeries

#endif