#include <limits>

#include "DensityCurve.h"
#include "../utils/utils.h"
#include "../utils/utilsImpl.h"

using namespace timeSeries;

DensityCurve::DensityCurve()
{
}

DensityCurve::~DensityCurve()
{
}

void DensityCurve::saveCsv(const char *pInOutputFileName)
{
    FILE *pOutputFile;
    pOutputFile = fopen(pInOutputFileName, "w");

    // write header
    fprintf(pOutputFile, "word,count\n");
    for (unsigned int density_curve_point = 0; density_curve_point < _seriesWords.size(); density_curve_point++)
    {
        fprintf(pOutputFile, "%s,%d\n", _seriesWords[density_curve_point].c_str(), _density[density_curve_point]);
    }
    fclose(pOutputFile);
}

void DensityCurve::printDensityCurve()
{
    printf("\n");
    printf("Curve mean: %f\n", _mean);
    printf("Curve deviation: %f\n", _standardDeviation);
    printf("Local max intervals: ");
    for (unsigned int point = 0; point < _localMaxPoints.size(); point++)
    {
        printf("%d ", _localMaxPoints[point]);
    }
    printf("\n");
    printf("Local min intervals: ");
    for (unsigned int point = 0; point < _localMinPoints.size(); point++)
    {
        printf("%d ", _localMinPoints[point]);
    }
    printf("\n");
    printf("Max density: %f\n", _maxDensity);
    printf("Min density: %f\n", _minDensity);
    printf("Global max intervals: ");
    for (unsigned int point = 0; point < _globalMaxPoints.size(); point++)
    {
        printf("%d ", _globalMaxPoints[point]);
    }
    printf("\n");
    printf("Global min intervals: ");
    for (unsigned int point = 0; point < _globalMinPoints.size(); point++)
    {
        printf("%d ", _globalMinPoints[point]);
    }
    printf("\n");
}

void DensityCurve::findLocalMinMaxIntervals()
{
    int current_min = _density[1];
    int current_max = _density[1];
    int max_mark = 1;
    int min_mark = 1;
    for (unsigned int density_curve_point = 1; density_curve_point < _density.size(); density_curve_point++)
    {
        int current_count = _density[density_curve_point];

        // check for global max and min
        if (_density[density_curve_point] == _minDensity)
        {
            _globalMinPoints.push_back(density_curve_point);
        }

        if (_density[density_curve_point] == _maxDensity)
        {
            _globalMaxPoints.push_back(density_curve_point);
        }

        // check if there was a positive change in data
        if (_density[density_curve_point - 1] - _density[density_curve_point] < 0)
        {
            max_mark = density_curve_point;

            if (_density[min_mark - 1] > _density[min_mark])
            {
                // setup this interval
                for (int interval_point = min_mark; interval_point < density_curve_point; interval_point++)
                {
                    _localMinPoints.push_back(interval_point);
                }

                min_mark = density_curve_point;
            }
        }

        // check if there was a negative change in data
        if (_density[density_curve_point - 1] - _density[density_curve_point] > 0)
        {
            min_mark = density_curve_point;

            // check max
            if (_density[max_mark - 1] < _density[max_mark])
            {
                // setup this interval
                for (int interval_point = max_mark; interval_point < density_curve_point; interval_point++)
                {
                    _localMaxPoints.push_back(interval_point);
                }

                max_mark = density_curve_point;
            }
        }
    }
}

void DensityCurve::updateDensityCurve(Sequitur &rInSequitur)
{
    _minDensity = std::numeric_limits<double>::max();
    _maxDensity = 0.0;
    double mean = 0.0;

    for (unsigned int density_curve_point = 0; density_curve_point < _seriesWords.size(); density_curve_point++)
    {
        std::string current_point_symbol = _seriesWords[density_curve_point];
        int count = rInSequitur.countSymbolInRules(current_point_symbol.c_str());
        _density[density_curve_point] = count;

        // compute mean sum
        mean += (double)count;

        // compute max and min
        if (count < _minDensity)
        {
            _minDensity = count;
        }

        if (count > _maxDensity)
        {
            _maxDensity = count;
        }
    }

    // find all local maximuns and minimuns
    findLocalMinMaxIntervals();

    mean /= (double)_seriesWords.size();
    _mean = mean;
    _standardDeviation = standardDeviation<int, double>(_mean, _density, 0, _density.size());
}

void DensityCurve::insertSymbol(const char *pInWord)
{
    _seriesWords.push_back(pInWord);
    _density.push_back(0);
}

void DensityCurve::getGlobalMinDensities(std::vector<int> &rOutVec)
{
    rOutVec.assign(_globalMinPoints.begin(), _globalMinPoints.end());
}

void DensityCurve::getGlobalMaxDensities(std::vector<int> &rOutVec)
{
    rOutVec.assign(_globalMaxPoints.begin(), _globalMaxPoints.end());
}

void DensityCurve::getLocalMinDensities(std::vector<int> &rOutVec)
{
    rOutVec.assign(_localMinPoints.begin(), _localMinPoints.end());
}

void DensityCurve::getLocalMaxDensities(std::vector<int> &rOutVec)
{
    rOutVec.assign(_localMaxPoints.begin(), _localMaxPoints.end());
}