#include <iostream>
#include <sstream>
#include <iterator>

#include "ErdbAnomalyDiscovery.h"
#include "../utils/timeSeriesUtils.h"

using namespace timeSeries;

ErdbAnomalyDiscovery::ErdbAnomalyDiscovery(std::vector<std::string> &rInSaxAlphabet, const int slidingWindowSize, const int paaSize)
{
    _slidingWindowSize = slidingWindowSize;
    _paaSize = paaSize;
    _saxAlphabet.assign(rInSaxAlphabet.begin(), rInSaxAlphabet.end());
}

ErdbAnomalyDiscovery::~ErdbAnomalyDiscovery()
{
}

void ErdbAnomalyDiscovery::insertSample(const double sample)
{
    // insert sample into time series
    _timeSeries.push_back(sample);

    std::string word = "";

    // check time series size so far and decide to calculate a new sax word or not
    if (_timeSeries.size() >= _slidingWindowSize)
    {
        // set subsequence start and end position
        int word_start_index = _timeSeries.size() - _slidingWindowSize;

        // get subsequence
        std::vector<double> subsequence;
        getSubsequence(_timeSeries, subsequence, word_start_index, _slidingWindowSize);

        // create sax object to generate a sax word
        SymbolicAggregateApproximation sax(_saxAlphabet, subsequence);

        // compute sax word
        word = sax.sax(_paaSize);

        // insert word into sequitur grammar generator
        _sequitur.insertSymbol(word.c_str(), word_start_index, _slidingWindowSize, true);
    }
}

DensityCurve *ErdbAnomalyDiscovery::getDensityCurve()
{
    // create a density curve object
    DensityCurve *curve = new DensityCurve();

    // get the densities generated alongside with the sequitur's algorithm
    std::vector<int> densities(_timeSeries.size(), 0);
    _sequitur.densityCurve(densities);

    // set the generated curve and original time series into the density curve object to be handled
    curve->setCurve(_timeSeries, densities, _slidingWindowSize);

    // return the generated curve
    return curve;
}