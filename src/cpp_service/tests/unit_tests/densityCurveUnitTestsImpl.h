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

#ifndef DENSITY_CURVE_UNIT_TESTS_IMPL_H
#define DENSITY_CURVE_UNIT_TESTS_IMPL_H

#include <string>
#include <vector>

#include "../../density_curve/DensityCurve.h"
#include "../../symbolic_aggregate_approximation/SymbolicAggregateApproximation.h"
#include "../../sequitur/Sequitur.h"
#include "../../utils/timeSeriesUtils.h"

using namespace timeSeries;

static bool densityCurveGenerationSlidingWindowUnitTest(std::vector<double> &rInSeries,
                                                        const int slidingWindowRange,
                                                        const int ppaSize,
                                                        std::vector<std::string> &rInAlphabet,
                                                        const char* pInExpectedOutput)
{
    printf("\n\nDensity curve generation unit test...\n\n");

    // generate a sax word given a sliding window
    int start = 0;
    int range = slidingWindowRange;
    int ppa_size = ppaSize;
    std::string word = "";

    Sequitur sequitur;
    DensityCurve density_curve;

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

        // insert word into sequitur grammar generator
        sequitur.insertSymbol(word.c_str());

        // insert word into density curve
        density_curve.insertSymbol(word.c_str());
    }

    // enforce rule utility
    sequitur.enforceRulesUtility();

    // update density curve and calculate max and min intervals
    density_curve.updateDensityCurve(sequitur);

    // printf generated curve
    density_curve.printDensityCurve();

    // TODO::check output and assign return value
    return true;
}

#endif