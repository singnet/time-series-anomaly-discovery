#include "Sequitur.h"
#include <boost/algorithm/string.hpp>

using namespace timeSeries;

Sequitur::Sequitur()
{
    _rBaseRule = new Rule();
    _rBaseRule->_usageCount = 0;
    _genId = 0;
    _baseRuleAddress = genId();
    _grammar[_baseRuleAddress] = _rBaseRule;
    _previousSymbol = NULL;
}

Sequitur::~Sequitur() {}

int Sequitur::genId()
{
    return _genId++;
}

void Sequitur::insertIntoGrammar(Rule *rule)
{
    int address = genId();
    rule->_id = address;
    _grammar[address] = rule;
}

void Sequitur::eraseRule(Rule *rule)
{
    int address = rule->_id;
    _grammar.erase(address);
}

void Sequitur::getLastDigram(Obj *&rOutLLast, Obj *&rOutLast)
{
    // check digrams
    int size = _grammar[_baseRuleAddress]->_symbols.size();
    rOutLast = _grammar[_baseRuleAddress]->_symbols.at(size - 1);
    rOutLLast = _grammar[_baseRuleAddress]->_symbols.at(size - 2);
}

int Sequitur::getFirstDigramRepetition()
{
    Obj *pLast;
    Obj *pLlast;
    getLastDigram(pLlast, pLast);

    for (int i = _grammar[_baseRuleAddress]->_symbols.size() - 4; i >= 0; i--)
    {
        if (*pLlast == *_grammar[_baseRuleAddress]->_symbols[i] && *pLast == *_grammar[_baseRuleAddress]->_symbols[i + 1])
            return i;
    }

    return -1;
}

Rule *Sequitur::checkForDigram()
{
    Obj *pLast;
    Obj *pLlast;
    getLastDigram(pLlast, pLast);
    string formed_digram = "";
    getDigramString(pLlast, pLast, formed_digram);

    if (_digramInRulesMap.find(formed_digram) != _digramInRulesMap.end())
        return _digramInRulesMap[formed_digram];

    return NULL;
}

void Sequitur::getDigramString(Obj *&rOutLLast, Obj *&rOutLast, string &rOutString)
{
    rOutString = "";
    if (rOutLLast->isSymbol())
    {
        rOutString += "{";
        rOutString += rOutLLast->_symbol->_symbol;
        rOutString += "}";
    }
    else
    {
        rOutString += "~";
        rOutString += std::to_string(rOutLLast->_rule->_id);
    }
    rOutString += "_";
    if (rOutLast->isSymbol())
    {
        rOutString += "{";
        rOutString += rOutLast->_symbol->_symbol;
        rOutString += "}";
    }
    else
    {
        rOutString += "~";
        rOutString += std::to_string(rOutLast->_rule->_id);
    }
}

