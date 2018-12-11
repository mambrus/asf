/**
 * \file
 *
 * \brief Example configuration
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
#ifndef CONF_EXAMPLE_H_INCLUDED
#define CONF_EXAMPLE_H_INCLUDED

// Configurations for USART used for USB-Serial bridge

// -- Enable/Disable
#define USART_ENABLE()
#define USART_DISABLE()
//#define USART_ENABLE()   gpio_set_pin_low(PIN_USART0_EN_IDX)
//#define USART_DISABLE()  gpio_set_pin_high(PIN_USART0_EN_IDX)

// -- Registers set base
//#define USART_BASE       ((Usart*)UART)
#define USART_BASE       ((Usart*)UART0)
//#define USART_BASE       USART0
//#define USART_BASE       USART1

// -- Handler see exceptions for SAM3 and SAM4
//#define USART_HANDLER    UART_Handler
#define USART_HANDLER    UART0_Handler
//#define USART_HANDLER    USART0_Handler
//#define USART_HANDLER    USART1_Handler

// -- HW ID to enable peripheral clock
//#define USART_ID         ID_UART
#define USART_ID         ID_UART0
//#define USART_ID         ID_USART0
//#define USART_ID         ID_USART1

// -- Interrupt priority setting
//#define USART_INT_IRQn   UART_IRQn
#define USART_INT_IRQn   UART0_IRQn
//#define USART_INT_IRQn   USART0_IRQn
//#define USART_INT_IRQn   USART1_IRQn

#define USART_INT_LEVEL  3

#endif /* CONF_EXAMPLE_H_INCLUDED */
