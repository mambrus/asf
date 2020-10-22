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
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,,
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
 *
 *     USART1 on SAME70Q21 is mapped on pins:
 *          TXD1 -> PB4
 *          RXD1 <- PA21
 */

#define USART2_RXD_GPIO   PIO_PD15_IDX
#define USART2_TXD_GPIO   PIO_PD16_IDX
#define USART2_RXD_FLAGS  IOPORT_MODE_MUX_B
#define USART2_TXD_FLAGS  IOPORT_MODE_MUX_B

#include <asf.h>
#include <conf_board.h>
#include <conf_clock.h>
#include <string.h>
#include "manchester.h"

/**
 * \brief Set peripheral mode for IOPORT pins.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param port IOPORT port to configure
 * \param masks IOPORT pin masks to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_port_peripheral_mode(port, masks, mode) \
	do {\
		ioport_set_port_mode(port, masks, mode);\
		ioport_disable_port(port, masks);\
	} while (0)

/**
 * \brief Set peripheral mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 */
#define ioport_set_pin_peripheral_mode(pin, mode) \
	do {\
		ioport_set_pin_mode(pin, mode);\
		ioport_disable_pin(pin);\
	} while (0)

/**
 * \brief Set input mode for one single IOPORT pin.
 * It will configure port mode and disable pin mode (but enable peripheral).
 * \param pin IOPORT pin to configure
 * \param mode Mode masks to configure for the specified pin (\ref ioport_modes)
 * \param sense Sense for interrupt detection (\ref ioport_sense)
 */
#define ioport_set_pin_input_mode(pin, mode, sense) \
	do {\
		ioport_set_pin_dir(pin, IOPORT_DIR_INPUT);\
		ioport_set_pin_mode(pin, mode);\
		ioport_set_pin_sense_mode(pin, sense);\
	} while (0)



/** XDMA channel used in this example. */
#define CONF_XDMAC_CH    0

/** XDMA channel configuration. */
/*static xdmac_channel_config_t xdmac_channel_cfg; */

/* DMA transfer done flag. */
volatile uint32_t g_xfer_done = 0;

/* Tick counter */
volatile unsigned counter;

/* Ethernet data puffers */
#define MTU 1518
#define MIP_USART_TX_BUFFER_LENGTH MTU
static uint16_t g_mipTxBuffer[MIP_USART_TX_BUFFER_LENGTH / 2];

void XDMAC_setup(void);
void XDMAC_transfer(void);
void timer_setup(void);
void interrupt_setup(void);

/* Conditionally print using stdio. Note that packet driver is mutually
 * exclusive via HW */

//#define USE_PRINTF
#ifdef USE_PRINTF
#  define PRINTF(...) printf(__VA_ARGS__)
#else
#  define PRINTF(...)
#endif

/**
 * \brief Configure the console UART.
 */
static void configure_console(void)
{
    /* If defined, this is done in board_init in init.c */
#ifndef CONF_BOARD_UART_CONSOLE
	ioport_set_pin_peripheral_mode(CONF_UART_RXD_GPIO, CONF_UART_RXD_FLAGS);
    /* See sam/utils/cmsis/same70/include/component/matrix.h */
	MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4; /* (CCFG_SYSIO) PB4 or TDI Assignment */
	ioport_set_pin_peripheral_mode(CONF_UART_TXD_GPIO, CONF_UART_TXD_FLAGS);
#endif //CONF_BOARD_UART_CONSOLE

    static usart_serial_options_t uart_serial_options = {
        .baudrate = CONF_UART_BAUDRATE,
        .charlength = CONF_UART_CHAR_LENGTH,
        .paritytype = CONF_UART_PARITY,
        .stopbits = CONF_UART_STOP_BITS,
    };

    /* Configure console UART. */
    sysclk_enable_peripheral_clock(CONF_UART_ID);
#ifdef USE_PRINTF
    stdio_serial_init(CONF_UART, &uart_serial_options);
#else
    usart_serial_init(CONF_UART, &uart_serial_options);
#endif
}

/**
 * USART1 TX DMA Channel
 */
void XDMAC_setup(void)
{

    pmc_enable_periph_clk(ID_XDMAC);
    /* *INDENT-OFF* */
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CDA =
        (uint32_t)&CONF_UART->US_THR;      /* Destination address is US_THR */

    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CC = (
        XDMAC_CC_TYPE_PER_TRAN |        /* memory to peripheral transcation */
        XDMAC_CC_MBSIZE_SINGLE |        /* Memory burst size 1 */
        XDMAC_CC_SAM_INCREMENTED_AM |   /* Source address increment (buffer) */
        XDMAC_CC_DAM_FIXED_AM |         /* Dest address fixed (SPI TDR) */
        XDMAC_CC_DSYNC_MEM2PER |        /* Mem to peripheral transfer */
        XDMAC_CC_CSIZE_CHK_1 |          /* Chunk size 1 */
        XDMAC_CC_DWIDTH_BYTE |          /* 8 bit data width */
        XDMAC_CC_SIF_AHB_IF0 |          /* See datasheet section 18.2.3 */
                                        /* for system bus connections */
        XDMAC_CC_DIF_AHB_IF1 |
        XDMAC_CC_PERID
        (CONF_PERID_UART_TX));
    /* *INDENT-ON* */
    XDMAC->XDMAC_GE |= 1 << CONF_XDMAC_CH;   /*enable TX channel */
}

