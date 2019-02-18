#include <string.h>
#include <stdio.h>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "timeSeriesAnomalyDetection.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

// PROTO_TYPES
using timeSeriesAnomalyDetection::InputParameters;
using timeSeriesAnomalyDetection::OutputString;
using timeSeriesAnomalyDetection::EfficientRuleDensityBasedAnomalyDetection;

class ServiceClient
{

  public:
    ServiceClient(std::shared_ptr<Channel> channel) : stub_(EfficientRuleDensityBasedAnomalyDetection::NewStub(channel))
    {
    }

    void run(int argc, char **argv)
    {
        if (argc < 5){
            printf("\n\nTime series anomaly discovery service\n\n");
            printf("input:\n\t<time series url string> values: url string containing a time series csv file");
            printf("\n\t<sliding window size> values: number greater than 0 ex: \"10\"");
            printf("\n\t<alphabet> size: size of the alphabet ex: \"1\" or \"2\", etc");
            printf("\n\t<paa size> values: number greater than 0 ex: \"2\"");
            printf("\n\t<debug flag> values: \"1\" or \"0\"\n\n");
            exit(-1);
        }

        InputParameters input;
        OutputString output;

        // set input parameter here
        input.set_timeseries(argv[1]);
        input.set_slidingwindowsize(argv[2]);
        input.set_alphabet(argv[3]);
        input.set_paasize(argv[4]);
        input.set_debugflag(argv[5]);

        ClientContext context;
        Status status = stub_->detectAnomalies(&context, input, &output);
        if (status.ok())
        {
            // save received responses into file
            FILE *out_curve = fopen("output_time_series.json", "w");
            FILE *out_densities = fopen("output_density_curve.json", "w");
            FILE *out_norm_densities = fopen("output_normalized_density_curve.json", "w");
            FILE *out_norm_inv_densities = fopen("output_normalized_inverted_density_curve.json", "w");

            // save received responses into file
            fprintf(out_curve, "%s", output.timeseries().c_str());
            fprintf(out_densities, "%s", output.density().c_str());
            fprintf(out_norm_densities, "%s", output.normalized().c_str());
            fprintf(out_norm_inv_densities, "%s", output.inverted().c_str());

            fclose(out_curve);
            fclose(out_densities);
            fclose(out_norm_densities);
            fclose(out_norm_inv_densities);

            // operations performed well for this call
            printf("OK");
        }
        else
        {
            std::cout << "detectAnomalies rpc failed." << std::endl;
        }
    }

  private:
    std::unique_ptr<EfficientRuleDensityBasedAnomalyDetection::Stub> stub_;
};

int main(int argc, char **argv)
{
    ServiceClient guide(grpc::CreateChannel("localhost:7055", grpc::InsecureChannelCredentials()));
    guide.run(argc, argv);
    return 0;
}
