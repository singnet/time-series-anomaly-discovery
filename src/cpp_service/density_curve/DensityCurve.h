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
 * \class DensityCurve
 *
 * This class holds a vector used to store density values. Density values
 * are the number of times a grammar rule spans over a time series.
 * Besides that, it allows to find anomalies by applying optimization methods.
 *
 * \author (last to touch it) $Author: Alysson Ribeiro da Silva
 *
 * Contact: alysson@singularitynet.io
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
    /**
      * This method allows to create an instance of this class. */
    DensityCurve();

    /**
      * Destructor for this class. */
    ~DensityCurve();

    /**
      * This method inserts a symbol at the last position of the density curve. */
    void insertSymbol(const char *pInWord);

    /**
      * This method counts how many times each symbol from the current density curve
      * appears in the sequitur's instance generated grammar so far. */
    void updateDensityCurve(Sequitur &rInSequitur);

    /**
      * This method returns the global mininum intervals for the density curve updated so far. */
    void getGlobalMinDensities(std::vector<int> &rOutVec);

    /**
      * This method returns the global maximum intervals for the density curve updated so far. */
    void getGlobalMaxDensities(std::vector<int> &rOutVec);

    /**
      * This method returns the local mininum intervals for the density curve updated so far. */
    void getLocalMinDensities(std::vector<int> &rOutVec);

    /**
      * This method returns the local max intervals for the density curve updated so far. */
    void getLocalMaxDensities(std::vector<int> &rOutVec);

    /**
      * This method prints the current density curve so far. */
    void printDensityCurve();

    /**
      * This method saves the density curve generated so far. */
    void saveCsv(const char *pInOutputFileName);

  private:
    /**
      * This method computes the maximum and minimum intervals.
      * It uses the delta between density curve samples to check if a change on information
      * happened. If so, then it starts to accumulate values until finding another change.
      * Intervals between changes are stored as maximum or minimum values. */
    void findLocalMinMaxIntervals();

    std::vector<std::string> _seriesWords; ///< Stores the time series words
    std::vector<int> _density; ///< Stores the densities for each word

    std::vector<int> _localMinPoints; ///< local minimum intervals
    std::vector<int> _localMaxPoints; ///< local maximum intervals
    std::vector<int> _globalMaxPoints; ///< global maximum intervals
    std::vector<int> _globalMinPoints; ///< global minimum intervals
    double _mean; ///< curve mean
    double _standardDeviation; ///< curve standard deviation
    double _minDensity; ///< curve minimum density
    double _maxDensity; ///< curve maximum density
};

} // namespace timeSeries

#endif