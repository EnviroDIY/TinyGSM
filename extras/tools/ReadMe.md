# TinyGSM Tools<!--! {#page_the_tools} -->

These tool sketches provide utilities for debugging, diagnosing, resetting, and validating TinyGSM modem support.

___

<!--! @if GITHUB -->

- [TinyGSM Tools](#tinygsm-tools)
  - [AT Debug](#at-debug)
  - [Diagnostics](#diagnostics)
  - [Factory Reset](#factory-reset)
  - [Test Build](#test-build)

<!--! @endif -->

<!--! @tableofcontents -->

<!--! @m_footernavigation -->

## AT Debug<!--! {#tools_at_debug} -->

This tool sketch tries to auto-detect the modem baud rate and provides direct AT command passthrough access.

- [The AT debug tool on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/extras/tools/AT_Debug)

<!--! @m_innerpage{tool_at_debug} -->

## Diagnostics<!--! {#tools_diagnostics} -->

This tool sketch runs a series of modem diagnostic checks and prints the results to help troubleshoot connectivity and configuration issues.

- [The diagnostics tool on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/extras/tools/Diagnostics)

<!--! @m_innerpage{tool_diagnostics} -->

## Factory Reset<!--! {#tools_factory_reset} -->

This tool sketch sends commands to reset a supported modem back to factory defaults.

- [The factory reset tool on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/extras/tools/FactoryReset)

<!--! @m_innerpage{tool_factory_reset} -->

## Test Build<!--! {#tools_test_build} -->

This tool sketch is a compile-only validation target used to exercise API coverage and ensure builds succeed; it is not intended for normal runtime use.

- [The test build tool on GitHub](https://github.com/EnviroDIY/TinyGSM/tree/master/extras/tools/test_build)

<!--! @m_innerpage{tool_test_build} -->
