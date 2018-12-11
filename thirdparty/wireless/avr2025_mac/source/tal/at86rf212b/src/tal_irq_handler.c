/**
 * @file tal_irq_handler.c
 *
 * @brief This file handles the interrupts generated by the transceiver.
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
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
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * Licensed under Atmel's Limited License Agreement --> EULA.txt
 */

/* === INCLUDES ============================================================ */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "pal.h"
#include "return_val.h"
#include "tal.h"
#include "ieee_const.h"
#include "stack_config.h"
#include "bmm.h"
#include "qmm.h"
#include "tal_irq_handler.h"
#include "tal_rx.h"
#include "at86rf212b.h"
#include "tal_internal.h"
#include "tal_constants.h"
#include "tal_tx.h"
#include "mac_build_config.h"

/* === TYPES =============================================================== */

/* === MACROS ============================================================== */

/* === GLOBALS ============================================================= */

/* === PROTOTYPES ========================================================== */

/* === IMPLEMENTATION ====================================================== */

/*
 * @brief Transceiver interrupt handler
 *
 * This function handles the transceiver generated interrupts.
 */
void trx_irq_handler_cb(void)
{
	trx_irq_reason_t trx_irq_cause;

	trx_irq_cause = (trx_irq_reason_t)trx_reg_read(RG_IRQ_STATUS);

#if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP)
#if (DISABLE_TSTAMP_IRQ == 1)
	if (trx_irq_cause & TRX_IRQ_2_RX_START) {
		/*
		 * Get timestamp.
		 *
		 * In case Antenna diversity is used or the utilization of
		 * the Timestamp IRQ is disabled, the timestamp needs to be read
		 * now
		 * the "old-fashioned" way.
		 *
		 * The timestamping is generally only done for
		 * beaconing networks or if timestamping is explicitly enabled.
		 */
		pal_trx_read_timestamp(&tal_rx_timestamp);
	}
#endif  /* #if (DISABLE_TSTAMP_IRQ == 1) */
#endif  /* #if (defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP) */

	if (trx_irq_cause & TRX_IRQ_3_TRX_END) {
		/*
		 * TRX_END reason depends on if the trx is currently used for
		 * transmission or reception.
		 */
#if ((MAC_START_REQUEST_CONFIRM == 1) && (defined BEACON_SUPPORT))
		if ((tal_state == TAL_TX_AUTO) || tal_beacon_transmission)
#else
		if (tal_state == TAL_TX_AUTO)
#endif
		{
			/* Get the result and push it to the queue. */
			if (trx_irq_cause & TRX_IRQ_6_TRX_UR) {
				handle_tx_end_irq(true); /* see tal_tx.c */
			} else {
				handle_tx_end_irq(false); /* see tal_tx.c */
			}
		} else { /* Other tal_state than TAL_TX_... */
			 /* Handle rx interrupt. */
			handle_received_frame_irq(); /* see tal_rx.c */

#if (defined SW_CONTROLLED_CSMA) && (defined RX_WHILE_BACKOFF)
			if (tal_state == TAL_BACKOFF) {
				pal_timer_stop(TAL_T_BOFF);
				tal_state = TAL_CSMA_CONTINUE;
			}
#endif
		}
	}

#if (_DEBUG_ > 0)
	/* Other IRQ than TRX_END */
	if (trx_irq_cause != TRX_IRQ_3_TRX_END) {
		/* PLL_LOCK interrupt migth be set, because poll mode is
		 * enabled. */

		/*
		 * if (trx_irq_cause & TRX_IRQ_0_PLL_LOCK)
		 * {
		 *  Assert("unexpected IRQ: TRX_IRQ_0_PLL_LOCK" == 0);
		 * }
		 */
		if (trx_irq_cause & TRX_IRQ_1_PLL_UNLOCK) {
			Assert("unexpected IRQ: TRX_IRQ_1_PLL_UNLOCK" == 0);
		}

		/* RX_START interrupt migth be set, because poll mode is
		 * enabled. */

		/*
		 * if (trx_irq_cause & TRX_IRQ_2_RX_START)
		 * {
		 *  Assert("unexpected IRQ: TRX_IRQ_2_RX_START" == 0);
		 * }
		 */
		if (trx_irq_cause & TRX_IRQ_4_CCA_ED_DONE) {
			Assert("unexpected IRQ: TRX_IRQ_4_CCA_ED_DONE" == 0);
		}

		/* AMI interrupt might set, because poll mode is enabled. */

		/*
		 * if (trx_irq_cause & TRX_IRQ_5_AMI)
		 * {
		 *  Assert("unexpected IRQ: TRX_IRQ_5_AMI" == 0);
		 * }
		 */
		if (trx_irq_cause & TRX_IRQ_6_TRX_UR) {
			Assert("unexpected IRQ: TRX_IRQ_6_TRX_UR" == 0);
		}

		if (trx_irq_cause & TRX_IRQ_7_BAT_LOW) {
			Assert("unexpected IRQ: TRX_IRQ_7_BAT_LOW" == 0);
		}
	}
#endif
} /* trx_irq_handler_cb() */

#if ((defined BEACON_SUPPORT) || (defined ENABLE_TSTAMP))  && \
	(DISABLE_TSTAMP_IRQ == 0)

/*
 * \brief Timestamp interrupt handler
 *
 * The timestamping is only required for
 * - beaconing networks or if timestamping is explicitly enabled,
 * - and if the time stamp interrupt is not explicitly disabled.
 */
void trx_irq_timestamp_handler_cb(void)
{
	pal_trx_read_timestamp(&tal_rx_timestamp);
}

#endif

/*
 * \brief Transceiver interrupt handler for awake end IRQ
 *
 * This function handles the transceiver awake end interrupt.
 */
void trx_irq_awake_handler_cb(void)
{
	trx_irq_reason_t trx_irq_cause = (trx_irq_reason_t)trx_reg_read(
			RG_IRQ_STATUS);

	if (trx_irq_cause & TRX_IRQ_4_CCA_ED_DONE) {
		/* Set the wake-up flag. */
		tal_awake_end_flag = true;
	}

#if (_DEBUG_ > 0)
	if (trx_irq_cause & (~(TRX_IRQ_0_PLL_LOCK | TRX_IRQ_4_CCA_ED_DONE))) {
		Assert("Unexpected interrupt" == 0);
	}
#endif
}

/* EOF */
