#include <stdio.h>
#include <stdlib.h>

#include "timeSeriesUtils.h"
#include "utilsImpl.h"
#include "utils.h"

using namespace timeSeries;

void timeSeries::getSubsequence(std::vector<double> &rInSeries, std::vector<double> &rOutSubsequence, const int start, const int range)
{
    rOutSubsequence.assign(rInSeries.begin() + start, rInSeries.begin() + start + range);
}

void timeSeries::zNorm(std::vector<double> &rInSubSequence, std::vector<double> &rOutNormalizedSubSequence)
{
    // let the output subsequence to be empty if no input subsequence is specified
    if (rInSubSequence.size() == 0)
    {
        return;
    }

    // ensure that the out normalized subsequence will be empty
    rOutNormalizedSubSequence.clear();

    // calculate statistics of this subsequence to allow normalization
    int start = 0;
    int range = rInSubSequence.size();

    double mean = 0.0;
    for (int i = 0; i < range; i++)
    {
        mean += rInSubSequence[i];
    }
    mean = mean / (double)rInSubSequence.size();

    //double mean_val = mean<double, double>(rInSubSequence, start, range);
    //double deviation_val = standardDeviation<double, double>(mean_val, rInSubSequence, start, range);
    double dist_mean_sum = 0.0;
    for (int i = 0; i < range; i++)
    {
        dist_mean_sum += std::pow(mean - rInSubSequence[i], 2.0);
    }
    double deviation_val = std::sqrt(dist_mean_sum / ((double)rInSubSequence.size() - 1.0));
    
    // normalize each sample
    for (unsigned int sample = 0; sample < rInSubSequence.size(); sample++)
    {
        rOutNormalizedSubSequence.push_back((rInSubSequence[sample] - mean) / deviation_val);
    }
}

void timeSeries::generateSenoidSeries(std::vector<double> &rOutSeries,
                                      const int size,
                                      const double frequency,
                                      const double amplitude,
                                      const double phase)
{
    for (int i = 0; i < size; i++)
    {
        double angular_frequency = 2.0 * M_PI * frequency;
        double wave_position = amplitude * sin(angular_frequency * (double)i + phase);
        rOutSeries.push_back(wave_position);
    }
}

void timeSeries::generateRandomSeries(std::vector<double> &rOutSeries, const int size)
{
    for (int i = 0; i < size; i++)
    {
        rOutSeries.push_back((double)rand() / RAND_MAX);
    }
}

void timeSeries::insertNoise(std::vector<double> &rOutSeries, const int start, const int end)
{
    for (int i = start; i < end; i++)
    {
        rOutSeries[i] += (double)rand() / RAND_MAX;
    }
}

void timeSeries::saveSeriesCsv(const char *pInOutputFile, std::vector<double> &rInSeries)
{
    FILE *pOutSeriesFile = fopen(pInOutputFile, "w");
    fprintf(pOutSeriesFile, "sample,value\n");
    for (unsigned int sample = 0; sample < rInSeries.size(); sample++)
    {
        fprintf(pOutSeriesFile, "%d,%f\n", sample, rInSeries[sample]);
    }
    fclose(pOutSeriesFile);
}

void timeSeries::printSeries(std::vector<double> &rInSubSequence)
{
    for (unsigned int sample = 0; sample < rInSubSequence.size(); sample++)
    {
        printf("%f ", rInSubSequence[sample]);
    }
    printf("\n");
}

void timeSeries::loadSeriesCsv(const char *pInOutputFile, std::vector<double> &rOutSeries, const bool hasHeader)
{
    // guarantee that the output series vector will be clean
    rOutSeries.clear();

    FILE *pSeriesFile = fopen(pInOutputFile, "r");
    char *readed_line = nullptr;
    size_t line_length = 0;

    if (hasHeader)
    {
        getline(&readed_line, &line_length, pSeriesFile);
    }

    while ((getline(&readed_line, &line_length, pSeriesFile)) != -1)
    {
        double series_sample = atof(readed_line);
        rOutSeries.push_back(series_sample);
    }

    fclose(pSeriesFile);
}

void timeSeries::loadSeriesURL(const char *pInUrl, std::vector<double> &rOutSeries, bool &rOutStatus, const bool hasHeader)
{
    rOutStatus = true;
    std::string tmp_series_file_name = "";
    std::string error_msg = "";
    CURLcode error_code = loadUrlFile(error_msg, pInUrl, tmp_series_file_name);

    if (error_code)
    {
        // TODO:handle error
        rOutStatus = false;
    }
    else
    {
        loadSeriesCsv(tmp_series_file_name.c_str(), rOutSeries, hasHeader);
    }

    // remove temp file
    if (remove(tmp_series_file_name.c_str()))
    {
        std::string msg = "loadSeriesURL(): Error deleting temporary file: ";
        // TODO:handle error
    }
}