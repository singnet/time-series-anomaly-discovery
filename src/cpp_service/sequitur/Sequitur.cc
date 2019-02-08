#include "Sequitur.h"
#include <boost/algorithm/string.hpp>

using namespace timeSeries;

Sequitur::Sequitur()
{
    _idGen = 1;
    _debug = false;
    _rules["~0"] = std::vector<std::string>();
}

Sequitur::~Sequitur()
{
}

int Sequitur::genId()
{
    return _idGen++;
}

void Sequitur::setDebug(bool val)
{
    _debug = val;
}

void Sequitur::getRule(int ruleCode, std::vector<std::string> &rOutRule)
{
    std::string rule_id_string = "~" + std::to_string(ruleCode);
    if (_rules.find(rule_id_string) != _rules.end())
    {
        rOutRule = _rules[rule_id_string];
    }
}

void Sequitur::checkDigram(std::vector<std::string> &rInRule,
                           std::vector<std::string> &rInDigram,
                           std::vector<std::vector<std::string>::iterator> &rOutIndexDigramOcurrences)
{
    rOutIndexDigramOcurrences.clear();
    int digram_size = 2;

    for (int rule_index = rInRule.size() - 2; rule_index >= 0; rule_index--)
    {
        if (rInRule[rule_index] == rInDigram[0] &&
            rInRule[rule_index + 1] == rInDigram[1])
        {
            bool insert = true;

            if (rOutIndexDigramOcurrences.size() > 0)
            {
                if (rInRule.begin() + rule_index + 1 == rOutIndexDigramOcurrences.back())
                {
                    insert = false;
                }
            }

            if (insert)
            {
                rOutIndexDigramOcurrences.push_back(rInRule.begin() + rule_index);
            }
        }
    }
}

void Sequitur::getLastDigram(std::vector<std::string> &rInRule, std::vector<std::string> &rOutDigram)
{
    rOutDigram.clear();
    rOutDigram.push_back(rInRule[rInRule.size() - 2]);
    rOutDigram.push_back(rInRule[rInRule.size() - 1]);
}

void Sequitur::verifyRuleExistance(std::vector<std::string> &rInDigram, std::string &rOutRuleKey)
{
    std::map<std::string, std::vector<std::string>>::iterator it = _rules.begin();
    for (; it != _rules.end(); it++)
    {
        if (it->second.size() > 2)
        {
            continue;
        }

        if (it->first == "~0")
        {
            continue;
        }

        std::vector<std::vector<std::string>::iterator> index_digram_ocurrences;
        checkDigram(it->second, rInDigram, index_digram_ocurrences);
        if (index_digram_ocurrences.size() > 0)
        {
            rOutRuleKey = it->first;
            return;
        }
    }
    return;
}

bool Sequitur::enforceRulesUtility()
{
    bool replaced = false;

    std::vector<std::string> rules_to_erase;

    std::map<std::string, std::vector<std::string>>::iterator it = _rules.begin();
    for (; it != _rules.end(); it++)
    {
        std::string current_rule = it->first;

        if (current_rule == "~0")
        {
            continue;
        }

        std::vector<std::vector<std::string>::iterator> rule_occurrences_symbols;
        std::vector<std::string> rule_occurrences;
        int rule_usage_count = ruleUsageCount(current_rule, rule_occurrences_symbols, rule_occurrences);

        // enforce rule utility only if it has been used once
        if (rule_usage_count == 1)
        {
            if (_debug)
            {
                printf("Enforcing rule: %s\n", current_rule.c_str());
            }
            // tell the algorithm that a replacement happened
            replaced = true;

            for (int rule = 0; rule < rule_occurrences.size(); rule++)
            {
                std::string current_rule = rule_occurrences[rule];
                int symbol_pos = rule_occurrences_symbols[rule] - _rules[current_rule].begin();

                // insert after rule
                _rules[current_rule].insert(rule_occurrences_symbols[rule] + 1, _rules[it->first].begin(), _rules[it->first].end());

                // remove the correct iterator
                _rules[current_rule].erase(_rules[current_rule].begin() + symbol_pos);
            }

            // erase rule
            rules_to_erase.push_back(current_rule);
            //_rules.erase();
        }
    }

    // need to erase after processing all data, since some compilers may lost reference to its elements if removed inside a loop
    for (int rule = 0; rule < rules_to_erase.size(); rule++)
    {
        _rules.erase(rules_to_erase[rule]);
    }

    return replaced;
}

int Sequitur::ruleUsageCount(std::string &rInRuleSymbol,
                             std::vector<std::vector<std::string>::iterator> &rOutRuleOccurrences,
                             std::vector<std::string> &rOutRules)
{
    std::map<std::string, std::vector<std::string>>::iterator it = _rules.begin();
    int count = 0;
    for (; it != _rules.end(); it++)
    {
        for (int symbol = 0; symbol < it->second.size(); symbol++)
        {
            if (it->second[symbol] == rInRuleSymbol)
            {
                rOutRuleOccurrences.push_back(it->second.begin() + symbol);
                rOutRules.push_back(it->first);
                count++;
            }
        }
    }
    return count;
}

