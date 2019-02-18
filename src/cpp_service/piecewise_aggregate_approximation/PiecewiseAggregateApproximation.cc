#include "PiecewiseAggregateApproximation.h"
#include "../utils/utils.h"
#include "../utils/utilsImpl.h"

using namespace timeSeries;

PiecewiseAggregateApproximation::PiecewiseAggregateApproximation(std::vector<double> *pInTimeSeries, const int approximationLength)
{
    // initialize class variable to hold the time series
    _pTimeSeries = pInTimeSeries;

    // initialize class variable to hold the approximated time series
    _pApproximatedTimeSeries = new std::vector<double>();

    // calculate the approximated word length
    _approximatedPointLength = _pTimeSeries->size() / approximationLength;

    // approximated points with mean
    for (unsigned int sample = 0; sample < _pTimeSeries->size(); sample += _approximatedPointLength)
    {
        double samples_mean = mean<double, double>(*_pTimeSeries, sample, _approximatedPointLength);
        _pApproximatedTimeSeries->push_back(samples_mean);
    }
}

PiecewiseAggregateApproximation::~PiecewiseAggregateApproximation()
{
    // only set its pointer to null to avoid errors, the object should not be deleted here since it is created outside
    _pTimeSeries = nullptr;

    // clean approximation allocated memory
    if (_pApproximatedTimeSeries != nullptr)
    {
        delete _pApproximatedTimeSeries;
        _pApproximatedTimeSeries = nullptr;
    }
}

int PiecewiseAggregateApproximation::getApproximatedPointLength()
{
    return _approximatedPointLength;
}

std::vector<double> *PiecewiseAggregateApproximation::getApproximatedSeries()
{
    return _pApproximatedTimeSeries;
}

std::vector<double> *PiecewiseAggregateApproximation::getTimeSeries()
{
    return _pTimeSeries;
}