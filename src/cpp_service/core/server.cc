#include <boost/algorithm/string.hpp>
#include <vector>
#include <string.h>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "../piecewise_aggregate_approximation/PiecewiseAggregateApproximation.h"
#include "../anomaly_discovery/ErdbAnomalyDiscovery.h"
#include "../utils/timeSeriesUtils.h"
#include "timeSeriesAnomalyDetection.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

// PROTO_TYPES
using timeSeriesAnomalyDetection::EfficientRuleDensityBasedAnomalyDetection;
using timeSeriesAnomalyDetection::InputParameters;
using timeSeriesAnomalyDetection::OutputString;

using namespace timeSeries;

#define MAX_SAMPLES 1000

class ServiceImpl final : public EfficientRuleDensityBasedAnomalyDetection::Service
{
  public:
    explicit ServiceImpl()
    {
    }

    bool buildArgs(const InputParameters *pInInputArgs,
                   std::vector<double> &rOutTimeSeries,
                   std::vector<std::string> &rOutAlphabet,
                   int &rOutSlidingWindowRange,
                   int &rOutpaaSize,
                   bool &rOutDebugStatus)
    {
        bool loading_status = true;

        // build up time series from arg 0
        loadSeriesURL(pInInputArgs->timeseries().c_str(), rOutTimeSeries, loading_status, false);

        if (!loading_status)
        {
            return loading_status;
        }

        // build up alphabet
        int alphabet_size = atoi(pInInputArgs->alphabet().c_str());
        std::vector<std::string> alphabet_string;
        for (int sample = 0; sample < alphabet_size; sample++)
        {
            rOutAlphabet.push_back(std::to_string(sample));
        }

        // get sliding window range
        rOutSlidingWindowRange = atoi(pInInputArgs->slidingwindowsize().c_str());

        // get paa size
        rOutpaaSize = atoi(pInInputArgs->paasize().c_str());

        // get debug status
        rOutDebugStatus = atoi(pInInputArgs->debugflag().c_str());

        return loading_status;
    }

    // SERVICE_API
    Status detectAnomalies(ServerContext *context, const InputParameters *pInInput, OutputString *pOutput) override
    {
        std::vector<double> time_series;
        std::vector<std::string> alphabet;
        int sliding_window_range;
        int paa_size;
        bool debug_status;

        // get arguments from input
        bool loading_status = buildArgs(pInInput, time_series, alphabet, sliding_window_range, paa_size, debug_status);

        if (!loading_status || time_series.size() < 20)
        {
            pOutput->set_timeseries("[[\"point\", \"value\"],[0,0]]");
            pOutput->set_density("[[\"point\", \"value\"],[0,0]]");
            pOutput->set_normalized("[[\"point\", \"value\"],[0,0]]");
            pOutput->set_inverted("[[\"point\", \"value\"],[0,0]]");
            return Status::OK;
        }

        printf("\n\nReceived request: \n");
        printf("Time Series: %s\n", pInInput->timeseries().c_str());
        printf("sliding_window_range: %s\n", pInInput->slidingwindowsize().c_str());
        printf("alphabet_size: %s\n", pInInput->alphabet().c_str());
        printf("paa_size: %s\n", pInInput->paasize().c_str());
        printf("debug_status: %s\n\n", pInInput->debugflag().c_str());

        // create an anomaly discovery object
        ErdbAnomalyDiscovery *anomaly_discovery = new ErdbAnomalyDiscovery(alphabet, sliding_window_range, paa_size);

        // generate valid subsequence, reduce dimentionality
        if(time_series.size() > MAX_SAMPLES) 
        {
            PiecewiseAggregateApproximation paa(&time_series, MAX_SAMPLES);
            std::vector<double> *pApproximatedSeries = paa.getApproximatedSeries();
            time_series.assign(pApproximatedSeries->begin(), pApproximatedSeries->end());
        }

        // guarantee that the parameters will be optimal
        if(sliding_window_range <= 10 || sliding_window_range >= MAX_SAMPLES || sliding_window_range >= time_series.size()) 
        {
            sliding_window_range = 10;
            printf("Using new sliding window size: %d\n\n", sliding_window_range);
        }

        // insert samples for the detection algorithm
        for (unsigned int sample = 0; sample < time_series.size(); sample++)
        {
            anomaly_discovery->insertSample(time_series[sample]);
        }

        // tell the anomaly discovery method to generate the density curve
        DensityCurve *curve = anomaly_discovery->getDensityCurve();

        // get google chart readable json strings for the original time series, density curve, normalized density curve, and inverted normalized density curve
        std::string time_series_json_string = "";
        std::string density_curve_json_string = "";
        std::string normalized_density_curve_json_string = "";
        std::string normalized_inverted_density_curve_json_string = "";
        curve->getJson(time_series_json_string, density_curve_json_string, normalized_density_curve_json_string, normalized_inverted_density_curve_json_string);

        // set output for this service
        pOutput->set_timeseries(time_series_json_string.c_str());
        pOutput->set_density(density_curve_json_string.c_str());
        pOutput->set_normalized(normalized_density_curve_json_string.c_str());
        pOutput->set_inverted(normalized_inverted_density_curve_json_string.c_str());

        // free curve
        delete curve;

        // delete anomaly_discovery;
        return Status::OK;
    }
};

static void RunServer()
{
    std::string server_address("0.0.0.0:7055");
    ServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char **argv)
{
    RunServer();
    return 0;
}
