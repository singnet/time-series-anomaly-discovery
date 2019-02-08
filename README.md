![singnetlogo](docs/assets/singnet-logo.jpg 'SingularityNET')

[author-home]: http://alysson.thegeneralsolution.com
[singularitynet-home]: https://www.singularitynet.io
[contribution-guidelines]: https://github.com/singnet/wiki/blob/master/guidelines/CONTRIBUTING.md
[cpp-tutorial]: https://github.com/singnet/wiki/tree/master/tutorials/howToWriteCPPService
[setup-script]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/setup.sh
[service_confi_file]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/service_conf

[sequitur_docs]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/docs/sequitur.md
[sax_docs]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/docs/sax.md
[paa_docs]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/docs/paa.md
[densitycurve_docs]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/docs/densitycurve.md
[erdb_docs]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/docs/erdb.md
[users_guide]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/docs/usersguide.md

[singnet_service_tutorial]:https://dev.singularitynet.io/tutorials/publish/

[![CircleCI](https://circleci.com/gh/singnet/time-series-anomaly-discovery.svg?style=svg)](https://circleci.com/gh/singnet/time-series-anomaly-discovery)

# Time Series Anomaly Discovery Service to SingularityNET

This project contains a [C++ service][cpp-tutorial] to discover time series anomalies in real-time.

This service allows to detect anomalies in time series as accomplished by [[1]](#anomalies_detection_general), based on grammar compression. It contains [SingularityNet][singularitynet-home] implementation of the following algorithms and methods.

[Symbolic Aggregate Approximation](#sax) - Discretization method for continuous signals [[2]](#sax).

[Piecewise Aggregate Approximation](#paa) - Series approximation and reduction method [[3]](#paa).

[Sequitur](#sequitur) - Context Free Grammar Generator [[4]](#sequitur).

[Density Curve](#anomalies_detection_general) - Density curve generator based on context free grammar rules [[1]](#anomalies_detection_general).

[Efficient, rule density-based anomaly discovery](#anomalies_detection_general) - Detect anomalies on a generated density curve, based on a hill-climbing inspired algorithm [[1]](#anomalies_detection_general).

For a detailed explanation about how this service works see the [users guide][users_guide] and [Sigularity Net Service Tutorial][singnet_service_tutorial].

## Table of contents

<!--ts-->
   * [Getting Started](#getting-started)
      * [Step 1 - Building](#step-1-building)
      * [Step 2 - Running local GRPC server](#step-2-running-local-grpc-server)
      * [Step 3 - Calling GRPC server locally](#step-3-Calling-grpc-server-locally)
   * [Parameters Explanation](#parameters-explanation)
   * [Running with DAEMON](#running-with-daemon)
   * [Publishing and Performing a Test Request Through the Daemon](#publishing-and-performing-a-test-request-through-the-daemon)
   * [Docker](#docker)
   * [More stuff](#more-stuff)
      * [Publishing](#publishing)
      * [Performing a Test Request](#performing-a-test-request)
      * [Creating a CircleCi Configuration File](#creating-a-circleci-configuration-file)
   * [Contributing and Reporting Issues](#contributing-and-reporting-issues)
   * [Author](#author)
   * [References](#references)
   * [License](#license)
<!--te-->

## Getting Started

For the sake of simplicity, this service provides a 'setup.sh' script. This script can be used to install all dependencies, compile the source, perform tests, run the service, and publish it. This service source was tested on Ubuntu 18.04 and requires at least the C++ GRPC library and the DAMEON installed to be built and executed. 

In order to install all dependencies, use the following command.

```
sudo ./setup.sh -i
```

Basically, this command will install the GRPC c++ library, the DAEMON to handle requests, and the snet cli to manage the blockchain related stuff.

<details><summary>Click here to see the commands called by './setup.sh -i'</summary><p>
    
```
# update packages list
apt-get update;\

# try to install all recommended software
apt-get install -y cxxtest nlohmann-json-dev build-essential autoconf libtool pkg-config \
                   libgflags-dev libgtest-dev clang libc++-dev git curl nano \
                   wget libudev-dev libusb-1.0-0-dev nodejs npm python3 python3-pip libboost-all-dev;\

# try upgrade pip
pip install --upgrade pip; \

# install GRPC library
cd /;\
git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc; \
cd grpc; \
git submodule update --init; \
make; \
make install; \
cd third_party/protobuf; \
make install; \
cd /;\

# install snet daemon to call for services
mkdir snet-daemon; \
cd snet-daemon; \
wget -q https://github.com/singnet/snet-daemon/releases/download/v0.1.5/snet-daemon-v0.1.5-linux-amd64.tar.gz; \
tar -xvf snet-daemon-v0.1.5-linux-amd64.tar.gz; \
mv ./snet-daemon-v0.1.5-linux-amd64/snetd /usr/bin/snetd; \
cd ..; \
rm -rf snet-daemon; \

# install cli to handle services publishing and other related operations
cd /opt; \
git clone https://github.com/singnet/snet-cli; \
cd snet-cli; \
./scripts/blockchain install; \
pip3 install -e .; \
```
</p></details>

## Step 1 - Building

To build this project's source and perform tests, run the following command in the project's root directory.


```
./setup.sh -b
```

Besides building the source, this command will check the responsiveness of real requests to this service GRPC server.

<details><summary>Click here to see the commands called by './setup.sh -b'</summary><p>
    
```
# build source
make clean; make

# run tests
./bin/deployTests
```
</p></details>

## Step 2 - Running local GRPC server

After building the source, run the GPRC server with following command. It will run the server in background mode. 

```
./bin/server.out &
```

## Step 3 - Calling GRPC server locally

After running the server, run the client with the command presented bellow. With the presented example input parameters, the algorithms should be able to detect simulated spikes in the input time series. A spike is represented by the number 1000 while a normal sample is represented by the number 1.

```
./bin/client.out "1 1 1 1 1 1000 1 1 1 1 1 1000 1 1 1 1 1 1000 1 1 1 1 1 1000 1 1 1 1 1 100" "a b c d e f g h i j" 4 2 0
```

Expected output:

```
4 5 10 11 16 17 22 23
```

The presented output represents the indexes in which anomalies were detected in the original time series beginning at index 0.

## Parameters Explanation

* arg [1]
   * Type = String numbers separated by spaces.
   * Represents: The time series in which anomalies will be detected.
* arg [2]
   * Type = String symbols separated by spaces.
   * Represents: Alphabet used to discretizise the paa apporximation.
* arg [3]
   * Type = Integer
   * Represents: Sliding window size used to create the time series symbols to build the free context grammar through the Sequitur algorithm.
* arg [4]
   * Type = Integer
   * Represents: Piecewise Aggregate Approximation defining the number of sub-samples that will be generated for each sliding window position.
* arg [5]
   * Type = Integer
   * Represents: Debug printing flag, where 0 is false and 1 is true.

## Running with DAEMON

In order to build and get this service running with the DAEMON, run the [setup.sh][setup-script] script located in the project's root directory with the *-r* flag. This flag will force the source to be built, perform tests, run the localhost GRPC server, and the DAEMON to handle requests to this service.


```
./setup.sh -r
```

It is important to note that the DAEMON listen to outside requests and the GRPC will only listen requests at the localhost address. Both are initially configurated to listen at 54.203.198.53:7090 and 0.0.0.0:7055, respectively. For more info about how to configure the service see the [service configuration file][service_confi_file].

<details><summary>Click here to see the commands called by './setup.sh -r'</summary><p>
    
```
# create snet daemon snetd.config.json file
createDeamonConfig

# run daemon in background
snetd --config snetd.config.json &

# run the service local server in background
./bin/server &
```
</p></details>

## Publishing and Performing a Test Request Through the Daemon

In order to publish, run services, configure and call the DAEMON, we higly recommend you to see the [Sigularity Net Service Tutorial][singnet_service_tutorial] for a more detailed explanation about those processes.

## Docker
This project provides two docker images located at the Dockerfiles folder. One is a basic C++ service based Dockerfile to allow to build basic docker images ready to run C++ services. On the other hand, the other one incorporates the service source, DAEMON, and a final CMD command to run the service directly with the docker 'run' command. 

In order to build a basic C++ service-ready image use the following command in the project's root directory.

```
docker build -t cpp_basic_image:latest -< ./Dockerfiles/CppServiceBaseDockerfile
```

In order to build a full image containing this service source and a CMD command that runs the service with the docker 'run' command, perform the following command in the projects root directory.

```
docker build -t time-series-anomaly-discovery:latest -< ./Dockerfiles/TimeSeriesAnomalyDiscovery
```

## More Stuff

Here are more commands provided by the 'setup' script.

<details><summary>Click here to see the the 'More Stuff' commands.</summary><p>

### Publishing

To publish variants of this service, call the following.


```
./setup.sh -p
```

This command will publish the service with the specified information in [service configuration file][service_confi_file] located in the project's root directory. Just remember that in order to publish a service, you firstly need a valid identity and [service configuration file][service_confi_file]. We higly recommend you to see the [Sigularity Net Service Tutorial][singnet_service_tutorial] for a more detailed explanation about the publication process.

<details><summary>Click here to see the commands called by './setup.sh -p'</summary><p>
    
```
# delete service before trying to publish it
snet service delete $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR -y

# create metadata json for this service with its name and the wallet that will receive money
snet service metadata-init src/service_spec $SERVICE_NAME_VAR $WALLET_VAR

# set the price to use this service
snet service metadata-set-fixed-price $PRICE_VAR

# set the local port to access this service server
snet service metadata-add-endpoints https://$HOST_IP_ADDRESS_VAR:$SERVICE_DAEMON_PORT_VAR

# publish the service at the specified organization
snet service publish $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR -y
```
</p></details>

### Performing a test request

To perform a test request, run the following command.

```
./setup.sh -e
```
This will do a request to this service DAEMON, already published at snet, with the default input parameter and DAEMON address specified in the [service configuration file][service_confi_file].

*A call for this service is free, however in order for this to work you need to have a valid identity/account.*

For more info regarding service calls, publication, DAEMON, how to create identities/accounts, and the blockchain, see [Sigularity Net Service Tutorial][singnet_service_tutorial].

<details><summary>Click here to see the commands called by './setup.sh -e'</summary><p>
    
```
echo
echo "Running a test call to this service daemon with the specified data in the 'service_conf' file."

# open a channel with the deposited amount to call for this service
CHANNEL_TIME_OUT=11000000
RESPONSE="$(snet channel open-init $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR $PRICE_VAR $CHANNEL_TIME_OUT -y)"

# get channel ID from the last substring obtained from the RESPONSE variable
RESPONSES=( $RESPONSE )
LENGTH=${#RESPONSES[@]}
CHANNEL_ID_INDEX=$(($LENGTH - 1))
CHANNEL_ID=${RESPONSES[$(($LENGTH - 1))]}

# call for the created service
DAEMON_RESPONSE="$(snet client call "$CHANNEL_ID" "$PRICE_VAR" "$HOST_IP_ADDRESS_VAR:$SERVICE_DAEMON_PORT_VAR" "$TEST_CALL_METHOD_VAR" "$TEST_CALL_INPUT_VAR")"

# print response from daemon
echo
echo "Daemon response:"
echo
echo $DAEMON_RESPONSE
echo
```
</p></details>

### Creating CircleCi Config File

In order to create a circleci configuration file for this project use the following command.

```
./setup.sh -c
```

This will create a [SingularityNET][singularitynet-home] circleci configuration file based on the [service configuration file][service_confi_file] located in the project's root directory.

</p></details>

## Contributing and Reporting Issues

Please read our [guidelines][contribution-guidelines] before
submitting issues or pull requests. 

## Author

[Alysson Ribeiro da Silva][author-home] - *Maintainer* - [SingularityNET][singularitynet-home]

## References

<a name=anomalies_detection_general>[1]</a> Senin, P., Lin, J., Wang, X., Oates, T., Gandhi, S., Boedihardjo, A.P., Chen, C., & Frankenstein, S. (2015). Time series anomaly     discovery with grammar-based compression. EDBT.

<a name=sax>[2]</a> Chaw Thet Zan and Hayato Yamana. 2016. An improved symbolic aggregate approximation distance measure based on its statistical features. In Proceedings of the 18th International Conference on Information Integration and Web-based Applications and Services (iiWAS '16). ACM, New York, NY, USA, 72-80. DOI: https://doi.org/10.1145/3011141.3011146

<a name=paa>[3]</a> Huorong Ren, Xiujuan Liao, Zhiwu Li, and Abdulrahman Ai-Ahmari. 2018. Anomaly detection using piecewise aggregate approximation in the amplitude domain. Applied Intelligence 48, 5 (May 2018), 1097-1110. DOI: https://doi.org/10.1007/s10489-017-1017-x

<a name=sequitur>[4]</a> Craig G. Nevill-Manning and Ian H. Witten. 1997. Identifying hierarchical structure in sequences: a linear-time algorithm. J. Artif. Int. Res. 7, 1 (September 1997), 67-82.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
