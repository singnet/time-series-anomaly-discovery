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
    Sequitur();
    ~Sequitur();
    void insertSymbol(const char *pInSymbol);
    void setDebug(bool val);
    void printGrammar();
    int countSymbolInRules(const char *pInSymbol);
    bool enforceRulesUtility();

  private:
    void getRule(int ruleCode, std::string &rOutRule);
    void verifyRuleExistance(std::vector<std::string> &rInDigram, std::string &rOutRuleKey);
    void checkDigram(std::vector<std::string> &rInRule,
                     std::vector<std::string> &rInDigram,
                     std::vector<std::vector<std::string>::iterator> &rOutIndexDigramOcurrences);
    void getLastDigram(std::vector<std::string> &rInRule, std::vector<std::string> &rOutDigram);
    int genId();
    int ruleUsageCount(std::string &rInRuleSymbol,
                       std::vector<std::vector<std::string>::iterator> &rOutRuleOcurrences,
                       std::vector<std::string> &rOutRules);
    bool isRule(std::string &rInSymbol);
    bool generateRules();

    int _idGen;
    bool _debug;
    std::string _lastInsertedSymbol;
    std::map<std::string, std::vector<std::string>> _rules;
};

} // namespace timeSeries

#endif