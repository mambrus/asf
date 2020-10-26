#ifndef uxart_pins_h
#define uxart_pins_h
/* SAM pins and pinmux group for UART and USART for supported variants */

#if defined(__same70q21__) || defined(__same70q21a__) | defined(__same70q21b__)
#  include "same70q21_uxart_pins.h"
#elif defined(__same70n21__) || defined(__same70n21a__) | defined(__same70n21b__)
#  include "same70n21_uxart_pins.h"
#elif defined(__same70j21__) || defined(__same70j21a__) | defined(__same70j21b__)
#  include "same70j21_uxart_pins.h"
#else
#  error uxart pins not defined for PART
#endif


#endif //uxart_pins_h