bool Sequitur::isRule(std::string &rInSymbol)
{
    if (rInSymbol[0] == '~')
    {
        return true;
    }
    return false;
}

int Sequitur::countSymbolInRules(const char *pInSymbol)
{
    int count = 0;
    std::map<std::string, std::vector<std::string>>::iterator it = _rules.begin();
    for (; it != _rules.end(); it++)
    {
        // do not count with rule 0
        if (it->first == "~0")
        {
            continue;
        }

        //std::vector<std::string> expanded_rule;
        //getExpandedRuleVector(it->first, expanded_rule);
        for (int symbol = 0; symbol < it->second.size(); symbol++)
        {
            if (it->second[symbol] == pInSymbol)
            {
                count++;

                // need to break since this rule already covers this subsequence
                break;
            }
        }
    }
    return count;
}

bool Sequitur::generateRules()
{
    bool grammar_updated = true;

    // check the primary symbol for repetitions
    std::vector<std::vector<std::string>::iterator> digram_repetitions;
    std::vector<std::string> last_digram;
    getLastDigram(_rules["~0"], last_digram);
    checkDigram(_rules["~0"], last_digram, digram_repetitions);

    std::string rule = "";
    verifyRuleExistance(last_digram, rule);

    if (digram_repetitions.size() > 1)
    {
        // create new rule only if necessary
        if (rule.length() <= 0)
        {
            rule.append("~" + std::to_string(genId()));

            // insert new rule here to avoid losing ref for the primary symbol
            _rules[rule] = std::vector<std::string>();
            _rules[rule].insert(_rules[rule].begin(), last_digram.begin(), last_digram.end());
        }

        for (unsigned int digram_repetition = 0; digram_repetition < digram_repetitions.size(); digram_repetition++)
        {
            (*digram_repetitions[digram_repetition]) = rule;
            _rules["~0"].erase(digram_repetitions[digram_repetition] + 1);
        }
    }
    else
    {
        if (rule.size() > 0)
        {
            // replace last digram
            (*digram_repetitions[0]) = rule;
            _rules["~0"].erase(digram_repetitions[0] + 1);
        }
        else
        {
            // tell the system that no change was made
            grammar_updated = false;
        }
    }

    // print grammar for each iteration
    if (grammar_updated && _debug)
    {
        printf("Rule generated: %s\n", rule.c_str());
        printGrammar();
    }

    return grammar_updated;
}

void Sequitur::insertSymbol(const char *pInSymbol, bool discardEquals)
{
    if (_debug)
    {
        printf("Inserted symbol: %s\n", pInSymbol);
    }

    int primary_symbol_length = _rules["~0"].size();
    int minimum_symbol_length = 4;
    bool insert_newword = true;

    // ignore repeated words for better compression, it is indispensable for the correct working of the algorithm
    if (_lastInsertedSymbol.length() > 0 && discardEquals)
    {
        if (_lastInsertedSymbol == pInSymbol)
        {
            insert_newword = false;
        }
    }

    // allow multi char input symbols and rules
    if (insert_newword)
    {
        _rules["~0"].push_back(std::string(pInSymbol));
        _lastInsertedSymbol.assign(pInSymbol);
    }

    bool grammar_updated = false;
    do
    {
        if (primary_symbol_length < minimum_symbol_length)
        {
            break;
        }

        // update grammar until there is a possible change to be made
        grammar_updated = generateRules();

    } while (grammar_updated);

    enforceRulesUtility();

    if (_debug)
    {
        printGrammar();
    }
}

void Sequitur::printGrammar()
{
    std::map<std::string, std::vector<std::string>>::iterator it = _rules.begin();
    for (; it != _rules.end(); it++)
    {
        printf("%s -> ", it->first.c_str());
        for (unsigned int symbol = 0; symbol < it->second.size(); symbol++)
        {
            printf("%s ", it->second[symbol].c_str());
        }
        printf("\n");
    }
    printf("\n");
}

void Sequitur::getExpandedRuleVector(std::string rule, std::vector<std::string> &rOutSymbolVector)
{
    // clear out string to ensure that the op. will be performed ok
    std::vector<std::string> &rules_symbols = _rules[rule];

    for (unsigned int symbol = 0; symbol < rules_symbols.size(); symbol++)
    {
        std::string current_symbol = rules_symbols[symbol];

        if (isRule(current_symbol))
        {
            getExpandedRuleVector(current_symbol, rOutSymbolVector);
        }
        else
        {
            rOutSymbolVector.push_back(current_symbol);
        }
    }
}

void Sequitur::expandGrammar(std::string rule, std::string &rOutString)
{
    // clear out string to ensure that the op. will be performed ok
    std::vector<std::string> &rules_symbols = _rules[rule];

    for (unsigned int symbol = 0; symbol < rules_symbols.size(); symbol++)
    {
        std::string current_symbol = rules_symbols[symbol];

        if (isRule(current_symbol))
        {
            expandGrammar(current_symbol, rOutString);
        }
        else
        {
            rOutString.append(current_symbol);
        }
    }
}