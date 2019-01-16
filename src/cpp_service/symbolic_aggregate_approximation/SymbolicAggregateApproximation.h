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

#ifndef SYMBOLIC_AGGREGATE_APPROXIMATION_H
#define SYMBOLIC_AGGREGATE_APPROXIMATION_H

#include <string>
#include <vector>

namespace timeSeries
{

class SymbolicAggregateApproximation
{
  public:
    SymbolicAggregateApproximation(std::vector<std::string> &rInAlphabet, std::vector<double> &rZNormTimeSeries);
    ~SymbolicAggregateApproximation();
    std::string sax(const int ppaSize, const int start = -1, const int range = -1);

  private:
    void generateSaxAlphabetRangeLimits(std::vector<double> &rOutRangeLimits,
                                        const double zNormSeriesMinimum,
                                        const double letterRange,
                                        const int alphabetSize);
    int getSaxAlphabetLetterIndex(const double mean, std::vector<double> &rInRangeLimits);

    double _letterRange;
    std::vector<double> _letterIndexRanges;
    std::vector<double> _rZNormTimeSeries;
    std::vector<std::string> _rAlphabet;
};

} // namespace timeSeries
#endif