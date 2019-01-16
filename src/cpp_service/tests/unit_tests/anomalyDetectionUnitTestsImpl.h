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

#ifndef ANOMALY_DETECTION_UNIT_TESTS_IMPL_H
#define ANOMALY_DETECTION_UNIT_TESTS_IMPL_H

#include <string.h>
#include <vector>

#include "../../anomaly_discovery/ErdbAnomalyDiscovery.h"

using namespace timeSeries;

static bool anomalyDiscoveryErdbUnitTest(std::vector<double> &rInSeries,
                                         const int slidingWindowRange,
                                         const int ppaSize,
                                         std::vector<std::string> &rInAlphabet,
                                         const char* pInExpectedOutput)
{
    printf("\n\nEfficient rule-density anomaly discovery unit test...\n\n");

    // create an anomaly discovery object
    ErdbAnomalyDiscovery anomaly_discovery(rInAlphabet, slidingWindowRange, ppaSize);

    // iterate over the provided time series
    for (unsigned int sample = 0; sample < rInSeries.size(); sample++)
    {
        anomaly_discovery.insertSample(rInSeries[sample]);
    }

    // for all conditions of sequitur to be applied, generate the density curve statistics, and get detected anomalies
    std::vector<int> detected_anomalies_index;
    anomaly_discovery.getAnomalies(detected_anomalies_index);

    // print all detected anomalies
    for (unsigned int anomaly = 0; anomaly < detected_anomalies_index.size(); anomaly++)
    {
        printf("Anomaly index: %d\n", detected_anomalies_index[anomaly]);
    }

    // TODO::check output and assign return value
    return true;
}

#endif