bool Sequitur::generateRule()
{
    bool generated = false;

    // get first repetition index
    int index = getFirstDigramRepetition();

    Obj *pLast;
    Obj *pLlast;
    getLastDigram(pLlast, pLast);

    if (index != -1)
    {
        Rule *pNewRule = new Rule();
        pNewRule->_usageCount = 2;

        pNewRule->_symbols.push_back(pLlast);
        pNewRule->_symbols.push_back(pLast);

        // compute digram ranges
        Range found_digram_range;
        if (_grammar[_baseRuleAddress]->_symbols[index]->isSymbol())
            found_digram_range._start = _grammar[_baseRuleAddress]->_symbols[index]->_symbol->_ranges[0]._start;
        else
            found_digram_range._start = _grammar[_baseRuleAddress]->_symbols[index]->_range._start;

        if (_grammar[_baseRuleAddress]->_symbols[index + 1]->isSymbol())
            found_digram_range._end = _grammar[_baseRuleAddress]->_symbols[index + 1]->_symbol->_ranges[0]._end;
        else
            found_digram_range._end = _grammar[_baseRuleAddress]->_symbols[index + 1]->_range._end;

        Range last_rule_range;
        if (pLlast->isSymbol())
            last_rule_range._start = pLlast->_symbol->_ranges.at(pLlast->_symbol->_ranges.size() - 1)._start;
        else
            last_rule_range._start = pLlast->_range._start;

        if (pLast->isSymbol())
            last_rule_range._end = pLast->_symbol->_ranges.at(pLast->_symbol->_ranges.size() - 1)._end;
        else
            last_rule_range._end = pLast->_range._end;

        // update ranges
        if (_grammar[_baseRuleAddress]->_symbols[index]->isSymbol())
            pLlast->_symbol->_ranges.insert(
                pLlast->_symbol->_ranges.begin(),
                _grammar[_baseRuleAddress]->_symbols[index]->_symbol->_ranges.begin(),
                _grammar[_baseRuleAddress]->_symbols[index]->_symbol->_ranges.end());

        if (_grammar[_baseRuleAddress]->_symbols[index + 1]->isSymbol())
            pLast->_symbol->_ranges.insert(
                pLast->_symbol->_ranges.begin(),
                _grammar[_baseRuleAddress]->_symbols[index + 1]->_symbol->_ranges.begin(),
                _grammar[_baseRuleAddress]->_symbols[index + 1]->_symbol->_ranges.end());

        //insert rule
        insertIntoGrammar(pNewRule);

        // new object to replace last digram
        Obj *newRuleObj_1 = new Obj();
        newRuleObj_1->_rule = pNewRule;
        newRuleObj_1->_range = found_digram_range;

        // object to replace found digram
        Obj *newRuleObj_2 = new Obj();
        newRuleObj_2->_rule = pNewRule;
        newRuleObj_2->_range = last_rule_range;

        // set digram into rules digram mapping
        string formed_drigram;
        getDigramString(pLlast, pLast, formed_drigram);
        _digramInRulesMap[formed_drigram] = pNewRule;

        // insert range into rule
        pNewRule->_ruleCoverage.push_back(found_digram_range);
        pNewRule->_ruleCoverage.push_back(last_rule_range);

        // replace digram from the first found repetition
        _grammar[_baseRuleAddress]->_symbols[index] = newRuleObj_1;
        _grammar[_baseRuleAddress]->_symbols.erase(_grammar[_baseRuleAddress]->_symbols.begin() + index + 1);

        // replace last digram
        _grammar[_baseRuleAddress]->_symbols[_grammar[_baseRuleAddress]->_symbols.size() - 2] = newRuleObj_2;
        _grammar[_baseRuleAddress]->_symbols.erase(_grammar[_baseRuleAddress]->_symbols.begin() + _grammar[_baseRuleAddress]->_symbols.size() - 1);

        // update rule count and rule utility
        if (pLast->isRule())
        {
            pLast->_rule->_usageCount--;
            if (pLast->_rule->_usageCount == 1)
            {
                // realocate its symbols before deleting
                pNewRule->_symbols.erase(pNewRule->_symbols.end() - 1);
                pNewRule->_symbols.insert(pNewRule->_symbols.end() - 1, pLast->_rule->_symbols.begin(), pLast->_rule->_symbols.end());

                // delete rules digram from reference
                if (pLast->_rule->_symbols.size() == 2)
                {
                    string formed_drigram;
                    getDigramString(pLast->_rule->_symbols[0], pLast->_rule->_symbols[1], formed_drigram);
                    _digramInRulesMap.erase(formed_drigram);
                }

                // delete rule
                eraseRule(pLast->_rule);
            }
        }
        if (pLlast->isRule())
        {
            pLlast->_rule->_usageCount--;
            if (pLlast->_rule->_usageCount == 1)
            {
                // realocate its symbols before deleting
                pNewRule->_symbols.erase(pNewRule->_symbols.begin());
                pNewRule->_symbols.insert(pNewRule->_symbols.begin(), pLlast->_rule->_symbols.begin(), pLlast->_rule->_symbols.end());

                // delete rules digram from reference
                if (pLlast->_rule->_symbols.size() == 2)
                {
                    string formed_drigram;
                    getDigramString(pLlast->_rule->_symbols[0], pLlast->_rule->_symbols[1], formed_drigram);
                    _digramInRulesMap.erase(formed_drigram);
                }

                // delete rule
                eraseRule(pLlast->_rule);
            }
        }

        generated = true;
    }
    else
    {
        // check if rule exist for last digram
        Rule *rule = checkForDigram();

        if (rule != NULL)
        {
            if (rule->_symbols.at(0)->isSymbol())
            {
                Range new_range;
                new_range._start = pLlast->_symbol->_ranges.begin()->_start;
                new_range._end = pLlast->_symbol->_ranges.begin()->_end;
                rule->_symbols.at(0)->_symbol->_ranges.push_back(new_range);
            }
            else
            {
                rule->_symbols.at(0)->_rule->_usageCount--;
            }

            if (rule->_symbols.at(1)->isSymbol())
            {
                Range new_range;
                new_range._start = pLast->_symbol->_ranges.begin()->_start;
                new_range._end = pLast->_symbol->_ranges.begin()->_end;
                rule->_symbols.at(1)->_symbol->_ranges.push_back(new_range);
            }
            else
            {
                rule->_symbols.at(1)->_rule->_usageCount--;
            }

            Obj *newRuleObj = new Obj();
            newRuleObj->_rule = rule;
            rule->_usageCount++;

            // need to set new object range here
            Range replaced_range;
            replaced_range._start = _grammar[_baseRuleAddress]->_symbols[_grammar[_baseRuleAddress]->_symbols.size() - 2]->_range._start;
            replaced_range._end = _grammar[_baseRuleAddress]->_symbols[_grammar[_baseRuleAddress]->_symbols.size() - 1]->_range._end;

            newRuleObj->_range = replaced_range;

            // update coverage
            rule->_ruleCoverage.push_back(replaced_range);

            // remove references from rule 0
            _grammar[_baseRuleAddress]->_symbols[_grammar[_baseRuleAddress]->_symbols.size() - 2] = newRuleObj;
            _grammar[_baseRuleAddress]->_symbols.erase(_grammar[_baseRuleAddress]->_symbols.begin() + _grammar[_baseRuleAddress]->_symbols.size() - 1);

            generated = true;
        }
    }

    return generated;
}

