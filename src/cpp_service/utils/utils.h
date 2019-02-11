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

#ifndef UTILS_H
#define UTILS_H

#include <map>
#include <string>
#include <vector>
#include <curl/curl.h>

namespace timeSeries
{

/**
  * This method allows to calculate the euclidean distance between two SAX words. */
double euclideanDistanceBetweenWords(std::string &rInSubSequenceA, std::string &rInSubSequenceB, std::map<char, int> &rInAlphabet);

/**
  * This method allows to calculate the euclidean distance between two vectors.
  * The distance is calculated only with the samples between a specified subsequence
  * start and end points. */
double euclideanDistance(std::vector<double> &rInSubSequenceA, std::vector<double> &rInSubSequenceB);

/**
  * This method allows to discover all discords in a received time series using a brute force algorithm. */
void discordDiscoveryBruteForce(std::vector<double> &rInSeries,
                                const int subSequenceSize,
                                std::vector<int> &rOutDiscords);

/**
  * This method allows to check if a given two reference points from different sequences,
  * they are a self match. */
bool isSelfMatch(const int startA, const int startB, const int windowSize);

/**
  * This method allows to load files and stores them into temporary files.
  */
CURLcode loadUrlFile(std::string &errorMessage, const std::string &rUrl, std::string &rOutputFileName);

} // namespace timeSeries

#endif