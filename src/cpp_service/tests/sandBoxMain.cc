#include <vector>
#include <string>

#include "../utils/timeSeriesUtils.h"
#include "../anomaly_discovery/ErdbAnomalyDiscovery.h"

using namespace timeSeries;

int main(int argc, char *argv[])
{
    // arg 1 - series file
    // arg 2 - window
    // arg 3 - alphabet size
    // arg 4 - paa size

    printf("Series URL: %s\n", argv[1]);
    printf("Sliding Window: %s\n", argv[2]);
    printf("Alphabet Size: %s\n", argv[3]);
    printf("Paa Size: %s\n", argv[4]);
    printf("Detection Threshold: %s\n", argv[5]);

    // load time series
    std::vector<double> time_series;
    bool load_status = false;
    loadSeriesURL(argv[1], time_series, load_status, false);

    if (!load_status)
    {
        printf("Cannot load this time series...\n");
        return 1;
    }

    // load window size
    int sliding_window_size = atoi(argv[2]);

    // load alphabet size
    int alphabet_size = atoi(argv[3]);

    // load paa size
    int paa_size = atoi(argv[4]);

    // load detection threshold
    int detection_threshold = atoi(argv[5]);

    // generate alphabet
    std::vector<std::string> alphabet;
    for (int alphabet_letter = 0; alphabet_letter < alphabet_size; alphabet_letter++)
    {
        alphabet.push_back(std::to_string(alphabet_letter));
    }

    // call for anomaly detection, it will print the output
    ErdbAnomalyDiscovery anomaly_discovery(alphabet, sliding_window_size, paa_size);

    // iterate over the provided time series
    for (unsigned int sample = 0; sample < time_series.size(); sample++)
    {
        anomaly_discovery.insertSample(time_series[sample]);
    }

    DensityCurve *curve = anomaly_discovery.getDensityCurve();
    curve->saveCsv("DensityCurve.csv");

    return 0;
}