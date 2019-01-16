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
    ErdbAnomalyDiscovery(std::vector<std::string> &rInSaxAlphabet, const int slidingWindowSize, const int ppaSize);
    ~ErdbAnomalyDiscovery();
    void insertSample(const double sample);
    void insertTimeSeries(std::vector<double> &rInTimeSeries);
    void getAnomalies(std::vector<int> &rOutAnomaliesIndex, std::string *pOutAnomaliesIndexString = nullptr, bool debug = false);
    void printAnomalies(std::vector<int> &rInAnomaliesIndex);

  private:
    std::vector<std::string> _saxAlphabet;
    std::vector<double> _timeSeries;
    Sequitur _sequitur;
    DensityCurve _densityCurve;

    int _slidingWindowSize;
    int _ppaSize;
};

} // namespace timeSeries

#endif