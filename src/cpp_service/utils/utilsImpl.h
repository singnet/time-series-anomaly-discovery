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

#ifndef UTILS_IMPL_H
#define UTILS_IMPL_H

#include <cmath>
#include <vector>

namespace timeSeries
{

template <class vecType, class returnType>
returnType mean(std::vector<vecType> &rInSubSequence, const int start, const int range)
{
    returnType sum = 0.0;

    for (unsigned int sample = start; sample < start + range; sample++)
    {
        sum += (returnType)rInSubSequence[sample];
    }

    return sum / (returnType)range;
}

template <class vecType, class returnType>
returnType standardDeviation(const returnType mean, std::vector<vecType> &rInSubSequence, const int start, const int range)
{
    returnType distance_to_mean_sum = 0.0;

    for (unsigned int sample = start; sample < start + range; sample++)
    {
        distance_to_mean_sum += std::pow(mean - (returnType)rInSubSequence[sample], 2.0);
    }

    return std::sqrt(distance_to_mean_sum / ((returnType)range - 1));
}

} // namespace timeSeries

#endif