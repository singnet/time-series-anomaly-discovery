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
using timeSeriesAnomalyDetection::ServiceDefinition;

class ServiceClient
{

  public:
    ServiceClient(std::shared_ptr<Channel> channel) : stub_(ServiceDefinition::NewStub(channel))
    {
    }

    void run(int argc, char **argv)
    {
        if (argc < 6){
            printf("\n\nTime series anomaly discovery service\n\n");
            printf("input:\n\t<time series string> values: numbers separated by spaces ex: \"1 2 3 1 2 100 200 1\"");
            printf("\n\t<alphabet> values: letters separated by spaces ex: \"a b c d e f g h i j\"");
            printf("\n\t<sliding window size> values: number greater than 0 ex: \"10\"");
            printf("\n\t<ppa size> values: number greater than 0 ex: \"2\"");
            printf("\n\t<debug flag> values: \"1\" or \"0\"\n\n");
            exit(-1);
        }

        InputParameters input;
        OutputString output;

        // set input parameter here
        input.set_timeseries(argv[1]);
        input.set_alphabet(argv[2]);
        input.set_slidingwindowsize(argv[3]);
        input.set_ppasize(argv[4]);
        input.set_debugflag(argv[5]);

        ClientContext context;
        Status status = stub_->run(&context, input, &output);
        if (status.ok())
        {
            // print received output if any
            printf("%s", output.output().c_str());
        }
        else
        {
            std::cout << "run rpc failed." << std::endl;
        }
    }

  private:
    std::unique_ptr<ServiceDefinition::Stub> stub_;
};

int main(int argc, char **argv)
{
    ServiceClient guide(grpc::CreateChannel("localhost:7055", grpc::InsecureChannelCredentials()));
    guide.run(argc, argv);
    return 0;
}
