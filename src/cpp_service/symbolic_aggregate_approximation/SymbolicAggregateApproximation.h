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
 * \class SymbolicAggregateApproximation
 *
 * This class allow to transform time series subsequences into letters of a specified alphabet.
 *
 * \author (last to touch it) $Author: Alysson Ribeiro da Silva
 *
 * Contact: alysson@singularitynet.io
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
    /** 
      * This constructor allows to initiate this object with an alphabet and a Z-normalized time series */
    SymbolicAggregateApproximation(std::vector<std::string> &rInAlphabet, std::vector<double> &rZNormTimeSeries);

    /** 
      * This destructor delete this object. */
    ~SymbolicAggregateApproximation();

    /**
      * Compute the sax word of a subsequence for the specified z-normalized time series. 
      * It received the paaSize variable since it used the PAA algorithm to reduce the
      * dimensionality of the time series. */
    std::string sax(const int paaSize, const int start = -1, const int range = -1);

  private:
    /**
      * This method generates the alphabet letter ranges. 
      * The ranges for are used to determine the letter in which 
      * each PAA subsequence will be transformed in. */
    void generateSaxAlphabetRangeLimits(std::vector<double> &rOutRangeLimits,
                                        const double zNormSeriesMinimum,
                                        const double letterRange,
                                        const int alphabetSize);

    /**
      * This method allows to calculate in which range a subsequence mean bellongs to.
      * It receives a range limits vectors to allow to compute the correct letter index. */
    int getSaxAlphabetLetterIndex(const double mean, std::vector<double> &rInRangeLimits);

    double _letterRange; ///< letter size given a PAA size
    std::vector<double> _letterIndexRanges; ///< letter ranges for each letter from the received alphabet
    std::vector<double> _rZNormTimeSeries; ///< the z-normalized received time series
    std::vector<std::string> _rAlphabet; ///< the received alphabet composed by strings
};

} // namespace timeSeries
#endif