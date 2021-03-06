/**
 * \file
 *
 * \brief Interrupt monitor.
 *
 * \par Application note:
 *      AVR1610: Guide to IEC60730 Class B compliance with XMEGA
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
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */


#include "classb_interrupt_monitor.h"

/**
 * \ingroup int_mon_func
 * \brief Array of data structures for the monitored interrupts
 */
static struct classb_intmon_interrupt monitored_interrupts[N_INTERRUPTS];

/**
 * \brief Register an interrupt.
 *
 * This function registers the information that the monitor needs in order to
 * check an interrupt. It should be called from the main application. Interrupts
 * must be declared in \ref classb_int_identifiers in the configuration file.
 * Note that by default the interrupt is not active for monitoring, i.e. the
 * state is \c OFF.
 *
 * \param identifier Interrupt identifier. Use symbol declared in
 * \ref classb_int_identifiers
 * \param reference Number of expected executions for the interrupt within an
 * RTC period:
 * <tt>F_INT (Hz) * CLASSB_RTC_INT_PERIOD / CLASSB_RTC_FREQ (Hz) </tt>
 * \param tolerance The allowed deviation (%) with respect to interrupt_counter
 * for the interrupt counter.
 *
 * \callergraph
 */
void classb_intmon_reg_int(enum classb_int_identifiers identifier,
		uint16_t reference, uint8_t tolerance)
{
	monitored_interrupts[identifier].expected = reference;
	monitored_interrupts[identifier].count = 0;
	monitored_interrupts[identifier].limit = (reference * tolerance) / 100;
	monitored_interrupts[identifier].state = OFF;
}

/**
 * \brief Increase the counter for the specified interrupt.
 *
 * This is called from the interrupt routine and it will increases the counter
 * if the interrupt is \c ON.
 *
 * \param identifier Interrupt identifier. Use symbol declared in
 * \ref classb_int_identifiers
 *
 * \callergraph
 */
void classb_intmon_increase(enum classb_int_identifiers identifier)
{
	if (monitored_interrupts[identifier].state == ON) {
		monitored_interrupts[identifier].count++;
	}
}

/**
 * \brief Set a state for the specified interrupt.
 *
 * This function should be called from the main application to enable or disable
 * monitoring this interrupt. The application can only set the state to ENABLE
 * or DISABLE, otherwise the error handler will be called, i.e., only the
 * monitor can set \c ON and \c OFF states.
 *
 * Further, if \ref CLASSB_STRICT_INTMON is defined, it is not allowed to enable
 * an interrupt already in the \c ON state, or disable an interrupt already in
 * the \c OFF state.
 *
 * \param identifier Interrupt identifier. Use symbol declared in
 * \ref classb_int_identifiers.
 * \param state Interrupt state. Use symbol declared in \ref classb_int_states.
 *
 * \callergraph
 */
void classb_intmon_set_state(enum classb_int_identifiers identifier,
		enum classb_int_states state)
{

	switch (state) {
	case M_ENABLE:
#ifdef CLASSB_STRICT_INTMON
		if (monitored_interrupts[identifier].state != OFF) {
			CLASSB_ERROR_HANDLER_INTERRUPT();
			break;
		}
#endif
		break;

	case M_DISABLE:
#ifdef CLASSB_STRICT_INTMON
		if (monitored_interrupts[identifier].state != ON) {
			CLASSB_ERROR_HANDLER_INTERRUPT();
			break;
		}
#endif
		break;

	default:
		CLASSB_ERROR_HANDLER_INTERRUPT();
	}

	/* Set the new state only if CLASSB_CONDITION1_INTERRUPT is true. */
	if (CLASSB_CONDITION1_INTERRUPT) {
		monitored_interrupts[identifier].state = state;
	}
}

/** Return the absolute value of the difference between two numbers. */
static inline uint16_t abs_diff(uint16_t a, uint16_t b)
{
	return (a > b) ? (a - b) : (b - a);
}

/**
 * \brief The interrupt monitor.
 *
 * For each registered interrupt, this function compares the counter with the
 * expected value. There is an error if the difference is greater than the limit
 * or if the interrupt is disabled and the counter is different than zero.
 * If \ref CLASSB_CONDITION2_INTERRUPT is true, the monitor will stop
 * immediately, i.e., the remaining interrupts are not checked.
 *
 * \note This should be called back from the RTC interrupt. See \ref rtc_driver.
 *
 * \callergraph
 */
void classb_intmon_callback(void)
{
	for (uint8_t i = 0; i < N_INTERRUPTS; i++ ) {
		switch (monitored_interrupts[i].state) {
		case ON:
			/* Check if the counter is within the allowed range */
			if ((uint16_t)abs_diff(monitored_interrupts[i].count,
					monitored_interrupts[i].expected)
					> monitored_interrupts[i].limit) {
				CLASSB_ERROR_HANDLER_INTERRUPT();
				break;
			}
			/* Reset counter */
			monitored_interrupts[i].count = 0;
			break;

		case OFF:
			/* The counter is only increased if the state is ON. */
			if (monitored_interrupts[i].count) {
				CLASSB_ERROR_HANDLER_INTERRUPT();
			}
			break;

		case M_ENABLE:
			/* Change state */
			monitored_interrupts[i].state = ON;
			break;

		case M_DISABLE:
			/* Change state and reset the counter */
			monitored_interrupts[i].state = OFF;
			monitored_interrupts[i].count = 0;
			break;

		default:
			CLASSB_ERROR_HANDLER_INTERRUPT();
		}

		/* If CLASSB_CONDITION2_INTERRUPT is true, there is no need to
		 * check the other interrupts.
		 */
		if (CLASSB_CONDITION2_INTERRUPT) {
			break;
		}
	}
}