void XDMAC_transfer(void)
{
/*    USART1->US_THR = 80; */

    /* Clear any pending interrupts for this channel */
    uint32_t xdmaint = XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CIS;
    (void)xdmaint;              /* Warning silence */

    /*uint32_t num_samples = strlen(output_string); */
    /* Source address is TX buffer */
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CSA =
        (uint32_t)(uintptr_t) & g_mipTxBuffer[0];

    /* Clear a bunch of registers we need to clear before enabling DMA */
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CNDC = 0;
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CDS_MSP = 0;
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CSUS = 0;
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CDUS = 0;

    /* Set microblock size to be the size requested */
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CUBC = MTU;
    /* Single microblock at a time */
    XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CBC = 0;

    xdmac_enable_interrupt(XDMAC, CONF_XDMAC_CH);
    /* End of Block interrupt is enabled */
    xdmac_channel_enable_interrupt(XDMAC, CONF_XDMAC_CH, XDMAC_CIE_BIE);
#ifdef CONF_BOARD_ENABLE_CACHE
    SCB_CleanDCache();
#endif
    XDMAC->XDMAC_GE = (XDMAC_GE_EN0);
    while ((XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CIS)) ;
}

void XDMAC_Handler(void)
{
    uint32_t dma_status;
    dma_status = xdmac_channel_get_interrupt_status(XDMAC, CONF_XDMAC_CH);

    NVIC_ClearPendingIRQ(XDMAC_IRQn);
    NVIC_DisableIRQ(XDMAC_IRQn);
    if (dma_status & XDMAC_CIS_BIS) {
        g_xfer_done = 1;
    }
}

void TC1_Handler(void)
{
    uint32_t TC1_status_reg = TC0->TC_CHANNEL[1].TC_SR; /* Clear flag */
    (void)TC1_status_reg;
    XDMAC_transfer();
    /* Changing some variables so we can see a result on the serial terminal */
    counter++;
}

void timer_setup(void)
{
    /* TC1 to trigger XDMAC to transmit sine waves */
    PMC->PMC_PCER0 |= (1 << ID_TC1);    /* Enable clock to TC1 (TC0, Ch1) */
    TC0->TC_CHANNEL[1].TC_CCR = (TC_CCR_SWTRG | TC_CCR_CLKEN);

    /* This timer is clocked from SLCK and half timer value for 1 Hz refresh */
    TC0->TC_CHANNEL[1].TC_CMR = (TC_CMR_ACPC(1) | TC_CMR_ACPA(2) | TC_CMR_WAVE
                                 | TC_CMR_WAVSEL(2) | TC_CMR_EEVT(3) |
                                 TC_CMR_TCCLKS_TIMER_CLOCK5);
    TC0->TC_CHANNEL[1].TC_RA = 0x0001;
    TC0->TC_CHANNEL[1].TC_RB = 0x0002;
    TC0->TC_CHANNEL[1].TC_RC = 0xFFFF;
    TC0->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;

    TC0->TC_BCR = 1;
}

void interrupt_setup(void)
{
    Disable_global_interrupt();
    NVIC_ClearPendingIRQ(TC1_IRQn);
    NVIC_EnableIRQ(TC1_IRQn);
    NVIC_ClearPendingIRQ(XDMAC_IRQn);
    NVIC_EnableIRQ(XDMAC_IRQn);
    Enable_global_interrupt();
}

int main(void)
{
    /* Initialize the system */
    sysclk_init();
    board_init();

    /* Configure console on intended serial device */
    configure_console();

    /* Output example information */
#ifdef USE_PRINTF
    while (1) {
        PRINTF("\n\r-- XDMAC Example --\n\r");
        PRINTF("-- %s\n\r", BOARD_NAME);
        PRINTF("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
    }
#endif //USE_PRINTF

    XDMAC_setup();
    timer_setup();

    /* Manchester encode test-block with known pattern */
    for (uint16_t i = 0, k = 0; i < ((MIP_USART_TX_BUFFER_LENGTH / 2) - 1);
         i++, k = k % 256) {
        g_mipTxBuffer[i] = manchester_encode(k);
        k++;
    }
    g_mipTxBuffer[0] = manchester_encode('M');
    g_mipTxBuffer[1] = manchester_encode('i');
    g_mipTxBuffer[2] = manchester_encode('l');
    g_mipTxBuffer[3] = manchester_encode('d');
    g_mipTxBuffer[4] = manchester_encode('e');
    g_mipTxBuffer[5] = manchester_encode('f');

    /* Initialize and enable DMA controller */
    pmc_enable_periph_clk(ID_XDMAC);

    PRINTF("> Test OK.\n\r");

    interrupt_setup();

    while (1) {
        if (g_xfer_done) {
#ifdef CONF_BOARD_ENABLE_CACHE
            SCB_InvalidateDCache();
#endif
        }
    };
}
