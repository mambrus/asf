/**
 * \file
 *
 * \brief User Interface
 *
 * Copyright (c) 2009-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <asf.h>
#include <qt1081.h>
#include "ui.h"

#define  MOUSE_MOVE_RANGE  3

void ui_init(void)
{
	LED_On(LED0);
	LED_Off(LED1);
}

void ui_powerdown(void)
{
	LED_Off(LED0);
	LED_Off(LED1);
}


void ui_wakeup_enable(void)
{
	// No external interrupt implemented
}

void ui_wakeup_disable(void)
{
	// No external interrupt implemented
}

void ui_wakeup(void)
{
	LED_On(LED0);
}


void ui_process(uint16_t framenumber)
{
	static uint8_t cpt_sof = 0;

	if ((framenumber % 1000) == 0) {
		LED_On(LED1);
	}
	if ((framenumber % 1000) == 500) {
		LED_Off(LED1);
	}
	// Scan process running each 2ms
	cpt_sof++;
	if (2 > cpt_sof)
		return;
	cpt_sof = 0;

	// No Scan buttons

	// Look touch sensor activity for the X and Y events
	if (is_touch_sensor_2())
		udi_hid_mouse_moveX(MOUSE_MOVE_RANGE);
	if (is_touch_sensor_3())
		udi_hid_mouse_moveX(-MOUSE_MOVE_RANGE);
	if (is_touch_sensor_1())
		udi_hid_mouse_moveY(MOUSE_MOVE_RANGE);
	if (is_touch_sensor_0())
		udi_hid_mouse_moveY(-MOUSE_MOVE_RANGE);
}

/**
 * \defgroup UI User Interface
 *
 * Human interface on EVK1105 :
 * - PWR led is on when power present
 * - Led 0 is on when USB line is in IDLE mode, and off in SUSPEND mode
 * - Led 1 blinks when USB host has checked and enabled HID mouse interface
 * - Mouse buttons are not linked.
 * - Touch sensors are used to move mouse
 * - No button are linked to wakeup USB Host in remote wakeup mode.
 */
