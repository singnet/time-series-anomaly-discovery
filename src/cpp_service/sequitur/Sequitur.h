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

#include <map>
#include <string>
#include <vector>

namespace timeSeries
{

class Sequitur
{
  public:
    /// Sequitur constructor
    Sequitur();

    /// Sequitur destructor
    ~Sequitur();

    /** This method allows to insert symbols into the grammar being generated.
      * After inserting a symbol, the method will check if the last digram repeats.
      * If it does, then it will transform it into a grammar rule 
      * and start the process over again. It will do it until no more digrams
      * are available to be processed. */
    void insertSymbol(const char *pInSymbol, bool discardEquals = false);

    /**
      * This method sets the value of a debug flag.
      * It is used to check if debug messages will be printed during the
      * execution of this class methods */
    void setDebug(bool val);

    /** 
      * This method prints the grammar generated so far. */
    void printGrammar();

    /**
      * This method counts how many times a symbol is used across the
      * grammar generated so far. */
    int countSymbolInRules(const char *pInSymbol);

    /**
      * This method enforces the rule utility in the grammar generated so far,
      * where a rule needs to be used more than one time to be considered 
      * useful. */
    bool enforceRulesUtility();

    /**
      * This method expand each grammar rule and generate a full output string. */
    void expandGrammar(std::string rule, std::string &rOutString);

  private:
    /**
      * This method returns a rule and all its symbols.
      * The returned rule is from the grammar generated so far. */
    void getRule(int ruleCode, std::vector<std::string> &rOutRule);

    /**
      * This method verify it a rule exists, if it does then it will output 
      * it's key. */
    void verifyRuleExistance(std::vector<std::string> &rInDigram, std::string &rOutRuleKey);

    /**
      * This method check if a digram repeats over the rules of the grammar generated so far.
      * It returns a list containing all the digram ocurrences. */
    void checkDigram(std::vector<std::string> &rInRule,
                     std::vector<std::string> &rInDigram,
                     std::vector<std::vector<std::string>::iterator> &rOutIndexDigramOcurrences);

    /**
      * This method returns the last generated digram */          
    void getLastDigram(std::vector<std::string> &rInRule, std::vector<std::string> &rOutDigram);

    /**
      * This method returns an ID generated for a new rule.
      * It increments for each generated rule. */
    int genId();

    /**
      * This method counts the number of times a determined rule is
      * used over the grammar generated so far. */
    int ruleUsageCount(std::string &rInRuleSymbol,
                       std::vector<std::vector<std::string>::iterator> &rOutRuleOcurrences,
                       std::vector<std::string> &rOutRules);

    /**
      * This method is used to check if a symbol from the grammar
      * generated so far is a rule or a symbol.
      * It is primarily used to distinguish symbols from rules. */
    bool isRule(std::string &rInSymbol);

    /**
      * This method generate new rules for each digram repetition found. */
    bool generateRules();

    int _idGen; ///< last rule ID
    bool _debug; ///< debug flag used to check if debug messages will be printed
    std::string _lastInsertedSymbol; ///< last symbol inserted into the grammar
    std::map<std::string, std::vector<std::string>> _rules; ///< the rules map for the grammar generated so far
};

} // namespace timeSeries

#endif