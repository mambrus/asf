/**
 * \file
 *
 * \brief STK600 with RC032X routing board init.
 *
 * To use this board, define BOARD=STK600_RC032X.
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
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
#include "board.h"
#include "compiler.h"
#include "stk600_rc032x.h"
#include "conf_board.h"
#include "ioport.h"

void board_init(void)
{
	ioport_configure_pin(LED0_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED1_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED2_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED3_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED4_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED5_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED6_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(LED7_GPIO, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);

	ioport_configure_pin(GPIO_PUSH_BUTTON_0, IOPORT_DIR_INPUT
			| IOPORT_PULL_UP | IOPORT_FORCE_ENABLE);
	ioport_configure_pin(GPIO_PUSH_BUTTON_1, IOPORT_DIR_INPUT
			| IOPORT_PULL_UP | IOPORT_FORCE_ENABLE);
	ioport_configure_pin(GPIO_PUSH_BUTTON_2, IOPORT_DIR_INPUT
			| IOPORT_PULL_UP | IOPORT_FORCE_ENABLE);
	ioport_configure_pin(GPIO_PUSH_BUTTON_3, IOPORT_DIR_INPUT
			| IOPORT_PULL_UP | IOPORT_FORCE_ENABLE);

#ifdef CONF_BOARD_ENABLE_AC_PINS
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTA, 0), IOPORT_DIR_INPUT);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTA, 2), IOPORT_DIR_INPUT);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTB, 1), IOPORT_DIR_INPUT);
#endif

#ifdef CONF_BOARD_AT45DBX
	ioport_configure_pin(AT45DBX_MASTER_SCK, IOPORT_DIR_OUTPUT |
			IOPORT_INIT_HIGH);
	ioport_configure_pin(AT45DBX_MASTER_MOSI, IOPORT_DIR_OUTPUT |
			IOPORT_INIT_HIGH);
	ioport_configure_pin(AT45DBX_MASTER_MISO, IOPORT_DIR_INPUT);
	ioport_configure_pin(AT45DBX_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
#endif

#ifdef CONF_BOARD_ENABLE_USARTC0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 3), IOPORT_DIR_OUTPUT
			| IOPORT_INIT_HIGH);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif

#ifdef CONF_BOARD_ENABLE_USARTD0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 3), IOPORT_DIR_OUTPUT
			| IOPORT_INIT_HIGH);
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 2), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif

#ifdef CONF_BOARD_ENABLE_USARTC0_ONEWIRE
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_INPUT);
#endif

#ifdef CONF_BOARD_ENABLE_USARTD0_ONEWIRE
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 2), IOPORT_DIR_INPUT);
#endif

#ifdef CONF_BOARD_CLKOUT_PC7
	PORTC.DIR |= 0x80;
	PORTCFG.CLKOUT |= PORTCFG_CLKOUT_PC7_gc;
#endif

	/* XCL OUT0 configurations */
#ifdef CONF_BOARD_XCL_OUT0_PD0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 0), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_OUT0_PD4
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 4), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_OUT0_PC0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 0), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_OUT0_PC4
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 4), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_OUT0_PD0_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 0), IOPORT_DIR_OUTPUT
			| IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_OUT0_PD4_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 4), IOPORT_DIR_OUTPUT
			| IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_OUT0_PC0_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 0), IOPORT_DIR_OUTPUT
			| IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_OUT0_PC4_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 4),
			IOPORT_DIR_OUTPUT | IOPORT_INV_ENABLED);
#endif

	/* XCL CC0 and CC1 configurations */
#ifdef CONF_BOARD_XCL_CC0_PC2
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_CC0_PC3
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 3), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_CC0_PD2
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 2), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_CC0_PD3
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 3), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_XCL_CC0_PC2_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_OUTPUT
			| IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_CC0_PC3_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 3), IOPORT_DIR_OUTPUT
			| IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_CC0_PD2_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 2), IOPORT_DIR_OUTPUT
			| IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_CC0_PD3_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 3), IOPORT_DIR_OUTPUT
			| IOPORT_INV_ENABLED);
#endif

	/* XCL IN0 configurations */
#ifdef CONF_BOARD_XCL_IN0_PD2
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 2), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN0_PD2_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 2), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN0_PD6
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 6), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN0_PD6_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 6), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN0_PC2
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN0_PC2_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN0_PC6
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 6), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN0_PC6_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 6), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif

	/* XCL IN1 Configurations */
#ifdef CONF_BOARD_XCL_IN1_PD0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 0),
			IOPORT_DIR_INPUT | IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN1_PD0_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 0), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN1_PD4
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 4), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN1_PD4_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 4), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN1_PC0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 0), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN1_PC0_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 0), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN1_PC4
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 4), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN1_PC4_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 4), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif

	/* XCL IN2 Configurations */
#ifdef CONF_BOARD_XCL_IN2_PD1
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 1), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN2_PD1_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 1), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN2_PD5
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 5), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN2_PD5_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 5), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN2_PC1
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 1), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN2_PC1_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 1), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN2_PC5
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 5), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN2_PC5_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 5), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif

	/* XCL IN3 Configurations */
#ifdef CONF_BOARD_XCL_IN3_PD3
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 3), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN3_PD3_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 3), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN3_PD7
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 7), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN3_PD7_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 7), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN3_PC3
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 3), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN3_PC3_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 3), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif
#ifdef CONF_BOARD_XCL_IN3_PC7
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 7), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE);
#endif
#ifdef CONF_BOARD_XCL_IN3_PC7_INV
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 7), IOPORT_DIR_INPUT
			| IOPORT_FORCE_ENABLE | IOPORT_INV_ENABLED);
#endif

	/* TC45 Output Configurations */
#ifdef CONF_BOARD_TC45_OUT0_PC0
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 0), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PC1
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 1), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PC2
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 2), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PC3
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 3), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PC4
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 4), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PC5
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 5), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PC6
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 6), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PC7
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTC, 7), IOPORT_DIR_OUTPUT);
#endif
#ifdef CONF_BOARD_TC45_OUT0_PD4
	ioport_configure_pin(IOPORT_CREATE_PIN(PORTD, 4), IOPORT_DIR_OUTPUT);
#endif
}
