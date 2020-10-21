/**
 * \file
 *
 * \brief SAM Direct Memory Access Controller driver example.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

/**
 * \mainpage SAM Extensible Direct Memory Access Controller Example - memory to memory transfer
 *
 * \section Purpose
 * This example demonstrates how to configure and use XDMA controller
 * for memory to memory data buffer transfers.
 *
 * \section Requirements
 * This example can be used with SAM evaluation kits that have a XDMA
 * Controller.
 *
 * \section Description
 * This example transfers data in one RAM buffer to another RAM buffer.
 * It uses single block with multiple microblock transfer with interrupt mode.
 * After transfer is done, the transferred data will be verified.
 *
 * \section Main Files
 * - xdmac.c: Direct Memory Access Controller driver
 * - xdmac.h: Direct Memory Access Controller driver header file
 * - xdmac_example.c: Direct Memory Access Controller example application
 *
 * \section Compilation Information
 * This software is written for GNU GCC and IAR Embedded Workbench&reg;
 * for Atmel&reg;. Other compilers may or may not work.
 *
 * \section Usage
 * -# Build the program and download it into the evaluation board.
 * -# On the computer, open and configure a terminal application
 *    (e.g., HyperTerminal on Microsoft&reg; Windows&reg;) with these settings:
 *   - 115200 baud
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application.
 * -# In the terminal window, the following text should appear.
 *    (values depend on the board and the chip used):
 *    \code
 *     -- XDMAC Example --
 *     -- xxxxxx-xx
 *     -- Compiled: xxx xx xxxx xx:xx:xx --
 *
 *     > Test OK. \endcode
 */

#include <asf.h>
#include <conf_board.h>
#include <conf_clock.h>
#include <string.h>

/** Micro-block length for single transfer  */
#define OSCCLK      12e6
#define PLLACLK     (OSCCLK * 50)
#define HCLK        (PLLACLK / 2)
#define MCK         (HCLK / 2)
#define PCK6_DIV    6
#define PCK6        (MCK / PCK6_DIV)
#define F_TIMER     1e3

/** XDMA channel used in this example. */
#define DMA_CH_USART1_CH    0
#define DMA_CH_USART1_TX    9

/** XDMA channel configuration. */
static xdmac_channel_config_t xdmac_channel_cfg;

/* DMA transfer done flag. */
volatile uint32_t g_xfer_done = 0;
//
// Buffer to be sent over serial port
volatile char output_string[100];
// Other variables to go into output_string
volatile float a = 1.02342353;
volatile float b = 0.7874378934;
volatile float c = 0.2342343;
// XDMAC variables
volatile int cubc = 50;
volatile int cbc = 2;
volatile int counter = 0;

/**
 * \brief Configure the console UART.
 */
