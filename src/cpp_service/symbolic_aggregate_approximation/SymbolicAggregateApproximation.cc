#include <algorithm>
#include <cmath>

#include "../symbolic_aggregate_approximation/SymbolicAggregateApproximation.h"
#include "../piecewise_aggregate_approximation/PiecewiseAggregateApproximation.h"
#include "../utils/utils.h"
#include "../utils/timeSeriesUtils.h"
#include "../utils/utilsImpl.h"

using namespace timeSeries;

SymbolicAggregateApproximation::SymbolicAggregateApproximation(std::vector<std::string> &rInAlphabet, std::vector<double> &rZNormTimeSeries)
{
    _rAlphabet.assign(rInAlphabet.begin(), rInAlphabet.end());
    _rZNormTimeSeries.assign(rZNormTimeSeries.begin(), rZNormTimeSeries.end());
}

SymbolicAggregateApproximation::~SymbolicAggregateApproximation()
{
}

void SymbolicAggregateApproximation::generateSaxAlphabetRangeLimits(std::vector<double> &rOutRangeLimits,
                                                                    const double zNormSeriesMinimum,
                                                                    const double letterRange,
                                                                    const int alphabetSize)
{
    _letterIndexRanges.clear();
    double range_upper_limit = zNormSeriesMinimum + letterRange;
    for (int i = 0; i < alphabetSize; i++)
    {
        rOutRangeLimits.push_back(range_upper_limit);
        range_upper_limit += letterRange;
    }
}

int SymbolicAggregateApproximation::getSaxAlphabetLetterIndex(const double mean, std::vector<double> &rInRangeLimits)
{
    for (unsigned int upper_limit = 0; upper_limit < rInRangeLimits.size(); upper_limit++)
    {
        if (mean <= rInRangeLimits[upper_limit])
        {
            return upper_limit;
        }
    }

    return rInRangeLimits.size() - 1;
}

std::string SymbolicAggregateApproximation::sax(const int paaSize, const int start, const int range)
{
    // sax word to be returned
    std::string sax_word = "";

    // define subsequence to seek for
    int s_start = 0;
    int s_end = (int)_rZNormTimeSeries.size();

    // approximate subsequence with paa to get sax words
    PiecewiseAggregateApproximation paa(&_rZNormTimeSeries, paaSize);
    std::vector<double> approximated_series = *paa.getApproximatedSeries();

    // normalize time series with znorm
    std::vector<double> znormed_subsequence;
    zNorm(approximated_series, znormed_subsequence);

    // series max value
    double rOutMax = *std::max_element(znormed_subsequence.begin(), znormed_subsequence.end());

    // series min value
    double rOutMin = *std::min_element(znormed_subsequence.begin(), znormed_subsequence.end());

    // alphabet size
    double alphabet_size = (double)_rAlphabet.size();

    // calculate sax letter range for alphabet definition
    _letterRange = (rOutMax + std::abs(rOutMin)) / alphabet_size;

    // calculate letter ranges once for this alphabet size and received series or subsequence
    generateSaxAlphabetRangeLimits(_letterIndexRanges, rOutMin, _letterRange, alphabet_size);

    // calculate sax word based on the approximated time series interval obtained with the paa algorithm
    for (unsigned int sample = 0; sample < znormed_subsequence.size(); sample++)
    {
        int letter_index = getSaxAlphabetLetterIndex(znormed_subsequence[sample], _letterIndexRanges);
        sax_word.append(_rAlphabet.at(letter_index));
    }

    return sax_word;
}