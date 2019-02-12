#include <boost/algorithm/string.hpp>
#include <vector>
#include <string.h>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

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
                   int &rOutDetectionThreshold,
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

        // get detection threshold
        rOutDetectionThreshold = atoi(pInInputArgs->detectionthreshold().c_str());

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
        int detection_threshold;

        // get arguments from input
        bool loading_status = buildArgs(pInInput, time_series, alphabet, sliding_window_range, paa_size, detection_threshold, debug_status);

        if (!loading_status)
        {
            pOutput->set_output("Invalid time series URL or file...");
            return;
        }

        printf("\n\nReceived request: \n");
        printf("Time Series: %s\n", pInInput->timeseries().c_str());
        printf("sliding_window_range: %s\n", pInInput->slidingwindowsize().c_str());
        printf("alphabet_size: %s\n", pInInput->alphabet().c_str());
        printf("paa_size: %s\n", pInInput->paasize().c_str());
        printf("detection_threshold: %s\n", pInInput->detectionthreshold().c_str());
        printf("debug_status: %s\n\n", pInInput->debugflag().c_str());

        // create an anomaly discovery object
        ErdbAnomalyDiscovery *anomaly_discovery = new ErdbAnomalyDiscovery(alphabet, sliding_window_range, paa_size);

        // insert received time series into this anomaly detector
        for (unsigned int sample = 0; sample < time_series.size(); sample++)
        {
            anomaly_discovery->insertSample(time_series[sample]);
        }

        // for all conditions of sequitur to be applied, generate the density curve statistics, and get detected anomalies
        std::vector<int> detected_anomalies_index;

        // convert anomalies into and output string
        std::string output_string = "";
        anomaly_discovery->getAnomalies(detected_anomalies_index, &output_string, detection_threshold, debug_status);

        printf("\n\nLOG DETECTED ANOMALIES IN SERVER\n\n%s\n\n", output_string.c_str());

        // set output for this service
        pOutput->set_output(output_string.c_str());

        delete anomaly_discovery;
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
