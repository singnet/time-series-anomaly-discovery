#include <iostream>
#include <sstream>
#include <iterator>

#include "ErdbAnomalyDiscovery.h"
#include "../utils/timeSeriesUtils.h"

using namespace timeSeries;

ErdbAnomalyDiscovery::ErdbAnomalyDiscovery(std::vector<std::string> &rInSaxAlphabet, const int slidingWindowSize, const int ppaSize)
{
    _slidingWindowSize = slidingWindowSize;
    _ppaSize = ppaSize;
    _saxAlphabet.assign(rInSaxAlphabet.begin(), rInSaxAlphabet.end());
}

ErdbAnomalyDiscovery::~ErdbAnomalyDiscovery()
{
}

void ErdbAnomalyDiscovery::insertTimeSeries(std::vector<double> &rInTimeSeries)
{
    for (unsigned int sample = 0; sample < rInTimeSeries.size(); sample++)
    {
        insertSample(rInTimeSeries[sample]);
    }
}

void ErdbAnomalyDiscovery::insertSample(const double sample)
{
    // insert sample into time series
    _timeSeries.push_back(sample);

    // check time series size so far and decide to calculate a new sax word or not
    if (_timeSeries.size() >= _slidingWindowSize)
    {
        // set subsequence start and end position
        int word_start_index = _timeSeries.size() - _slidingWindowSize;

        // get subsequence
        std::vector<double> subsequence;
        getSubsequence(_timeSeries, subsequence, word_start_index, _slidingWindowSize);

        // normalize time series with znorm
        std::vector<double> znormed_subsequence;
        zNorm(subsequence, znormed_subsequence);

        // create sax object to generate a sax word
        SymbolicAggregateApproximation sax(_saxAlphabet, znormed_subsequence);

        // compute sax word
        std::string word = "";
        word = sax.sax(_ppaSize);

        // insert word into sequitur grammar generator
        _sequitur.insertSymbol(word.c_str());

        // insert word into density curve to keep track of it
        _densityCurve.insertSymbol(word.c_str());
    }
}

void ErdbAnomalyDiscovery::getAnomalies(std::vector<int> &rOutAnomaliesIndex, std::string *pOutAnomaliesIndexString, bool debug)
{
    // enforce rule utilities in sequitur for better compression
    _sequitur.enforceRulesUtility();

    // update density curve and calculate max and min, globall and locals
    _densityCurve.updateDensityCurve(_sequitur);

    // get anomalies with the efficient rule density-base anomaly discovery method
    _densityCurve.getGlobalMinDensities(rOutAnomaliesIndex);

    // extract string if output string is non-null
    if (pOutAnomaliesIndexString != nullptr)
    {
        std::stringstream out_string_stream;
        std::copy(rOutAnomaliesIndex.begin(), rOutAnomaliesIndex.end(), std::ostream_iterator<int>(out_string_stream, " "));
        pOutAnomaliesIndexString->assign(out_string_stream.str());
    }

    if (debug)
    {
        printf("\n\n---- request ----\n\nReceived time series:\n");
        printSeries(_timeSeries);
        printf("\nSliding window size: %d\nPPA size: %d\n", _slidingWindowSize, _ppaSize);
        printf("\nGenerated sequitur grammar:\n\n");
        _sequitur.printGrammar();
        printf("\nDensity curve statistics:\n\n");
        _densityCurve.printDensityCurve();
        printf("\nAnomalies index:\n\n");
        printAnomalies(rOutAnomaliesIndex);
    }
}

void ErdbAnomalyDiscovery::printAnomalies(std::vector<int> &rInAnomaliesIndex)
{
    for (int index = 0; index < rInAnomaliesIndex.size(); index++)
    {
        printf("%d ", rInAnomaliesIndex[index]);
    }
    printf("\n");
}