/**
 * \file
 *
 * \brief User Interface
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

#include <asf.h>
#include "ui.h"

#define  MOUSE_MOVE_RANGE  3

/* Wakeup pin is push button 2 (BP3, fast wakeup 10) */
#define  WAKEUP_PMC_FSTT (PUSHBUTTON_2_WKUP_FSTT)
#define  WAKEUP_PIN      (GPIO_PUSH_BUTTON_2)
#define  WAKEUP_PIO      (PIN_PUSHBUTTON_2_PIO)
#define  WAKEUP_PIO_ID   (PIN_PUSHBUTTON_2_ID)
#define  WAKEUP_PIO_MASK (PIN_PUSHBUTTON_2_MASK)
#define  WAKEUP_PIO_ATTR \
	(PIO_INPUT | PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_LOW_LEVEL)

/* Interrupt on "pin change" from push button to do wakeup on USB
 * Note:
 * This interrupt is enable when the USB host enable remotewakeup feature
 * This interrupt wakeup the CPU if this one is in idle mode
 */
static void ui_wakeup_handler(uint32_t id, uint32_t mask)
{
	if (WAKEUP_PIO_ID == id && WAKEUP_PIO_MASK == mask) {
		/* It is a wakeup then send wakeup USB */
		udc_remotewakeup();
	}
}

void ui_init(void)
{
	/* Set handler for push button */
	pio_handler_set(WAKEUP_PIO, WAKEUP_PIO_ID, WAKEUP_PIO_MASK,
		WAKEUP_PIO_ATTR, ui_wakeup_handler);
	/* Enable IRQ for button (PIOA) */
	NVIC_EnableIRQ((IRQn_Type) WAKEUP_PIO_ID);
	/* Initialize LEDs */
	LED_Off(LED0);
	LED_Off(LED1);
	LED_Off(LED2);
}

void ui_powerdown(void)
{
	LED_Off(LED0);
	LED_Off(LED1);
	LED_Off(LED2);
}


void ui_wakeup_enable(void)
{
	/* Configure BP3 as PIO input */
	pio_configure_pin(WAKEUP_PIN, WAKEUP_PIO_ATTR);
	/* Enable interrupt for BP3 */
	pio_enable_pin_interrupt(WAKEUP_PIN);
	/* Enable fast wakeup for button pin (WKUP10 for PA20) */
	pmc_set_fast_startup_input(WAKEUP_PMC_FSTT);
}

void ui_wakeup_disable(void)
{
	/* Disable interrupt for button pin */
	pio_disable_pin_interrupt(WAKEUP_PIN);
	/* Disable fast wakeup for button pin (WKUP10 for BP3) */
	pmc_clr_fast_startup_input(WAKEUP_PMC_FSTT);
}

void ui_wakeup(void)
{
	LED_On(LED0);
}


void ui_start_read(void)
{
	LED_On(LED2);
}

void ui_stop_read(void)
{
	LED_Off(LED2);
}

void ui_start_write(void)
{
	LED_On(LED2);
}

void ui_stop_write(void)
{
	LED_Off(LED2);
}

void ui_process(uint16_t framenumber)
{
	bool b_btn_state;
	static bool btn_left = false, btn_right = false;
	static uint16_t cpt_sof = 0;

	if ((framenumber % 1000) == 0) {
		LED_On(LED1);
	}
	if ((framenumber % 1000) == 500) {
		LED_Off(LED1);
	}
	/* Scan process running each 2ms */
	cpt_sof++;
	if (cpt_sof < 2) {
		return;
	}
	cpt_sof = 0;

	/* Uses buttons to move mouse */
	if (!ioport_get_pin_level(GPIO_PUSH_BUTTON_3)) {
		udi_hid_mouse_moveY(-MOUSE_MOVE_RANGE);
	}
	if (!ioport_get_pin_level(GPIO_PUSH_BUTTON_4)) {
		udi_hid_mouse_moveY( MOUSE_MOVE_RANGE);
	}
	/* Check button click */
	b_btn_state = !ioport_get_pin_level(GPIO_PUSH_BUTTON_2);
	if (b_btn_state != btn_left) {
		btn_left = b_btn_state;
		udi_hid_mouse_btnleft(btn_left);
	}
	b_btn_state = !ioport_get_pin_level(GPIO_PUSH_BUTTON_1);
	if (b_btn_state != btn_right) {
		btn_right = b_btn_state;
		udi_hid_mouse_btnright(btn_right);
	}
}
/**
 * \defgroup UI User Interface
 *
 * Human interface on SAM4E-EK:
 * - Led 0 (D2) is on when USB is wakeup
 * - Led 1 (D3) blinks when USB host has checked and enabled HID and MSC interface
 * - Led 2 (D4) is on during MSC read/write operation
 * - Push button 2 (BP3) and push button 1 (BP2) are linked to mouse button
 *   left and right
 * - Push button 3 (BP4) and push button 4 (BP5) are used to move mouse up
 *   and down
 * - Only a low level on push button 2 (BP3) will generate a wakeup to USB Host
 *   in remote wakeup mode.
 *
 */
