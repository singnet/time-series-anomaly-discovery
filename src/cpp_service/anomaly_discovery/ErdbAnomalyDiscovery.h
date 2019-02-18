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
 * \class ErdbAnomalyDiscovery (Efficient Rule-based Density Anomaly Discovery)
 *
 * This class allows to detect anomalies using the efficient rule-based anomaly discovery method.
 * It uses the Sequitur, Sax with Z-normalization, and Paa algorithms to build a density curve that 
 * is used to detect anomalies. In this density curve, a method inpired on the hill clibing algorithm 
 * is called to find the global and local minimum values. Those values are pointed as anomalies.
 *
 * \author (last to touch it) $Author: Alysson Ribeiro da Silva
 *
 * Contact: alysson@singularitynet.io
 */

#ifndef ERDB_ANOMALY_DISCOVERY_H
#define ERDB_ANOMALY_DISCOVERY_H

#include <string.h>
#include <vector>

#include "../density_curve/DensityCurve.h"
#include "../piecewise_aggregate_approximation/PiecewiseAggregateApproximation.h"
#include "../symbolic_aggregate_approximation/SymbolicAggregateApproximation.h"
#include "../sequitur/Sequitur.h"

namespace timeSeries
{

class ErdbAnomalyDiscovery
{
public:
  /** 
      * This constructor receives the parameters used to calculate the SAX words 
      * as inputs to the sequitur and density curve generator algorithms. */
  ErdbAnomalyDiscovery(std::vector<std::string> &rInSaxAlphabet, const int slidingWindowSize, const int paaSize);

  /**
      * Destructor used to delete this object heap variables. */
  ~ErdbAnomalyDiscovery();

  /**
      * This method allows to insert a new sample in the time 
      * series being build for this detection object. */
  void insertSample(const double sample);

  /**
      * This method calculates the density of the timeseries so far. */
  DensityCurve *getDensityCurve();

private:
  std::vector<std::string> _saxAlphabet; ///< used alphabet
  std::vector<double> _timeSeries;       ///< the original time series
  Sequitur _sequitur; ///< sequitur algorithm instance

  int _slidingWindowSize; ///< used to define the sliding window size for each generated sax word
  int _paaSize;           ///< the size of the approximated subsequence for each sliding window
};

} // namespace timeSeries

#endif