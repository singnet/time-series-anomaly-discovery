#include "utils.h"

#include <functional>
#include <queue>
#include <random>
#include <stdio.h>

static size_t writeData(void *pPtr, size_t size, size_t nmemb, void *pStream)
{
    size_t written = fwrite(pPtr, size, nmemb, (FILE *)pStream);
    return written;
}

class CompareIndexPairGreaterThan
{
  public:
    bool operator()(std::pair<int, double> &rInA, std::pair<int, double> &rInB)
    {
        if (rInA.second > rInB.second)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class CompareIndexPairLessThan
{
  public:
    bool operator()(std::pair<int, double> &rInA, std::pair<int, double> &rInB)
    {
        if (rInA.second < rInB.second)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

CURLcode timeSeries::loadUrlFile(std::string &errorMessage, const std::string &rUrl, std::string &rOutputFileName)
{
    // create temp file to hold the URL contents
    CURL *pCurlHandle;
    char tmp_name[] = "/tmp/URL_LOADED_FILE_XXXXXX";
    FILE *pFile = fdopen(mkstemp(tmp_name), "w");

    // load URL and save its contents into the file
    curl_global_init(CURL_GLOBAL_ALL);
    pCurlHandle = curl_easy_init();
    curl_easy_setopt(pCurlHandle, CURLOPT_URL, rUrl.c_str());
    curl_easy_setopt(pCurlHandle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(pCurlHandle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, pFile);

    // if unable to handle url set error status, does not override file error
    CURLcode errorCode = curl_easy_perform(pCurlHandle);
    if (errorCode) {
        errorMessage.assign(curl_easy_strerror(errorCode));
    }

    // close the file
    fclose(pFile);
    curl_easy_cleanup(pCurlHandle);
    curl_global_cleanup();

    // set the outputfile name
    rOutputFileName = std::string(tmp_name);

    // return the generated file
    return errorCode;
}

double timeSeries::euclideanDistance(std::vector<double> &rInSubSequenceA, std::vector<double> &rInSubSequenceB)
{
    if (rInSubSequenceA.size() != rInSubSequenceB.size())
    {
        return -1.0;
    }

    double dist = 0.0;
    for (unsigned int sample = 0; sample < rInSubSequenceA.size(); sample++)
    {
        dist += std::pow(rInSubSequenceA[sample] - rInSubSequenceB[sample], 2.0);
    }
    dist = std::sqrt(dist) / (double)rInSubSequenceA.size();

    return dist;
}

double timeSeries::euclideanDistanceBetweenWords(std::string &rInSubSequenceA, std::string &rInSubSequenceB, std::map<char, int> &rInAlphabet)
{
    if (rInSubSequenceA.size() != rInSubSequenceB.size())
    {
        return -1.0;
    }

    double dist = 0.0;
    for (unsigned int symbol = 0; symbol < rInSubSequenceA.size(); symbol++)
    {
        char symbol_a = rInSubSequenceA.at(symbol);
        char symbol_b = rInSubSequenceB.at(symbol);

        double letter_a_val = (double)rInAlphabet[symbol_a];
        double letter_b_val = (double)rInAlphabet[symbol_b];

        dist += std::pow(letter_a_val - letter_b_val, 2.0);
    }
    dist = std::sqrt(dist) / (double)rInSubSequenceA.length();

    return dist;
}

bool timeSeries::isSelfMatch(const int startA, const int startB, const int windowSize)
{
    int end_a = startA + windowSize;
    int end_b = startB + windowSize;

    if (end_b < startA ||
        startB > end_a)
    {
        return false;
    }
    return true;
}

void timeSeries::discordDiscoveryBruteForce(std::vector<double> &rInSeries,
                                            const int subSequenceSize,
                                            std::vector<int> &rOutDiscords)
{
    // stores discords rankings
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, CompareIndexPairLessThan> discords;

    // go over all subsequences
    for (int i = 0; i < rInSeries.size(); i += subSequenceSize)
    {
        std::vector<double> subsequence;
        subsequence.assign(rInSeries.begin() + i, rInSeries.begin() + i + subSequenceSize);

        double nearest_neighbour_dist = std::numeric_limits<double>::max();
        // find nearest neighbour
        for (int j = 0; j < rInSeries.size(); j++)
        {
            // get only non-self matches
            if (!isSelfMatch(i, j, subSequenceSize))
            {
                // candidate subsequence
                std::vector<double> candidate;
                candidate.assign(rInSeries.begin() + j, rInSeries.begin() + j + subSequenceSize);

                // get subsequence
                double dist = euclideanDistance(subsequence, candidate);

                // check if it is a better neares neighbour
                if (dist < nearest_neighbour_dist)
                {
                    nearest_neighbour_dist = dist;
                }
            }
        }

        // check if nearest neighbour is the highest one from all subsequences
        discords.push(std::pair<int, double>(i, nearest_neighbour_dist));
    }

    // setup the output discords
    rOutDiscords.clear();
    while (discords.size() > 0)
    {
        rOutDiscords.push_back(discords.top().first);
        discords.pop();
    }
}