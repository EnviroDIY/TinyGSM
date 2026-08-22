# Multiple Modules<!--! {#example_multiple_modules} -->

This example shows using two different modems together in the same code.
The key thing to notice is that instead of defining the modem type and including the TinyGsmClient.h file, it includes the specific modem headers.
You cannot use the typedefs `TinyGsm`, `TinyGsmClient`, and `TinyGsmClientSecure` for the modem and the clients.
You must use the full class names instead.

_______

<!--! @section example_multiple_modules_code The Complete Code -->

<!--! @include{lineno} more/Multiple_Modules/Multiple_Modules.ino -->