void Sequitur::insertSymbol(const char *pInSymbol, const int sample, const int windowSize, const bool numerosityReduction)
{
    Symbol *pSymbol = NULL;

    if (!numerosityReduction)
    {
        // create a symbol for the received word
        pSymbol = new Symbol();
        pSymbol->_symbol.assign(pInSymbol);
        Range symbol_range;
        symbol_range._start = sample;
        symbol_range._end = sample + windowSize;
        pSymbol->_ranges.push_back(symbol_range);

        // create an object for this symbol
        Obj *object = new Obj();
        object->_symbol = pSymbol;
        object->_range._start = sample;
        object->_range._end = sample + windowSize;
        _rBaseRule->_symbols.push_back(object);

        unsigned int base_rule_size = _grammar[_baseRuleAddress]->_symbols.size();

        if (base_rule_size >= 4)
        {
            while (generateRule())
                ;
        }

        //printGrammar();

        return;
    }

    if (_previousSymbol != NULL)
    {
        if (_previousSymbol->_symbol != pInSymbol)
        {
            // create an object for this symbol
            Obj *object = new Obj();
            object->_symbol = _previousSymbol;
            object->_range._start = sample;
            object->_range._end = sample + windowSize;
            _rBaseRule->_symbols.push_back(object);

            unsigned int base_rule_size = _grammar[_baseRuleAddress]->_symbols.size();

            if (base_rule_size >= 4)
            {
                while (generateRule())
                    ;
            }

            //printGrammar();

            // create a symbol for the received word
            pSymbol = new Symbol();
            pSymbol->_symbol.assign(pInSymbol);
            Range symbol_range;
            symbol_range._start = sample;
            symbol_range._end = sample + windowSize;
            pSymbol->_ranges.push_back(symbol_range);
        }
        else
        {
            _previousSymbol->_ranges.at(0)._end = sample + windowSize;
        }
    }
    else
    {
        // create a symbol for the received word
        pSymbol = new Symbol();
        pSymbol->_symbol.assign(pInSymbol);
        Range symbol_range;
        symbol_range._start = sample;
        symbol_range._end = sample + windowSize;
        pSymbol->_ranges.push_back(symbol_range);
    }

    if (pSymbol != NULL)
        _previousSymbol = pSymbol;
}

