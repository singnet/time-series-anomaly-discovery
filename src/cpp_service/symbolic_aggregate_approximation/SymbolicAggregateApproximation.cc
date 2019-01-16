#include <algorithm>
#include <cmath>

#include "../symbolic_aggregate_approximation/SymbolicAggregateApproximation.h"
#include "../piecewise_aggregate_approximation/PiecewiseAggregateApproximation.h"
#include "../utils/utils.h"
#include "../utils/utilsImpl.h"

using namespace timeSeries;

SymbolicAggregateApproximation::SymbolicAggregateApproximation(std::vector<std::string> &rInAlphabet, std::vector<double> &rZNormTimeSeries)
{
    _rAlphabet.assign(rInAlphabet.begin(), rInAlphabet.end());
    _rZNormTimeSeries.assign(rZNormTimeSeries.begin(), rZNormTimeSeries.end());

    // alphabet size
    double alphabet_size = (double)_rAlphabet.size();

    // series max value
    double rOutMax = *std::max_element(rZNormTimeSeries.begin(), rZNormTimeSeries.end());

    // series min value
    double rOutMin = *std::min_element(rZNormTimeSeries.begin(), rZNormTimeSeries.end());

    // calculate sax letter range for alphabet definition
    _letterRange = (rOutMax + std::abs(rOutMin)) / alphabet_size;

    // calculate letter ranges once for this alphabet size and received series or subsequence
    generateSaxAlphabetRangeLimits(_letterIndexRanges, rOutMin, _letterRange, alphabet_size);
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

std::string SymbolicAggregateApproximation::sax(const int ppaSize, const int start, const int range)
{
    // sax word to be returned
    std::string sax_word = "";

    // define subsequence to seek for
    int s_start = 0;
    int s_end = (int)_rZNormTimeSeries.size();

    if (start > 0)
    {
        if (start < _rZNormTimeSeries.size())
        {
            s_start = start;
        }

        if (start + range > 0 &&
            start + range < _rZNormTimeSeries.size())
        {
            s_end = start + range;
        }
    }

    // get subsequence from the normalized series
    std::vector<double> znormed_subsequence;
    znormed_subsequence.assign(_rZNormTimeSeries.begin() + s_start, _rZNormTimeSeries.begin() + s_end);

    // approximate subsequence with ppa to get sax words
    PiecewiseAggregateApproximation ppa(&znormed_subsequence, ppaSize);
    std::vector<double> approximated_series = *ppa.getApproximatedSeries();

    // calculate sax word based on the approximated time series interval obtained with the ppa algorithm
    for (unsigned int sample = 0; sample < approximated_series.size(); sample++)
    {
        int letter_index = getSaxAlphabetLetterIndex(approximated_series[sample], _letterIndexRanges);
        sax_word.append(_rAlphabet.at(letter_index));
    }

    return sax_word;
}