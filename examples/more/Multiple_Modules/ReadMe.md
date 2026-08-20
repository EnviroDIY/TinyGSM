# Multiple Modules<!--! {#example_multiple_modules} -->

This example shows using two different modems together in the same code.
The key thing to notice is that instead of defining the modem type and including the TinyGsmClient.h file, it includes the specific modem headers.
The modem and the clients cannot be referred to using the typedefs `TinyGsm`, `TinyGsmClient`, and `TinyGsmClientSecure` but must use their full class names.

_______

<!--! @section example_multiple_modules_code The Complete Code -->

<!--! @include{lineno} more/Multiple_Modules/Multiple_Modules.ino -->
