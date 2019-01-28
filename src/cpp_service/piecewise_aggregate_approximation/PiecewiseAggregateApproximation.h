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
 * \class PiecewiseAggregateApproximation
 *
 * This class allows to to performing dimensionality reduction in continuous signals, time series, vectors, etc.
 *
 * \author (last to touch it) $Author: Alysson Ribeiro da Silva
 *
 * Contact: alysson@singularitynet.io
 */

#ifndef PIECEWISE_AGGREGATE_APPROXIMATION_H
#define PIECEWISE_AGGREGATE_APPROXIMATION_H

#include <vector>

namespace timeSeries
{

class PiecewiseAggregateApproximation
{
  public:
    /**
      * Create an instance of this class specifying the time series to approximated and the size of the approximation. 
      * This method also calculate the approximation window for each new sample and create the new time series. */
    PiecewiseAggregateApproximation(std::vector<double> *pInTimeSeries, const int approximationLength);

    /// This class destructor.
    ~PiecewiseAggregateApproximation();

    /**
      * This method returns a pointer to the approximated time series. */
    std::vector<double> *getApproximatedSeries();

    /**
      * This method returns a points to the original time series. */
    std::vector<double> *getTimeSeries();

    /**
      * This method returns the length of the approximated time series. */
    int getApproximatedPointLength();

  private:
    std::vector<double> *_pTimeSeries; ///< original time series points
    std::vector<double> *_pApproximatedTimeSeries; ///< approximated time series sample
    int _approximatedPointLength; ///< approximated time series length
};

} // namespace timeSeries

#endif