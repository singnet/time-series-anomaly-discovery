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
      * This method saves the density curve generated so far. */
    void saveCsv(const char *pInOutputFileName);

    /**
      */
    void getJson(
        std::string &rOutTimeSeries,
        std::string &rOutDensityString,
        std::string &rOutNormalizedDensityString,
        std::string &rOutInvertedDensityString);

    /**
      */
    void getDensityCurve(std::vector<int> &rOutDensities);

    /**
      */
    void getNormalized(std::vector<int> &rOutDensities);

    /**
      */
    void getNormalizedInverted(std::vector<int> &rOutDensities);

    /**
      */
    void setCurve(std::vector<double> &rInTimeSeries, std::vector<int> &rInDensityCurve, const int windowSize);

  private:
    /**
      */
    void generateNormalizedAndInvert();

    int _windowSize;                 ///<
    std::vector<int> _density;       ///< Stores the densities for each word
    std::vector<double> _timeSeries; ///<
    std::vector<double> _normalized; ///<
    std::vector<double> _inverted;   ///<
};

} // namespace timeSeries

#endif