void Sequitur::printGrammar()
{
    map<int, Rule *>::iterator it = _grammar.begin();
    for (; it != _grammar.end(); it++)
    {
        printf("%d -> ", it->first);
        for (unsigned int symbol = 0; symbol < it->second->_symbols.size(); symbol++)
        {
            if (it->second->_symbols[symbol]->isRule())
                printf("%d ", it->second->_symbols[symbol]->_rule->_id);
            else
            {
                printf("%s ", it->second->_symbols[symbol]->_symbol->_symbol.c_str());
            }
        }
        printf("\n");
    }
    printf("\n");
}

void Sequitur::printRulesUsageCount()
{
    map<int, Rule *>::iterator it = _grammar.begin();
    for (; it != _grammar.end(); it++)
    {
        printf("%d -> usage = %d\n", it->first, it->second->_usageCount);
    }
}

void Sequitur::expandRuleCoverage(Rule *rule, set<int> &coverage)
{
    for (int i = 0; i < rule->_ruleCoverage.size(); i++)
    {
        int start = rule->_ruleCoverage[i]._start;
        int end = rule->_ruleCoverage[i]._end;

        for (int point = start; point < end; point++)
            if (coverage.find(point) == coverage.end())
                coverage.insert(point);
    }
}

void Sequitur::printRulesCoverage()
{
    map<int, Rule *>::iterator it = _grammar.begin();
    for (; it != _grammar.end(); it++)
    {
        printf("%d -> coverage = ", it->first);
        for (int i = 0; i < it->second->_ruleCoverage.size(); i++)
        {
            int start = it->second->_ruleCoverage[i]._start;
            int end = it->second->_ruleCoverage[i]._end;

            printf("(%d,%d) ", start, end);
        }
        printf("\n");
    }
}

bool Sequitur::ruleCoverage(int sampleNumber, Rule *rule)
{
    set<int> coverage;
    expandRuleCoverage(rule, coverage);
    if (coverage.find(sampleNumber) != coverage.end())
        return true;
    return false;
}

void Sequitur::densityCurve(vector<int> &rOutDensity)
{
    map<int, Rule *>::iterator it = _grammar.begin();
    for (; it != _grammar.end(); it++)
    {
        if (it->first != 0)
        {
            set<int> coverage;
            expandRuleCoverage(it->second, coverage);
            set<int>::iterator cit = coverage.begin();
            for (; cit != coverage.end(); cit++)
                rOutDensity[*cit] += 1;
        }
    }
}

void Sequitur::expandGrammar(std::string &rOutString)
{
    expandGrammar(rOutString, _grammar[_baseRuleAddress]);
}

void Sequitur::expandGrammar(std::string &rOutString, Rule *pInRule)
{
    for (int i = 0; i < pInRule->_symbols.size(); i++)
    {
        if (pInRule->_symbols[i]->isSymbol())
            rOutString += pInRule->_symbols[i]->_symbol->_symbol;
        else
            expandGrammar(rOutString, pInRule->_symbols[i]->_rule);
    }
}