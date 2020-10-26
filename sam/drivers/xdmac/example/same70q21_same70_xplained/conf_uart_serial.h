/**
 * \file
 *
 * \brief USART Serial Configuration
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

#ifndef CONF_USART_SERIAL_H_INCLUDED
#define CONF_USART_SERIAL_H_INCLUDED

/** UART Interface */
#define CONF_UART            UART1
#define CONF_UART_ID         ID_UART1
#define CONF_PERID_UART_TX   XDMAC_CHANNEL_HWID_UART1_TX

/* Actual pins are mapped in sam/boards/same70_xplained/same70_xplained.h
 * in the following manner:
 *
#define UART0_RXD_GPIO   PIO_PA21_IDX
#define UART0_TXD_GPIO   PIO_PB4_IDX
*/

#define CONF_UART_RXD_GPIO   UART1_RXD_GPIO
#define CONF_UART_RXD_FLAGS  UART1_RXD_FLAGS
#define CONF_UART_TXD_GPIO   UART1_TXD_GPIO
#define CONF_UART_TXD_FLAGS  UART1_TXD_FLAGS

/* Any or both defines below can be omitted if corresponding pin doesn't have
 * an alternative. If any of pins has, it's mandatury to set them or
 * copilation error occurs to serve as a double-check for system designer
 * */
#define UART1_TXD_ALT_1
//#define UART1_RXD_ALT_1

 /** Baudrate setting */
#define CONF_UART_BAUDRATE   (5000000UL)
/** Character length setting */
#define CONF_UART_CHAR_LENGTH  US_MR_CHRL_8_BIT
/** Parity setting */
#define CONF_UART_PARITY     US_MR_PAR_NO
/** Stop bits setting */
#define CONF_UART_STOP_BITS    US_MR_NBSTOP_1_BIT

#endif/* CONF_USART_SERIAL_H_INCLUDED */
