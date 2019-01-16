#include "unit_tests/anomalyDetectionUnitTestsImpl.h"
#include "unit_tests/densityCurveUnitTestsImpl.h"
#include "unit_tests/saxUnitTestsImpl.h"
#include "unit_tests/sequiturUnitTestsImpl.h"

int main(int argc, char *argv[])
{
    // variable to store the last test results
    bool result = false;

    // series related variables
    double frequency = 0.1;
    double amplitude = 20.0;
    double phase = 0.0;
    int samples = 90;
    int ppa_size = 3;
    int sliding_window_size = 9;
    std::vector<std::string> alphabet = {"a", "b", "c"};
    std::vector<double> senoid_time_series;
    generateSenoidSeries(senoid_time_series, samples, frequency, amplitude, phase);

    result = saxWordGenerationUnitTest(senoid_time_series, sliding_window_size, ppa_size, alphabet, "");
    result = saxWordGenerationSlidingWindowUnitTest(senoid_time_series, sliding_window_size, ppa_size, alphabet, "");
    result = sequiturGrammarGenerationUnitTest(senoid_time_series, sliding_window_size, ppa_size, alphabet, "");
    result = densityCurveGenerationSlidingWindowUnitTest(senoid_time_series, sliding_window_size, ppa_size, alphabet, "");
    result = anomalyDiscoveryErdbUnitTest(senoid_time_series, sliding_window_size, ppa_size, alphabet, "");
}