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

#ifndef DENSITY_CURVE_H
#define DENSITY_CURVE_H

#include "../sequitur/Sequitur.h"
#include <string.h>
#include <vector>

namespace timeSeries
{

class DensityCurve
{
  public:
    DensityCurve();
    ~DensityCurve();
    void insertSymbol(const char *pInWord);
    void updateDensityCurve(Sequitur &rInSequitur);
    void getGlobalMinDensities(std::vector<int> &rOutVec);
    void getGlobalMaxDensities(std::vector<int> &rOutVec);
    void getLocalMinDensities(std::vector<int> &rOutVec);
    void getLocalMaxDensities(std::vector<int> &rOutVec);
    void printDensityCurve();
    void saveCsv(const char *pInOutputFileName);

  private:
    void findLocalMinMaxIntervals();

    std::vector<std::string> _seriesWords;
    std::vector<int> _density;

    // statistics
    std::vector<int> _localMinPoints;
    std::vector<int> _localMaxPoints;
    std::vector<int> _globalMaxPoints;
    std::vector<int> _globalMinPoints;
    double _mean;
    double _standardDeviation;
    double _minDensity;
    double _maxDensity;
};

} // namespace timeSeries

#endif