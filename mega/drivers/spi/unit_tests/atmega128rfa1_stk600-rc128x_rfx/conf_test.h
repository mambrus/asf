/**
 * \file
 *
 * \brief Unit test configuration
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
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
#ifndef CONF_TEST_H
#define CONF_TEST_H

/**
 * \defgroup spi_unit_test_pin_defs
 *  - <b> ATMEGA128RFA1-EK-STK600  --  AT45DBX component </b>
 *   - SS(PB0)   -- CS
 *   - SCK(PB1)  -- SCK
 *   - MISO(PB3) -- SO
 *   - MOSI(PB2) -- SI
 */

/* ! \note Perform unit tests using SPI */
#define CONF_TEST_SPI        SPCR
/* ! \note SPI BAUDRATE */
#define CONF_TEST_SPI_BAUDRATE 1000000
/* ! \note SPI Mode */
#define CONF_TEST_SPI_MODE   SPI_MODE0
/* ! \note USARTA1 on PORTD (RX on PD2, TX on PD3) */
#define CONF_TEST_USART      &USARTA1
/* ! \note 38.4 kbaud */
#define CONF_TEST_BAUDRATE   38400
/* ! \note 8-bit character length */
#define CONF_TEST_CHARLENGTH USART_CHSIZE_8BIT_gc
/* ! \note No parity check */
#define CONF_TEST_PARITY     USART_PMODE_DISABLED_gc
/* ! \note No extra stopbit, i.e., use 1 */
#define CONF_TEST_STOPBITS   false

#endif
