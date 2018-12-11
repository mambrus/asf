/**
 * \file
 *
 * \brief XPRO_LCD example configuration.
 *
 * Copyright (c) 2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/** Configuration of the XPRO_LCD LCD glass driver */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_XPRO_LCD_H_INCLUDED
#define CONF_XPRO_LCD_H_INCLUDED
#include "slcd.h"

/**
 * \name Circular Animation Shift Direction
 * @{
 */
/** Right Direction. */
#define XPRO_LCD_CSR_RIGHT      SLCD_CIRCULAR_SHIFT_RIGHT
/** Left Direction. */
#define XPRO_LCD_CSR_LEFT       SLCD_CIRCULAR_SHIFT_LEFT
/** @} */

/** Init contrast configuration, it's within [0-15]. */
#define CONF_XPRO_LCD_CONTRAST  10

/** Frame count 0 configuration.
	Prescaler is not bypassed,the overflow value is (CONF_XPRO_LCD_FC0+1).
 */
#define CONF_XPRO_LCD_FC0     2
/** Frame count 0 configuration.
	Prescaler is not bypassed,the overflow value is (CONF_XPRO_LCD_FC0+1).
 */
#define CONF_XPRO_LCD_FC1     2
/** Frame count 0 configuration.
	Prescaler is not bypassed,the overflow value is (CONF_XPRO_LCD_FC0+1).
 */
#define CONF_XPRO_LCD_FC2     1

/** Blink timer configuration. */
#define CONF_XPRO_LCD_BLINK_TIMER                 SLCD_FRAME_COUNTER_0

/** Circular animation configuration. */
#define CONF_XPRO_LCD_CIRCULAR_ANIMATION_TIMER    SLCD_FRAME_COUNTER_0


#define XPRO_LCD_NB_OF_COM 4
#define XPRO_LCD_NB_OF_SEG 24

#define XPRO_LCD_NUM_SEG_INDEX_S 4
#define XPRO_LCD_NUM_SEG_INDEX_E 7

#define XPRO_LCD_TXT_SEG_INDEX_S 4
#define XPRO_LCD_TXT_SEG_INDEX_E 7

#define XPRO_LCD_HOUR_SEG_S   4
#define XPRO_LCD_MIN_SEG_S    7

#define XPRO_LCD_MAX_CHAR 5
#define XPRO_LCD_MAX_NUM  5

#define XPRO_LCD_MAX_ICON 26

/* Icon with selected segments blinking feature */
#define XPRO_LCD_ICON_USB                  1, 1
#define XPRO_LCD_ICON_COLON                3, 1
#define XPRO_LCD_ICON_BAT                  0, 0
#define XPRO_LCD_ICON_ATMEL                0, 1
/* Icon without selected segments blinking feature */
#define XPRO_LCD_ICON_MINUS                0, 17
#define XPRO_LCD_ICON_MINUS_SEG1           0, 13
#define XPRO_LCD_ICON_MINUS_SEG2           0, 9
#define XPRO_LCD_ICON_DOT_1                0, 5
#define XPRO_LCD_ICON_DOT_2                3, 6
#define XPRO_LCD_ICON_DOT_3                3, 10
#define XPRO_LCD_ICON_DOT_4                3, 14
#define XPRO_LCD_ICON_DOT_5                3, 18
#define XPRO_LCD_ICON_BAT_LEVEL_1          2, 0
#define XPRO_LCD_ICON_BAT_LEVEL_2          3, 0
#define XPRO_LCD_ICON_BAT_LEVEL_3          1, 0
#define XPRO_LCD_ICON_WIRELESS_LEVEL_0     3, 3
#define XPRO_LCD_ICON_WIRELESS_LEVEL_1     3, 2
#define XPRO_LCD_ICON_WIRELESS_LEVEL_2     2, 3
#define XPRO_LCD_ICON_WIRELESS_LEVEL_3     2, 2
#define XPRO_LCD_ICON_AUDIO_PLAY           2, 1
#define XPRO_LCD_ICON_AM                   0, 2
#define XPRO_LCD_ICON_PM                   0, 3
#define XPRO_LCD_ICON_DEGREE_C             3, 22
#define XPRO_LCD_ICON_DEGREE_F             0, 21
#define XPRO_LCD_ICON_VOLT                 1, 2
#define XPRO_LCD_ICON_MILLI_VOLT           1, 3

/* @} */

#endif /* CONF_XPRO_LCD_H_INCLUDED */