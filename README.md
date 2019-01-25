![singnetlogo](docs/assets/singnet-logo.jpg 'SingularityNET')

[author-home]: http://alysson.thegeneralsolution.com
[singularitynet-home]: https://www.singularitynet.io
[contribution-guidelines]: https://github.com/singnet/wiki/blob/master/guidelines/CONTRIBUTING.md
[cpp-tutorial]: https://github.com/singnet/wiki/tree/master/tutorials/howToWriteCPPService
[setup-script]: https://github.com/Ophien/TSAD-service/blob/master/setup.sh
[service_confi_file]: https://github.com/Ophien/TSAD-service/blob/master/service_conf

[sequitur_docs]: https://github.com/Ophien/TSAD-service/blob/master/docs/sequitur.md
[sax_docs]: https://github.com/Ophien/TSAD-service/blob/master/docs/sax.md
[paa_docs]: https://github.com/Ophien/TSAD-service/blob/master/docs/paa.md
[densitycurve_docs]: https://github.com/Ophien/TSAD-service/blob/master/docs/densitycurve.md
[erdb_docs]: https://github.com/Ophien/TSAD-service/blob/master/docs/erdb.md
[users_guide]: https://github.com/Ophien/TSAD-service/blob/master/docs/usersguide.md

[![CircleCI](https://circleci.com/gh/singnet/time-series-anomaly-discovery.svg?style=svg)](https://circleci.com/gh/singnet/time-series-anomaly-discovery)

# Time Series Anomaly Discovery Service to SingularityNET

This repository contains the [C++ service][cpp-tutorial] for Grammar-Based Compression Time Series Anomaly Discovery.

## Getting Started

This service allows to detect anomalies in time series as accomplished by [[1]](#anomalies_detection_general), based on grammar compression. It contains [SingularityNet][singularitynet-home] implementation of the following algorithms and methods.

[Symbolic Aggregate Approximation][sax_docs] - Discretization method for continuous signals [[2]](#sax).

[Piecewise Aggregate Approximation][paa_docs] - Series approximation and reduction method [[3]](#paa).

[Sequitur][sequitur_docs] - Context Free Grammar Generator [[4]](#sequitur).

[Density Curve][densitycurve_docs] - Density curve generator based on context free grammar rules [[1]](#anomalies_detection_general).

[Efficient, rule density-based anomaly discovery][erdb_docs] - Detect anomalies on a generated density curve, based on a hill-climbing inspired algorithm [[1]](#anomalies_detection_general).

For a detailed explanation about how this service works see the [users guide][users_guide].

## Compile

To compile this project's source and perform integration tests, run one of the following commands in the project's root directory.

```
./setup.sh -c
```

<details><summary>Click here to see the commands called by './setup.sh -c'</summary><p>
    
```
# build source
make clean; make

# run tests
./bin/deployTests
```
</p></details>


Besides building the source, this command will check the responsiveness of real requests to this service GRPC server.

## Running

In order to compile and get this service running locally, run the [setup.sh][setup-script] script located in the project's root directory with the *-r* flag. This flag will force the source to be built, run integration tests, the localhost GRPC server, and the daemon to handle requests to this service.

```
./setup.sh -r
```

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


The daemon and the GRPC are initially configurated to listen at 0.0.0.0:7090 and 0.0.0.0:7055, respectively. For more info about how to configure the service see the [service configuration file][service_confi_file].

## Publishing

To publish this service call the following.

```
./setup.sh -p
```

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

# publish the service at the especified organization
snet service publish $ORGANIZATION_TO_PUBLISH_VAR $SERVICE_NAME_VAR -y
```
</p></details>


This command will publish the service with the specified information located at the [service configuration file][service_confi_file] located in the project's root directory.

## Performing deploy test

To perform a deploy test, run the following command.

```
./setup.sh -e
```

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


This will call the daemon for this service with the default input parameter to the service. Both, daemon port and service input parameter, are specified in the [service configuration file][service_confi_file].

## Contributing and Reporting Issues

Please read our [guidelines][contribution-guidelines] before
submitting issues or pull requests. 

## Author

[Alysson Ribeiro da Silva][author-home] - *Maintainer* - [SingularityNET][singularitynet-home]

### References

<a name=anomalies_detection_general>[1]</a> Senin, P., Lin, J., Wang, X., Oates, T., Gandhi, S., Boedihardjo, A.P., Chen, C., & Frankenstein, S. (2015). Time series anomaly     discovery with grammar-based compression. EDBT.

<a name=sax>[2]</a> Chaw Thet Zan and Hayato Yamana. 2016. An improved symbolic aggregate approximation distance measure based on its statistical features. In Proceedings of the 18th International Conference on Information Integration and Web-based Applications and Services (iiWAS '16). ACM, New York, NY, USA, 72-80. DOI: https://doi.org/10.1145/3011141.3011146

<a name=paa>[3]</a> Huorong Ren, Xiujuan Liao, Zhiwu Li, and Abdulrahman Ai-Ahmari. 2018. Anomaly detection using piecewise aggregate approximation in the amplitude domain. Applied Intelligence 48, 5 (May 2018), 1097-1110. DOI: https://doi.org/10.1007/s10489-017-1017-x

<a name=sequitur>[4]</a> Craig G. Nevill-Manning and Ian H. Witten. 1997. Identifying hierarchical structure in sequences: a linear-time algorithm. J. Artif. Int. Res. 7, 1 (September 1997), 67-82.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
