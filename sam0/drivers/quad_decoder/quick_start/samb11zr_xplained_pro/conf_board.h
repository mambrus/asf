/**
 * \file
 *
 * \brief SAM B11 ZR Xplained Pro board configuration.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
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
#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

#define CONF_QUAD_DECODER_PIN_X_A   PIN_LP_GPIO_14;
#define CONF_QUAD_DECODER_PIN_X_B   PIN_LP_GPIO_15;
#define CONF_QUAD_DECODER_PIN_Y_A   PIN_LP_GPIO_16;
#define CONF_QUAD_DECODER_PIN_Y_B   PIN_LP_GPIO_17;
#define CONF_QUAD_DECODER_PIN_Z_A   PIN_LP_GPIO_18;
#define CONF_QUAD_DECODER_PIN_Z_B   PIN_LP_GPIO_19;

#define CONF_QUAD_DECODER_MUX_X_A	PINMUX_LP_GPIO_14_M_QUAD_DEC0_A;
#define CONF_QUAD_DECODER_MUX_X_B	PINMUX_LP_GPIO_15_M_QUAD_DEC0_B;
#define CONF_QUAD_DECODER_MUX_Y_A	PINMUX_LP_GPIO_16_M_QUAD_DEC1_A;
#define CONF_QUAD_DECODER_MUX_Y_B	PINMUX_LP_GPIO_17_M_QUAD_DEC1_B;
#define CONF_QUAD_DECODER_MUX_Z_A	PINMUX_LP_GPIO_18_M_QUAD_DEC2_A;
#define CONF_QUAD_DECODER_MUX_Z_B	PINMUX_LP_GPIO_19_M_QUAD_DEC2_B;

#endif /* CONF_BOARD_H_INCLUDED */
