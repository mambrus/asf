/**
 *
 * \brief SAM Direct Memory Access Controller driver example.
 *
 *     Pin-map SAME70Q21 (U == UART, No U == USART)
 *          TXD1    -> PB4
 *          RXD1    <- PA21
 *          UTXD0   -> PA10     (A)
 *          URXD0   <- PA9      (A)
 */

//#define USE_PRINTF

#include <conf_uart_serial.h>
#include "uxart_pins.h"
#include <asf.h>
#include <conf_board.h>
#include <conf_clock.h>
#include <string.h>
#include "manchester.h"
#include "mdp_xdmac.h"

mdp_Xdmac_t *mdp_Xdmac = (mdp_Xdmac_t *) XDMAC;

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
volatile uint32_t g_xdmac_Block = 0;
volatile uint32_t g_xdmac_Linked_List = 0;
volatile uint32_t g_xdmac_Disable = 0;
volatile uint32_t g_xdmac_Flush = 0;
volatile uint32_t g_xdmac_Read_Bus_Error = 0;
volatile uint32_t g_xdmac_Write_Bus_Error = 0;
volatile uint32_t g_xdmac_Request_Overflow_Error = 0;

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
    MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4;    /* (CCFG_SYSIO) PB4 or TDI Assignment */
    ioport_set_pin_peripheral_mode(CONF_UART_TXD_GPIO, CONF_UART_TXD_FLAGS);
#endif                          //CONF_BOARD_UART_CONSOLE

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
    usart_serial_init((Usart *) CONF_UART, &uart_serial_options);
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
        (uint32_t)&((Usart*)CONF_UART)->US_THR;      /* Destination address is US_THR */

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
    XDMAC->XDMAC_GE |= 1 << CONF_XDMAC_CH;  /*enable TX channel */
}

void XDMAC_transfer(void)
{
/*    USART1->US_THR = 80; */

    /* Clear any pending interrupts for this channel */
    uint32_t xdmaint = XDMAC->XDMAC_CHID[CONF_XDMAC_CH].XDMAC_CIS;
    (void)xdmaint;              /* Warning silence */

    g_xfer_done = 0;

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
    mdp_XdmacIntr_t dma_status = mdp_Xdmac->chid[CONF_XDMAC_CH].CIS;

    NVIC_ClearPendingIRQ(XDMAC_IRQn);
    NVIC_DisableIRQ(XDMAC_IRQn);

    if (*(uint32_t *)&dma_status) {
        g_xfer_done = 1;
    }
    if (dma_status.Block) {
        g_xdmac_Block++;
    }
    if (dma_status.Block) {
        g_xdmac_Block++;
    }
    if (dma_status.Linked_List) {
        g_xdmac_Linked_List++;
    }
    if (dma_status.Disable) {
        g_xdmac_Disable++;
    }
    if (dma_status.Flush) {
        g_xdmac_Flush++;
    }
    if (dma_status.Read_Bus_Error) {
        g_xdmac_Read_Bus_Error++;
    }
    if (dma_status.Write_Bus_Error) {
        g_xdmac_Write_Bus_Error++;
    }
    if (dma_status.Request_Overflow_Error) {
        g_xdmac_Request_Overflow_Error++;
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
    int xfer_done_cntr = 0;
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
#endif                          //USE_PRINTF

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
            xfer_done_cntr++;
        }
    };
}
