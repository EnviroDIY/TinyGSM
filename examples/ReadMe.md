# TinyGSM Examples<!--! {#page_the_examples} -->

These example programs demonstrate how to use the TinyGSM library.

___

<!--! @if GITHUB -->

- [TinyGSM Examples](#tinygsm-examples)
  - [All Functions](#all-functions)
  - [AWS IoT Core](#aws-iot-core)
  - [Blynk Client](#blynk-client)
  - [File Download](#file-download)
  - [HTTP Client](#http-client)
  - [HTTPS Client](#https-client)
  - [MQTT Client](#mqtt-client)
  - [Web Client](#web-client)
  - [Hologram Dash](#hologram-dash)
  - [Multiple Connections](#multiple-connections)
  - [SIM800 SSL Set Cert](#sim800-ssl-set-cert)

<!--! @endif -->

<!--! @tableofcontents -->

<!--! @m_footernavigation -->

## All Functions<!--! {#examples_all_functions} -->

This example demonstrates and exercises a broad set of TinyGSM modem, network, client, SMS, call, and utility functions in a single sketch for capability testing.

- [The all functions example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/AllFunctions)

<!--! @subpage example_all_functions -->

## AWS IoT Core<!--! {#examples_aws_iot_core} -->

This example shows how to provision certificates on supported modems and connect to AWS IoT Core over MQTT with TLS.

- [The AWS IoT Core example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/AWS_IoTCore)

<!--! @subpage example_aws_iot_core -->

## Blynk Client<!--! {#examples_blynk_client} -->

This example connects a TinyGSM modem to Blynk so a device can exchange data and be controlled from the Blynk app.

- [The Blynk client example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/BlynkClient)

<!--! @subpage example_blynk_client -->

## File Download<!--! {#examples_file_download} -->

This example downloads a remote file in chunks and validates data integrity using CRC32 checksums.

- [The file download example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/FileDownload)

<!--! @subpage example_file_download -->

## HTTP Client<!--! {#examples_http_client} -->

This example uses ArduinoHttpClient with TinyGSM to perform HTTP requests for REST-style API interactions.

- [The HTTP client example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/HttpClient)

<!--! @subpage example_http_client -->

## HTTPS Client<!--! {#examples_https_client} -->

This example uses ArduinoHttpClient with TinyGSM to perform HTTPS requests over TLS on supported modems.

- [The HTTPS client example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/HttpsClient)

<!--! @subpage example_https_client -->

## MQTT Client<!--! {#examples_mqtt_client} -->

This example connects to an MQTT broker and demonstrates publish/subscribe messaging using TinyGSM and PubSubClient.

- [The MQTT client example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/MqttClient)

<!--! @subpage example_mqtt_client -->

## Web Client<!--! {#examples_web_client} -->

This example builds and sends a raw HTTP request over a TinyGSM client socket to illustrate manual web request handling.

- [The web client example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/WebClient)

<!--! @subpage example_web_client -->

## Hologram Dash<!--! {#examples_hologram_dash} -->

This example targets Hologram Dash workflows by entering passthrough mode and sending network requests through TinyGSM.

- [The Hologram Dash example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/more/Hologram_Dash)

<!--! @subpage example_hologram_dash -->

## Multiple Connections<!--! {#examples_multiple_connections} -->

This example opens and manages multiple simultaneous client connections to different servers in one sketch.

- [The multiple connections example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/more/Multiple_Connections)

<!--! @subpage example_multiple_connections -->

## SIM800 SSL Set Cert<!--! {#examples_sim800_ssl_set_cert} -->

This example uploads SSL certificate material to SIM8xx modems so secure TLS connections can be established.

- [The SIM800 SSL set cert example on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/examples/more/SIM800_SslSetCert)

<!--! @subpage example_sim800_ssl_set_cert -->
