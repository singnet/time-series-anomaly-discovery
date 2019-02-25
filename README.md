![singnetlogo](docs/assets/singnet-logo.jpg 'SingularityNET')

[author-home]: http://alysson.thegeneralsolution.com
[singularitynet-home]: https://www.singularitynet.io
[contribution-guidelines]: https://github.com/singnet/wiki/blob/master/guidelines/CONTRIBUTING.md
[cpp-tutorial]: https://github.com/singnet/wiki/tree/master/tutorials/howToWriteCPPService
[setup-script]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/setup.sh
[service_confi_file]: https://github.com/singnet/time-series-anomaly-discovery/blob/master/service_conf
[users_guide]: ./docs/usersguide.md
[docker_tutorial]: https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-18-04
[docker_files]: ./Dockerfiles
[singnet_service_tutorial]:https://dev.singularitynet.io/tutorials/publish/
[cxx_test_home]: https://cxxtest.com/

[![CircleCI](https://circleci.com/gh/singnet/time-series-anomaly-discovery.svg?style=svg)](https://circleci.com/gh/singnet/time-series-anomaly-discovery)

# Time Series Anomaly Discovery Service to SingularityNET

This project contains a [C++ service][cpp-tutorial] to discover time series anomalies in real-time.

This service allows to detect anomalies in time series as accomplished by [[1]](#anomalies_detection_general), based on grammar compression. It contains [SingularityNet][singularitynet-home] implementation of the following algorithms and methods.

[Symbolic Aggregate Approximation](#sax) - Discretization method for continuous signals [[2]](#sax).

[Piecewise Aggregate Approximation](#paa) - Series approximation and reduction method [[3]](#paa).

[Sequitur](#sequitur) - Context Free Grammar Generator [[4]](#sequitur).

[Density Curve](#anomalies_detection_general) - Density curve generator based on context free grammar rules [[1]](#anomalies_detection_general).

[Efficient, rule density-based anomaly discovery](#anomalies_detection_general) - Detect anomalies on a generated density curve, based on a hill-climbing inspired algorithm [[1]](#anomalies_detection_general).

For a detailed explanation about how this service works see the [user's guide][users_guide] and [Sigularity Net Service Tutorial][singnet_service_tutorial].

## Table of contents

<!--ts-->
   * [Getting Started](#getting-started)
      * [Docker](#docker)
      * [Local Instalation](#local-installation)
   * [Next Steps](#next-steps)
      * [Step 1 - Building](#step-1-building)
      * [Step 2 - Running local GRPC server](#step-2-running-local-grpc-server)
      * [Step 3 - Calling GRPC server locally](#step-3-Calling-grpc-server-locally)
   * [Parameters Explanation](#parameters-explanation)
   * [Running with DAEMON](#running-with-daemon)
   * [Publishing and Performing a Test Request Through the Daemon](#publishing-and-performing-a-test-request-through-the-daemon)
   * [Contributing and Reporting Issues](#contributing-and-reporting-issues)
   * [Author](#author)
   * [References](#references)
   * [License](#license)
<!--te-->

## Getting Started

If you are not familiarized with docker you can proceed with the local instalation, however we higly recommend for this project to be built inside a docker container due to the fact that some of its dependencies can not be easily removed from your system (e.g. GRPC C++ library). If you choose to use docker and know nothing about it, follow this [docker related tutorial][docker_tutorial] before proceeding.

### Docker

This project provides two docker files located in the [Dockerfiles][docker_files] folder. One of them allows you to build clean C++ GRPC ready images  to run and develop C++ services. On the other hand, the other one incorporate the service source, all the [Sigularity Net Service][singnet_service_tutorial] stuff to interact with the block chain, and a final CMD command to run the service directly with the docker 'run' command. 

In order to build a basic C++ service-ready image use the following command in the project's root directory.

```
docker build -t cpp_basic_image:latest -< ./Dockerfiles/CppServiceBaseDockerfile
```

In order to build a full image containing this service source and a CMD command that runs the service with the docker 'run' command, perform the following command in the projects root directory.

```
docker build -t time-series-anomaly-discovery:latest -< ./Dockerfiles/TimeSeriesAnomalyDiscovery
```

### Local installation

Since this service requires the GRPC C++ library installed, we higly recommend you to read the following warning message with caution and think about the consequences before proceeding with a local instalation.

<b>WARNING: After installing with make install there is no easy way to uninstall, which can cause issues if you later want to remove the grpc and/or protobuf installation or upgrade to a newer version.</b>

For the sake of simplicity, this repository also contains a 'setup.sh' script. This script can be used to install all dependencies, compile the source, perform tests, run the service, and publish it. This service source was tested on Ubuntu 18.04 and requires at least the C++ GRPC library and the DAMEON installed to be built and executed. 

In order to install all dependencies, use the following command.

```
sudo ./setup.sh -i
```

Basically, this command will install the GRPC c++ library, the DAEMON to handle requests, and the snet cli to manage the blockchain related stuff.

<details><summary>Click here to see the commands called by './setup.sh -i'</summary><p>
    
```
    apt-get update;\
    apt-get install -y libcurl4-gnutls-dev cxxtest nlohmann-json-dev build-essential autoconf libtool pkg-config \
                       libgflags-dev libgtest-dev clang libc++-dev git curl nano \
                       wget libudev-dev libusb-1.0-0-dev nodejs npm python3 python3-pip libboost-all-dev;\

    # try upgrade pip
    pip install --upgrade pip; \

    # install GRPC
    cd /;\
    git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc; \
    cd grpc; \
    git submodule update --init; \
    make; \
    make install; \
    cd third_party/protobuf; \
    make install; \
    cd /;\

    # install daemon
    mkdir snet-daemon; \
    cd snet-daemon; \
    wget -q https://github.com/singnet/snet-daemon/releases/download/v0.1.6/snet-daemon-v0.1.6-linux-amd64.tar.gz; \
    tar -xvf snet-daemon-v0.1.6-linux-amd64.tar.gz; \
    mv ./snet-daemon-v0.1.6-linux-amd64/snetd /usr/bin/snetd; \
    cd ..; \
    rm -rf snet-daemon; \

    # install cli
    cd /opt; \
    git clone https://github.com/singnet/snet-cli; \
    cd snet-cli; \
    ./scripts/blockchain install; \
    pip3 install -e .; \
```
</p></details>

# Next steps

The next steps can be performed either locally or inside a [docker][docker_tutorial] container, choose what best fits for you.

## Step 1 - Building

To build this project's source and perform tests, run the following command in the project's root directory.


```
./setup.sh -b
```

Besides building the source, this command will check the responsiveness of real requests to this service GRPC server through integration tests and it will also perform [CXX][cxx_test_home] unit tests.

<details><summary>Click here to see the commands called by './setup.sh -b'</summary><p>
    
```
    # build source
    make clean; make

    echo "Running unit tests..."
    ./bin/release_cxxUnitTestsRunner.out

    echo "Running integration tests..."
    ./bin/release_integrationTests.out
```
</p></details>

## Step 2 - Running local GRPC server

After building the source, run the GPRC server with following command. It will run the server in background mode. 

```
./bin/release_server.out &
```

## Step 3 - Calling GRPC server locally

After running the server, run the client with the command presented bellow. With the presented example input parameters, the algorithms should be able to detect anomalies in a ECG exam.

```
./bin/release_client.out https://raw.githubusercontent.com/singnet/time-series-anomaly-discovery/master/resources/time_series/ecg0606_1.csv 100 5 10 0
```

For this call the service will output the following files.

* output_time_series.json
   - the original time series json string.
* output_density_curve.json
   - the generated density curve for the specified input parameters representing detected anomalies.
* output_normalized_density_curve.json
   - the normalized density curve representing detected anomalies.
* output_normalized_inverted_density_curve.json
   - the inverted normalized density curve representing detected anomalies.

For more information about anomaly detection through grammar compression please see the [user's guide][users_guide].

## Parameters Explanation

* arg [1]
   * Type = String
   * Represents: An URL containing a time series csv file.
* arg [2]
   * Type = Integer
   * Represents: Sliding window size used to create the time series symbols to build the free context grammar through the Sequitur algorithm.
* arg [3]
   * Type = Integer
   * Represents: Alphabet size.
* arg [4]
   * Type = Integer
   * Represents: Piecewise Aggregate Approximation defining the number of sub-samples that will be generated for each sliding window position.
* arg [5]
   * Type = Integer
   * Represents: Debug printing flag, where 0 is false and 1 is true.

## Running with DAEMON

This part involves blockchain stuff, consequently we higly recommend you to check our [Sigularity Net Service Tutorial][singnet_service_tutorial] to better understand it.

In order to build and get this service running with the [SingularityNET][singularitynet-home] DAEMON, to handle blockchain related stuff, run the [setup.sh][setup-script] script located in the project's root directory with the *-r* flag. This flag will force the source to be built, perform tests, run the localhost GRPC server, and the DAEMON to handle requests to this service and interact with the blockchain.


```
./setup.sh -r
```

It is important to note that the DAEMON listen to outside requests and the GRPC will only listen requests at the localhost address. Both are initially configurated to listen at 54.203.198.53:7090 and 0.0.0.0:7055, respectively. In order to configure the service see the [service configuration file][service_confi_file].

<details><summary>Click here to see the commands called by './setup.sh -r'</summary><p>
    
```
    # run daemon for the ropsten
    snetd --config ./snetd_configs/snetd.ropsten.json & 
    
    # run server
    ./bin/release_server.out
```
</p></details>

## Publishing and Performing a Test Request Through the Daemon

In order to publish, run services, configure and call the DAEMON, we higly recommend you to see the [Sigularity Net Service Tutorial][singnet_service_tutorial] for a more detailed explanation about those processes since they involve a large variety of technologies.

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