static void configure_console(void) {
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.charlength = CONF_UART_CHAR_LENGTH,
		.paritytype = CONF_UART_PARITY,
		.stopbits = CONF_UART_STOP_BITS,
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}
//
void XDMAC_setup(void) {
    //
    // USART1 TX DMA Channel
    pmc_enable_periph_clk(ID_XDMAC);
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CDA = (uint32_t) &USART1->US_THR;	    // Destination address is US_THR
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CC = (XDMAC_CC_TYPE_PER_TRAN |		// memory to peripheral transcation
        XDMAC_CC_MBSIZE_SINGLE |                                                    // Memory burst size 1
        XDMAC_CC_SAM_INCREMENTED_AM |                                               // Source address increment (buffer)
        XDMAC_CC_DAM_FIXED_AM |												        // Dest address fixed (SPI TDR)
        XDMAC_CC_DSYNC_MEM2PER |                                                    // Mem to peripheral transfer
        XDMAC_CC_CSIZE_CHK_1 |                                                      // Chunk size 1
        XDMAC_CC_DWIDTH_BYTE |												        // 8 bit data width
        XDMAC_CC_SIF_AHB_IF0 |                                                      // See datasheet section 18.2.3 for system bus connections
        XDMAC_CC_DIF_AHB_IF1 |
        XDMAC_CC_PERID(DMA_CH_USART1_TX));
    XDMAC->XDMAC_GE |= 1 << DMA_CH_USART1_CH;										//enable TX channel
}
//
void XDMAC_transfer(void) {
//    USART1->US_THR = 80;
    sprintf(output_string, "\x1B[2J\x1B[H %2i\n\rVt = %6.4f \n\rP = %6.4f \n\rQ = %6.4f \n\r", counter, a, b, c);
    uint32_t xdmaint = XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CIS;				// Clear any pending interrupts for this channel
    uint32_t num_samples = strlen(output_string);
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CSA = (uint32_t) &output_string[0];      // Source address is TX buffer
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CNDC = 0;								// Clear a bunch of registers we need to clear before enabling DMA
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CDS_MSP = 0;
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CSUS = 0;
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CDUS = 0;
    //
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CUBC = 50;					        // Set microblock size to be the size requested
    XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CBC = 0;						    // Single microblock at a time
    //
    xdmac_enable_interrupt(XDMAC, DMA_CH_USART1_CH);
    xdmac_channel_enable_interrupt(XDMAC, DMA_CH_USART1_CH, XDMAC_CIE_BIE);			// End of Block interrupt is enabled
#ifdef CONF_BOARD_ENABLE_CACHE
	SCB_CleanDCache();
#endif
    XDMAC->XDMAC_GE = (XDMAC_GE_EN0);
    while((XDMAC->XDMAC_CHID[DMA_CH_USART1_CH].XDMAC_CIS));
}
//
void XDMAC_Handler(void) {
    uint32_t dma_status;
    dma_status = xdmac_channel_get_interrupt_status(XDMAC, DMA_CH_USART1_CH);
    (void) dma_status;
    NVIC_ClearPendingIRQ(XDMAC_IRQn);
    NVIC_DisableIRQ(XDMAC_IRQn);

#ifdef CONF_BOARD_ENABLE_CACHE
	SCB_InvalidateDCache();
#endif
}
//
void TC1_Handler (void) {
    uint32_t TC1_status_reg = TC0->TC_CHANNEL[1].TC_SR;     // Clear flag
    (void) TC1_status_reg;
    XDMAC_transfer();
    // Changing some variables so we can see a result on the serial terminal
    counter++;
    if (counter < 10) c = 0.1;
    if (counter > 10) c = 0.2;
    if (counter > 20) c = 0.3;
    if (counter > 30) c = 0.4;
}
//
void timer_setup(void) {
    //
    // TC1 to trigger XDMAC to transmit sine waves
    PMC->PMC_PCER0 |= (1 << ID_TC1);								// Enable clock to TC1 (TC0, Ch1)
    TC0->TC_CHANNEL[1].TC_CCR = (TC_CCR_SWTRG | TC_CCR_CLKEN);
    // This timer is clocked from SLCK and half timer value for 1 Hz refresh
    TC0->TC_CHANNEL[1].TC_CMR = (TC_CMR_ACPC(1) | TC_CMR_ACPA(2) | TC_CMR_WAVE
        | TC_CMR_WAVSEL(2) | TC_CMR_EEVT(3) | TC_CMR_TCCLKS_TIMER_CLOCK5);
    TC0->TC_CHANNEL[1].TC_RA = 0x0001;
    TC0->TC_CHANNEL[1].TC_RB = 0x0002;
    TC0->TC_CHANNEL[1].TC_RC = 0xFFFF;
    TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
    //
    TC0->TC_BCR = 1;
}
//
void interrupt_setup(void) {
    Disable_global_interrupt();
    NVIC_ClearPendingIRQ(TC1_IRQn);
    NVIC_EnableIRQ(TC1_IRQn);
    NVIC_ClearPendingIRQ(XDMAC_IRQn);
    NVIC_EnableIRQ(XDMAC_IRQn);
    Enable_global_interrupt();
}
//
int main(void) {
	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Configure console on USART1 */
	configure_console();
    //
    XDMAC_setup();
    timer_setup();
    //
	/* Output example information */
	printf("\n\r-- XDMAC Example --\n\r");
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    //
	/* Initialize and enable DMA controller */
	pmc_enable_periph_clk(ID_XDMAC);
    //
	printf("> Test OK.\n\r");
    //
    interrupt_setup();
    //
	while (1);
}
