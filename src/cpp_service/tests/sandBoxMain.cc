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

    // load time series
    std::vector<double> time_series;
    loadSeriesCsv(argv[1], time_series, false);

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

    // for all conditions of sequitur to be applied, generate the density curve statistics, and get detected anomalies
    std::vector<int> detected_anomalies_index;
    anomaly_discovery.getAnomalies(detected_anomalies_index, nullptr, detection_threshold, false);

    for (unsigned int anomaly = 0; anomaly < detected_anomalies_index.size(); anomaly++)
    {
        printf("%d ", detected_anomalies_index[anomaly]);
    }
    printf("\n");

    DensityCurve curve;
    anomaly_discovery.getDensityCurve(curve);

    curve.saveCsv("DensityCurve");

    return 0;
}