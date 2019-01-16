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

#ifndef SAX_UNIT_TESTS_IMPL_H
#define SAX_UNIT_TESTS_IMPL_H

#include <string>
#include <vector>

#include "../../symbolic_aggregate_approximation/SymbolicAggregateApproximation.h"
#include "../../utils/timeSeriesUtils.h"

using namespace timeSeries;

static bool saxWordGenerationUnitTest(std::vector<double> &rInSeries,
                                      const int slidingWindowRange,
                                      const int ppaSize,
                                      std::vector<std::string> &rInAlphabet,
                                      const char* pInExpectedOutput)
{
    printf("\n\nSax word unit test...\n\n");

    // generate a sax word given a sliding window
    int start = 0;
    int range = slidingWindowRange;
    int ppa_size = ppaSize;
    std::string word = "";

    std::vector<double> znormed_series;

    // normalize time series with znorm
    zNorm(rInSeries, znormed_series);

    // create sax object to generate a sax word
    SymbolicAggregateApproximation sax(rInAlphabet, znormed_series);

    for (int word_start_index = 0; word_start_index < rInSeries.size(); word_start_index += range)
    {
        // compute sax word
        word = sax.sax(ppa_size, word_start_index, word_start_index + range);

        // print generated sax word
        printf("Generated sax word for the subsequence starting at %d and ending at %d with ppa reduction size equals to %d: %s\n",
               word_start_index,
               word_start_index + range,
               ppa_size,
               word.c_str());
    }

    // TODO::check output and assign return value
    return true;
}

static bool saxWordGenerationSlidingWindowUnitTest(std::vector<double> &rInSeries,
                                                   const int slidingWindowRange,
                                                   const int ppaSize,
                                                   std::vector<std::string> &rInAlphabet,
                                                   const char* pInExpectedOutput)
{
    printf("\n\nSax word unit test...\n\n");

    // generate a sax word given a sliding window
    int start = 0;
    int range = slidingWindowRange;
    int ppa_size = ppaSize;
    std::string word = "";

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
        word = sax.sax(ppa_size);

        // print generated sax word
        printf("Generated sax word for the subsequence starting at %d and ending at %d with ppa reduction size equals to %d: %s\n",
               word_start_index,
               word_start_index + range,
               ppa_size,
               word.c_str());
    }

    // TODO::check output and assign return value
    return true;
}

#endif