
#ifndef same70q21_uxart_pins_h
#define same70q21_uxart_pins_h
/*
 * Auto-generated code based on pinmux documentation for:
 * SAME70_Q_144
 * */

/**
* UARTs
*/

/*  UART0_TXD i.e. pin = UTXD0  */
#define UART0_TXD_GPIO      PIO_PA10_IDX
#define UART0_TXD_FLAGS     IOPORT_MODE_MUX_A

/*  UART0_RXD i.e. pin = URXD0  */
#define UART0_RXD_GPIO      PIO_PA9_IDX
#define UART0_RXD_FLAGS     IOPORT_MODE_MUX_A

/*  UART1_TXD i.e. pin = UTXD1  */
#ifdef UART1_TXD_ALT_1
#  define UART1_TXD_GPIO    PIO_PA4_IDX
#  define UART1_TXD_FLAGS   IOPORT_MODE_MUX_C
#endif //UART1_TXD_ALT_1
#ifdef UART1_TXD_ALT_2
#  define UART1_TXD_GPIO    PIO_PA6_IDX
#  define UART1_TXD_FLAGS   IOPORT_MODE_MUX_C
#endif //UART1_TXD_ALT_2
#ifdef UART1_TXD_ALT_3
#  define UART1_TXD_GPIO    PIO_PD26_IDX
#  define UART1_TXD_FLAGS   IOPORT_MODE_MUX_D
#endif //UART1_TXD_ALT_3

/*  UART1_RXD i.e. pin = URXD1  */
#define UART1_RXD_GPIO      PIO_PA5_IDX
#define UART1_RXD_FLAGS     IOPORT_MODE_MUX_C

/*  UART2_TXD i.e. pin = UTXD2  */
#define UART2_TXD_GPIO      PIO_PD26_IDX
#define UART2_TXD_FLAGS     IOPORT_MODE_MUX_C

/*  UART2_RXD i.e. pin = URXD2  */
#define UART2_RXD_GPIO      PIO_PD25_IDX
#define UART2_RXD_FLAGS     IOPORT_MODE_MUX_C

/*  UART3_TXD i.e. pin = UTXD3  */
#ifdef UART3_TXD_ALT_1
#  define UART3_TXD_GPIO    PIO_PD30_IDX
#  define UART3_TXD_FLAGS   IOPORT_MODE_MUX_A
#endif //UART3_TXD_ALT_1
#ifdef UART3_TXD_ALT_2
#  define UART3_TXD_GPIO    PIO_PD31_IDX
#  define UART3_TXD_FLAGS   IOPORT_MODE_MUX_B
#endif //UART3_TXD_ALT_2

/*  UART3_RXD i.e. pin = URXD3  */
#define UART3_RXD_GPIO      PIO_PD28_IDX
#define UART3_RXD_FLAGS     IOPORT_MODE_MUX_A


/**
* USARTs
*/

/*  USART0_TXD i.e. pin = TXD0  */
#define USART0_TXD_GPIO     PIO_PB1_IDX
#define USART0_TXD_FLAGS    IOPORT_MODE_MUX_C

/*  USART0_RXD i.e. pin = RXD0  */
#define USART0_RXD_GPIO     PIO_PB0_IDX
#define USART0_RXD_FLAGS    IOPORT_MODE_MUX_C

/*  USART1_TXD i.e. pin = TXD1  */
#define USART1_TXD_GPIO     PIO_PB4_IDX
#define USART1_TXD_FLAGS    IOPORT_MODE_MUX_D

/*  USART1_RXD i.e. pin = RXD1  */
#define USART1_RXD_GPIO     PIO_PA21_IDX
#define USART1_RXD_FLAGS    IOPORT_MODE_MUX_A

/*  USART2_TXD i.e. pin = TXD2  */
#define USART2_TXD_GPIO     PIO_PD16_IDX
#define USART2_TXD_FLAGS    IOPORT_MODE_MUX_B

/*  USART2_RXD i.e. pin = RXD2  */
#define USART2_RXD_GPIO     PIO_PD15_IDX
#define USART2_RXD_FLAGS    IOPORT_MODE_MUX_B

/*  USART3_TXD i.e. pin = TXD3  */

/*  USART3_RXD i.e. pin = RXD3  */

#endif //same70q21_uxart_pins_h
