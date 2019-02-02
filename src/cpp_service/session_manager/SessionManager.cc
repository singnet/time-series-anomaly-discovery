#include "SessionManager.h"

using namespace timeSeries;

int SessionManager::startSession(std::vector<std::string> &rInAlphabet, const int slidingWindow, const int paaSize)
{
    // generate a new ID for the new session
    int newly_generated_session = genId();

    // store a new ErdbAnomalyDiscovery object to detect anomalies for this session
    _sessions[newly_generated_session] = new ErdbAnomalyDiscovery(rInAlphabet, slidingWindow, paaSize);

    // returned session if
    return newly_generated_session;
}

void SessionManager::insertSample(const int sessionId, const double sample)
{
    if (_sessions.find(sessionId) != _sessions.end())
    {
        // insert sample into a valid session
        _sessions[sessionId]->insertSample(sample);
    }
    else
    {
        // TODO::raise error
    }
}

void SessionManager::endSession(const int sessionId)
{
    if (_sessions.find(sessionId) != _sessions.end())
    {
        // free mem
        delete _sessions[sessionId];

        // end session
        _sessions.erase(sessionId);
    }
}

void SessionManager::getAnomalies(const int sessionId, std::string &rOutOutputString, std::vector<int> &rOutDetectedAnomalies)
{
    if (_sessions.find(sessionId) != _sessions.end())
    {
        // debug status flag for debug printing inside getAnomalies
        bool debug_status = false;

        // get all anomalies inside for this session
        _sessions[sessionId]->getAnomalies(rOutDetectedAnomalies, &rOutOutputString, debug_status);
    }
}

int SessionManager::genId()
{
    return _idGen++;
}