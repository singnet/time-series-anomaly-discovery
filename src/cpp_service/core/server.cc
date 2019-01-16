#include <boost/algorithm/string.hpp>
#include <vector>
#include <string.h>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "../session_manager/SessionManager.h"
#include "timeSeriesAnomalyDetection.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

// PROTO_TYPES
using timeSeriesAnomalyDetection::ServiceDefinition;
using timeSeriesAnomalyDetection::InputParameters;
using timeSeriesAnomalyDetection::OutputString;

using namespace timeSeries;

class ServiceImpl final : public ServiceDefinition::Service
{

  public:
    explicit ServiceImpl()
    {
    }

    void buildArgs(const InputParameters *pInInputArgs,
                   std::vector<double> &rOutTimeSeries,
                   std::vector<std::string> &rOutAlphabet,
                   int &rOutSlidingWindowRange,
                   int &rOutPpaSize,
                   bool &rOutDebugStatus)
    {     
        // build up time series from arg 0
        std::vector<std::string> time_series_string;
        boost::split(time_series_string, pInInputArgs->timeseries(), [](char c) { return c == ' '; });
        for (int sample = 0; sample < time_series_string.size(); sample++)
        {
            rOutTimeSeries.push_back(atoi(time_series_string[sample].c_str()));
        }

        // build up alphabet
        std::vector<std::string> alphabet_string;
        boost::split(alphabet_string, pInInputArgs->alphabet(), [](char c) { return c == ' '; });
        for (int sample = 0; sample < alphabet_string.size(); sample++)
        {
            rOutAlphabet.push_back(alphabet_string[sample]);
        }

        // get sliding window range
        rOutSlidingWindowRange = atof(pInInputArgs->slidingwindowsize().c_str());

        // get ppa size
        rOutPpaSize = atof(pInInputArgs->ppasize().c_str());

        // get debug status
        rOutDebugStatus = atoi(pInInputArgs->debugflag().c_str());
    }

    // SERVICE_API
    Status run(ServerContext *context, const InputParameters *pInInput, OutputString *pOutput) override
    {
        std::vector<double> time_series;
        std::vector<std::string> alphabet;
        int sliding_window_range;
        int ppa_size;
        bool debug_status;

        // get arguments from input
        buildArgs(pInInput, time_series, alphabet, sliding_window_range, ppa_size, debug_status);

        // create an anomaly discovery object
        ErdbAnomalyDiscovery anomaly_discovery(alphabet, sliding_window_range, ppa_size);

        // insert received time series into this anomaly detector
        anomaly_discovery.insertTimeSeries(time_series);

        // for all conditions of sequitur to be applied, generate the density curve statistics, and get detected anomalies
        std::vector<int> detected_anomalies_index;

        // convert anomalies into and output string
        std::string output_string = "";
        anomaly_discovery.getAnomalies(detected_anomalies_index, &output_string, debug_status);

        // set output for this service
        pOutput->set_output(output_string.c_str());
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
