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

#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <map>

#include "anomaly_discovery/ErdbAnomalyDiscovery.h"

namespace timeSeries
{

class SessionManager
{
  public:
    SessionManager();
    ~SessionManager();

    int startSession(std::vector<std::string> &rInAlphabet, const int slidingWindow, const int paaSize);
    void insertSample(const int sessionId, const double sample);
    void endSession(const int sessionId);
    void getAnomalies(const int sessionId, std::string &rOutOutputString, std::vector<int> &rOutDetectedAnomalies);

  private:
    int genId();

    int _idGen;
    std::map<int, ErdbAnomalyDiscovery*> _sessions;
};

} // namespace timeSeries

#endif