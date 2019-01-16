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

#ifndef TIME_SERIES_HANDLER_H
#define TIME_SERIES_HANDLER_H

#include <vector>

namespace timeSeries
{
void getSubsequence(std::vector<double> &rInSeries, std::vector<double> &rOutSubsequence, const int start, const int range);
void zNorm(std::vector<double> &rInSubSequence, std::vector<double> &rOutNormalizedSubSequence);
void generateSenoidSeries(std::vector<double> &rOutSeries,
                          const int size,
                          const double frequency,
                          const double amplitude,
                          const double phase);
void generateRandomSeries(std::vector<double> &rOutSeries, const int size);
void insertNoise(std::vector<double> &rOutSeries, const int start, const int end);
void printSeries(std::vector<double> &rInSubSequence);
void saveSeriesCsv(const char *pInOutputFile, std::vector<double> &rInSeries);
void loadSeriesCsv(const char *pInOutputFile, std::vector<double> &rOutSeries, const bool hasHeader);

} // namespace timeSeries

#endif