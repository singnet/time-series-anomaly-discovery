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

#ifndef SEQUITUR_UNIT_TESTS_IMPL_H
#define SEQUITUR_UNIT_TESTS_IMPL_H

#include <string>
#include <vector>

#include "../../symbolic_aggregate_approximation/SymbolicAggregateApproximation.h"
#include "../../sequitur/Sequitur.h"
#include "../../utils/timeSeriesUtils.h"

using namespace timeSeries;

static bool sequiturStringCompressionUnitTest(std::string &rInString, std::string &rOutString) {
    printf("\n\nSequitur string compression unit test...\n\n");

    Sequitur sequitur;
    //sequitur.setDebug(true);

    for (int symbol = 0; symbol < rInString.length(); symbol++) {
        std::string input_symbol = "";
        input_symbol += rInString.at(symbol);
        sequitur.insertSymbol(input_symbol.c_str());
    }

    //sequitur.enforceRulesUtility();

    sequitur.expandGrammar("~0", rOutString);

    printf("Input string: %s\n\n\n\nGrammar expanded string: %s\n", rInString.c_str(), rOutString.c_str());
}

static bool sequiturGrammarGenerationUnitTest(std::vector<double> &rInSeries,
                                              const int slidingWindowRange,
                                              const int paaSize,
                                              std::vector<std::string> &rInAlphabet,
                                              const char* pInExpectedOutput)
{
    printf("\n\nSequitur grammar generation unit test...\n\n");

    // generate a sax word given a sliding window
    int start = 0;
    int range = slidingWindowRange;
    int paa_size = paaSize;
    std::string word = "";

    Sequitur sequitur;

    for (int word_start_index = 0; word_start_index + slidingWindowRange < rInSeries.size(); word_start_index += 1)
    {
        // get subsequence
        std::vector<double> subsequence;
        getSubsequence(rInSeries, subsequence, word_start_index, slidingWindowRange);

        // normalize time series with znorm
        std::vector<double> znormed_subsequence;
        zNorm(subsequence, znormed_subsequence);

        // create sax object to generate a sax word
        SymbolicAggregateApproximation sax(rInAlphabet, znormed_subsequence);

        // compute sax word
        word = sax.sax(paa_size);

        // insert word into sequitur grammar generator
        sequitur.insertSymbol(word.c_str(), false);
    }

    // print grammar
    sequitur.printGrammar();

    // TODO::check output and assign return value
    return true;
}

#endif