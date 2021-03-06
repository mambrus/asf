/**
 * \file
 *
 * \brief AVR CAN Configuration File
 *
 * Copyright (c) 2011-2018 Microchip Technology Inc. and its subsidiaries.
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
#ifndef _CONF_CAN_H
#define _CONF_CAN_H

#define NB_MOB_CHANNEL                          16
#define CAN_AUTOBAUD                            false

#define CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz
#define CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_SJW     1
#define CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PRES    3
#define CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PRS     2
#define CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PHS1    1
#define CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PHS2    1

#define BAUDRATE                            CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz
#define BAUDRATE_HZ                         1000000
#define BAUDRATE_SJW                        CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_SJW
#define BAUDRATE_PRES                       CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PRES
#define BAUDRATE_PRS                        CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PRS
#define BAUDRATE_PHS1                       CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PHS1
#define BAUDRATE_PHS2                       CAN_BAUDRATE_1MHZ_CANCLOCK_8MHz_PHS2

#define CAN0_INT_TX_LEVEL                       AVR32_INTC_INT0
#define CAN0_INT_RX_LEVEL                       AVR32_INTC_INT0
#define CAN0_INT_ERR_LEVEL                      AVR32_INTC_INT0
#define CAN0_INT_BOFF_LEVEL                     AVR32_INTC_INT0
#define CAN0_INT_WAKE_UP_LEVEL                  AVR32_INTC_INT0

#define CAN1_INT_TX_LEVEL                       AVR32_INTC_INT0
#define CAN1_INT_RX_LEVEL                       AVR32_INTC_INT0
#define CAN1_INT_ERR_LEVEL                      AVR32_INTC_INT0
#define CAN1_INT_BOFF_LEVEL                     AVR32_INTC_INT0
#define CAN1_INT_WAKE_UP_LEVEL                  AVR32_INTC_INT0

#define CAN_LIB_UNDER_INTERRUPT

#endif // _CONF_CAN_H
