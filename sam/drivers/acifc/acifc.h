/**
 * \file
 *
 * \brief SAM4L Analog Comparator Interface Driver.
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef ACIFC_H_INCLUDED
#define ACIFC_H_INCLUDED

/**
 * \defgroup asfdoc_sam_drivers_acifc_group SAM4L Analog Comparator Interface Controller (ACIFC) Driver
 *
 * This driver for Atmel&reg; | SMART ARM&reg;-based microcontrollers provides 
 * an interface for the configuration and management of the device's Analog 
 * Comparator Interface Controller functionality.
 *
 * The Analog Comparator Interface controls eight Analog Comparators (AC) with
 * identical behavior. Each Analog Comparator compares two voltages, yielding an
 * output that depends on the result of the comparison.
 *
 * Devices from the following series can use this module:
 * - Atmel | SMART SAM4L
 *
 * The outline of this documentation is as follows:
 *  - \ref asfdoc_sam_drivers_acifc_prerequisites
 *  - \ref asfdoc_sam_drivers_acifc_module_overview
 *  - \ref asfdoc_sam_drivers_acifc_special_considerations
 *  - \ref asfdoc_sam_drivers_acifc_extra_info
 *  - \ref asfdoc_sam_drivers_acifc_examples
 *  - \ref asfdoc_sam_drivers_acifc_api_overview
 *
 *
 * \section asfdoc_sam_drivers_acifc_prerequisites Prerequisites
 *
 * There are no prerequisites for this module.
 *
 *
 * \section asfdoc_sam_drivers_acifc_module_overview Module Overview
 *
 * The Analog Comparator Interface (ACIFC) controls a number of Analog Comparators
 * (AC) with identical behavior. Each Analog Comparator compares two voltages and
 * gives a compare output depending on the results of this comparison.
 *
 * The ACIFC module can be configured in normal mode, in which each comparator
 * is used independently, or in window mode, which uses defined comparator pairs.
 * The comparators do not all have to be in the same mode; some comparators may
 * be in normal mode, while others are in window mode. There are, however,
 * restrictions on which ACs can be grouped together in window mode.
 *
 * \section asfdoc_sam_drivers_acifc_special_considerations Special Considerations
 *
 * \subsection asfdoc_sam_drivers_acifc_special_considerations_io I/O Lines
 * The ACIFC pins are multiplexed with other peripherals. The user application
 * must therefore configure the GPIO Controller, to give the ACIFC module control
 * of the associated pins.
 *
 * \subsection asfdoc_sam_drivers_acifc_special_considerations_power Power Management
 * The ACIFC stops functioning when the system enters a sleep mode that disables
 * its clock. However, the ACIFC module can resume its operation if the system
 * enters the SleepWalking mode and the ACIFC clock is started by the Peripheral
 * Event System. During this time, if the ACIFC module generates an interrupt, the
 * system will wake-up from sleep mode and normal system operation resumes.
 *
 * \subsection asfdoc_sam_drivers_acifc_special_considerations_clocks Clocks
 * The clock for ACIFC (CLK_ACIFC) is generated by the Power Manager (PM). It
 * can be manually disabled, through the Power Manager, or may be automatically
 * disabled when the system enters a sleep mode that disables the clocks to the
 * peripheral bus modules.
 *
 * \subsection asfdoc_sam_drivers_acifc_special_considerations_irq Interrupts
 * The ACIFC interrupt request line is connected to the Nested Vectored Interrupt
 * Controller (NVIC). Using the ACIFC interrupt requires that the NVIC be
 * configured beforehand.
 *
 * \subsection asfdoc_sam_drivers_acifc_special_considerations_events Peripheral Events
 * The ACIFC peripheral events are connected via the Peripheral Event System.
 *
 * \subsection asfdoc_sam_drivers_acifc_special_considerations_debug Debug Operation
 * When an external debugger forces the CPU into debug mode, the ACIFC module
 * continues normal operation. If the ACIFC module is configured in a way that
 * requires it to be periodically serviced by the CPU through interrupts (or similar),
 * debugging may result in improper operation or data loss.
 *
 *
 * \section asfdoc_sam_drivers_acifc_extra_info Extra Information
 *
 * For extra information, see \ref asfdoc_sam_drivers_acifc_extra. This includes:
 *  - \ref asfdoc_sam_drivers_acifc_extra_acronyms
 *  - \ref asfdoc_sam_drivers_acifc_extra_dependencies
 *  - \ref asfdoc_sam_drivers_acifc_extra_errata
 *  - \ref asfdoc_sam_drivers_acifc_extra_history
 *
 * \section asfdoc_sam_drivers_acifc_examples Examples
 *
 * For a list of examples related to this driver, see
 * \ref asfdoc_sam_drivers_acifc_exqsg.
 *
 *
 * \section asfdoc_sam_drivers_acifc_api_overview API Overview
 * @{
 */

#include <compiler.h>
#include <status_codes.h>

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

#if !defined(__DOXYGEN__)
/*** AC Number of channel */
#define AC_NB_OF_CH           8

/** AC Number of Window */
#define AC_NB_OF_WIN          (AC_NB_OF_CH >> 1)

/** AC Window interrupt Offset */
#define AC_WIN_INT_OFFSET     24
#endif /* !defined(__DOXYGEN__) */

/** ACIFC module configuration structure. */
struct ac_config {
	/** Peripheral event trigger enable/disable. */
	bool event_trigger;
};

/* Prototype for device instance. */
struct ac_dev_inst;

/** ACIFC interrupt callback type. */
typedef void (*ac_callback_t)(void);

/** ACIFC driver structure. */
struct ac_dev_inst {
	/** Base address of the ACIFC module. */
	Acifc *hw_dev;
	/** Pointer to an ACIFC configuration structure. */
	struct ac_config  *cfg;
};

/** ACIFC hysteresis voltage. */
enum ac_hysteresis_voltage {
	AC_HYSTERESIS_0_MV = 0,		/**< 0mV of hysteresis */
	AC_HYSTERESIS_25_MV,		/**< 25mV of hysteresis */
	AC_HYSTERESIS_50_MV,		/**< 50mV of hysteresis */
	AC_HYSTERESIS_75_MV			/**< 75mV of hysteresis */
};

/** ACIFC negative input. */
enum ac_negative_input{
	AC_NEGTIVE_INPUT_EXTERNAL = 0,		/**< ACANx (ACBNx) pin selected. */
#if !defined(__DOXYGEN__)
	AC_NEGTIVE_INPUT_RESERVED1,			/**< Reserved */
	AC_NEGTIVE_INPUT_RESERVED2,			/**< Reserved */
	AC_NEGTIVE_INPUT_RESERVED3			/**< Reserved */
#endif /* !defined(__DOXYGEN__) */
};

/** ACIFC comparator mode. */
enum ac_comparator_mode {
	AC_COMPARATOR_OFF = 0,			/**< Off */
	AC_COMPARATOR_CONTINUOUS,		/**< Continuous measurement mode */
	AC_COMPARATOR_USER_TRIGGERED,	/**< User triggered single measurement mode */
	AC_COMPARATOR_EVENT_TRIGGERED	/**< Peripheral event triggered single measurement mode */
};

/** ACIFC channel interrupt setting. */
enum ac_ch_interrupt_setting {
	AC_CH_IS_VINP_GT_VINN = 0,		/**< Interrupt when V<SUB>INP</SUB> > V<SUB>INN</SUB> */
	AC_CH_IS_VINP_LT_VINN,			/**< Interrupt when V<SUB>INP</SUB> < V<SUB>INN</SUB> */
	AC_CH_IS_OUTPUT_TGL,			/**< Interrupt on toggle of ACOUT */
	AC_CH_IS_COMP_DONE				/**< Interrupt when comparison of V<SUB>INP</SUB> and V<SUB>INN</SUB> is done */
};

/** ACIFC channel configuration structure. */
struct ac_ch_config {
	/** Hysteresis value */
	enum ac_hysteresis_voltage hysteresis_voltage;
	/**  Specify whether the ACIFC channel is always on */
	bool always_on;
	/**  Enable/disable fast mode */
	bool fast_mode;
	/**  Enable/disable the output event when ACOUT is zero */
	bool event_negative;
	/**  Enable/disable the output event when ACOUT is one */
	bool event_positive;
	/** The negative input setting */
	enum ac_negative_input negative_input;
	/** The comparator mode setting */
	enum ac_comparator_mode comparator_mode;
	/** The interrupt setting */
	enum ac_ch_interrupt_setting interrupt_setting;
};

/** ACIFC window event output configuration. */
enum ac_win_event_source {
	AC_WIN_EVENT_ACWOUT_RISING_EDGE = 0,	/**< ACWOUT rising edge */
	AC_WIN_EVENT_ACWOUT_FALLING_EDGE,		/**< ACWOUT falling edge */
	AC_WIN_EVENT_ACWOUT_ON_ANY_EDGE,		/**< ACWOUT rising or falling edge */
	AC_WIN_EVENT_INSIDE_WINDOW,				/**< Inside window */
	AC_WIN_EVENT_OUTSIDE_WINDOW,			/**< Outside window */
	AC_WIN_EVENT_MEASURE_DONE,				/**< Measurement done */
#if !defined(__DOXYGEN__)
	AC_WIN_EVENT_RESERVED1,					/**< Reserved */
	AC_WIN_EVENT_RESERVED2					/**< Reserved */
#endif /* !defined(__DOXYGEN__) */
};

/** ACIFC window interrupt settings. */
enum ac_win_interrupt_setting {
	/** Window interrupt as soon as the common input voltage is inside the window */
	AC_WIN_IS_VINP_INSIDE_WINDOW = 0,
	/** Window interrupt as soon as the common input voltage is outside the window */
	AC_WIN_IS_VINP_OUTSIDE_WINDOW,
	/** Window interrupt on toggle of ACWOUT */
	AC_WIN_IS_WINDOW_OUTPUT_TGL,
	/** Window interrupt when evaluation of common input voltage is done */
	AC_WIN_IS_WINDOW_COMP_DONE,
	/** Window interrupt when the common input voltage enters the window
	(e.g. the rising-edge of ACWOUT) */
	AC_WIN_IS_VINP_ENTER_WINDOW,
	/** Window interrupt when the common input voltage leaves the window
	(e.g. the falling-edge of ACWOUT) */
	AC_WIN_IS_VINP_LEAVE_WINDOW,
#if !defined(__DOXYGEN__)
	/** Reserved */
	AC_WIN_IS_RESERVED1,
	/** Reserved */
	AC_WIN_IS_RESERVED2,
#endif /* !defined(__DOXYGEN__) */
};

/** ACIFC window configuration structure. */
struct ac_win_config {
	/**  Enable/disable window mode */
	bool enable;
	/**  Enable/disable the window event from ACWOUT */
	bool event_enable;
	/** Window event output configuration */
	enum ac_win_event_source event_source;
	/** Interrupt settings */
	enum ac_win_interrupt_setting interrupt_setting;
};

/** ACIFC interrupt configuration type. */
typedef enum ac_interrupt_source {
	/** Conversion completed on channel 0 */
	AC_INTERRUPT_CONVERSION_COMPLETED_0 = 0,
	/** Start-up time elapsed on channel 0 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_0,
	/** Conversion completed on channel 1 */
	AC_INTERRUPT_CONVERSION_COMPLETED_1,
	/** Start-up time elapsed on channel 1 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_1,
	/** Conversion completed on channel 2 */
	AC_INTERRUPT_CONVERSION_COMPLETED_2,
	/** Start-up time elapsed on channel 2 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_2,
	/** Conversion completed on channel 3 */
	AC_INTERRUPT_CONVERSION_COMPLETED_3,
	/** Start-up time elapsed on channel 3 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_3,
	/** Conversion completed on channel 4 */
	AC_INTERRUPT_CONVERSION_COMPLETED_4,
	/** Start-up time elapsed on channel 4 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_4,
	/** Conversion completed on channel 5 */
	AC_INTERRUPT_CONVERSION_COMPLETED_5,
	/** Start-up time elapsed on channel 5 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_5,
	/** Conversion completed on channel 6 */
	AC_INTERRUPT_CONVERSION_COMPLETED_6,
	/** Start-up time elapsed on channel 6 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_6,
	/** Conversion completed on channel 7 */
	AC_INTERRUPT_CONVERSION_COMPLETED_7,
	/** Start-up time elapsed on channel 7 */
	AC_INTERRUPT_STARTUP_TIME_ELAPSED_7,
	/** Window 0 interrupt */
	AC_INTERRUPT_WINDOW_0,
	/** Window 1 interrupt */
	AC_INTERRUPT_WINDOW_1,
	/** Window 2 interrupt */
	AC_INTERRUPT_WINDOW_2,
	/** Window 3 interrupt */
	AC_INTERRUPT_WINDOW_3,
#if !defined(__DOXYGEN__)	
	AC_INTERRUPT_NUM,
#endif /* !defined(__DOXYGEN__) */
} ac_interrupt_source_t;

/**
 * \brief Initialize an ACIFC configuration structure to default values.
 *
 * \note This function should be called to prepare all new instances of ACIFC
 * configuration structures before they are further modified by the user application.
 *
 * The default configuration is as follows:
 * - Peripheral event trigger is disabled
 *
 * \param[out] cfg  ACIFC configuration structure pointer
 */
static inline void ac_get_config_defaults(
		struct ac_config *const cfg)
{
	/* Sanity check argument. */
	Assert(cfg);

	cfg->event_trigger = false;
}

enum status_code ac_init(
		struct ac_dev_inst *const dev_inst,
		Acifc *const ac,
		struct ac_config *const cfg);

/**
 * \brief Initializes an ACIFC channel configuration structure to default values.
 *
 * \note This function should be called to prepare all new instances of ACIFC channel
 * configuration structures before they are further modified by the user application.
 *
 * The default configuration is as follows:
 *  - Hysteresis voltage = 0mV
 *  - ACIFC is disabled between measurements
 *  - Fast mode is disabled
 *  - No output peripheral event when ACOUT is zero or one
 *  - Negative input from the external pin
 *  - User triggered single measurement mode
 *  - Interrupt enabled when the comparison is done
 *
 * \param[out] cfg  ACIFC channel configuration structure pointer
 */
static inline void ac_ch_get_config_defaults(
		struct ac_ch_config *const cfg)
{
	/* Sanity check argument */
	Assert(cfg);

	cfg->hysteresis_voltage = AC_HYSTERESIS_0_MV;
	cfg->always_on = false;
	cfg->fast_mode = false;
	cfg->event_negative = false;
	cfg->event_positive = false;
	cfg->negative_input = AC_NEGTIVE_INPUT_EXTERNAL;
	cfg->comparator_mode = AC_COMPARATOR_USER_TRIGGERED;
	cfg->interrupt_setting = AC_CH_IS_COMP_DONE;
}

void ac_ch_set_config(struct ac_dev_inst *const dev_inst, uint32_t channel,
		struct ac_ch_config *cfg);

/**
 * \brief Initializes an ACIFC window configuration structure to default values.
 *
 * \note This function should be called to prepare all new instances of ACIFC window
 * configuration structures before they are further modified by the user application.
 *
 *  The default configuration is as follows:
 *  - Window mode is disabled
 *  - Peripheral event from ACWOUT is disabled
 *  - Generate the window peripheral event when the comparison is done
 *  - Window interrupt enabled when the evaluation of the common input voltage is
 *  done
 *
 * \param[out] cfg ACIFC window configuration structure pointer
 */
static inline void ac_win_get_config_defaults(
		struct ac_win_config *const cfg)
{
	/* Sanity check argument */
	Assert(cfg);

	cfg->enable = false;
	cfg->event_enable = false;
	cfg->event_source = AC_WIN_EVENT_MEASURE_DONE;
	cfg->interrupt_setting = AC_WIN_IS_WINDOW_COMP_DONE;
}

void ac_win_set_config(struct ac_dev_inst *const dev_inst,
		uint32_t window, struct ac_win_config *cfg);

void ac_set_callback(struct ac_dev_inst *const dev_inst,
		ac_interrupt_source_t source, ac_callback_t callback,
		uint8_t irq_level);

/**
 * \brief Get the ACIFC interrupt status.
 *
 * \param[in] dev_inst Driver structure pointer
 *
 * \return The ACIFC interrupt status.
 */
static inline uint32_t ac_get_interrupt_status(
		struct ac_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->ACIFC_ISR;
}

/**
 * \brief Get the ACIFC interrupt mask.
 *
 * \param[in] dev_inst Driver structure pointer
 *
 * \return The ACIFC interrupt mask.
 */
static inline uint32_t ac_get_interrupt_mask(
		struct ac_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->ACIFC_IMR;
}

/**
 * \brief Write to the ACIFC interrupt status.
 *
 * \param[in] dev_inst Driver structure pointer
 * \param[in] status   Interrupt status to be clear
 *
 */
static inline void ac_clear_interrupt_status(
		struct ac_dev_inst *const dev_inst,
		uint32_t status)
{
	dev_inst->hw_dev->ACIFC_ICR = status;
}

void ac_enable_interrupt(struct ac_dev_inst *const dev_inst,
		ac_interrupt_source_t source);
void ac_disable_interrupt(struct ac_dev_inst *const dev_inst,
		ac_interrupt_source_t source);

void ac_enable(struct ac_dev_inst *const dev_inst);
void ac_disable(struct ac_dev_inst *const dev_inst);

/**
 * \brief Start a single ACIFC comparison.
 *
 * \param[in] dev_inst Driver structure pointer
 */
static inline void ac_user_trigger_single_comparison(
		struct ac_dev_inst *const dev_inst)
{
	dev_inst->hw_dev->ACIFC_CTRL |= ACIFC_CTRL_USTART;
}

/**
 * \brief Check if the ACIFC comparison is done.
 *
 * \param[in] dev_inst Driver structure pointer
 *
 * \return The ACIFC comparison result.
 * \retval false ACIFC comparison has not completed
 * \retval true ACIFC comparison has completed
 */
static inline bool ac_is_comparison_done(
		struct ac_dev_inst *const dev_inst)
{
	return ((dev_inst->hw_dev->ACIFC_CTRL & ACIFC_CTRL_USTART) !=
			ACIFC_CTRL_USTART);
}

/**
 * \brief Get the ACIFC status.
 *
 * \param[in] dev_inst Driver structure pointer
 *
 * \return The ACIFC status.
 */
static inline uint32_t ac_get_status(
		struct ac_dev_inst *const dev_inst)
{
	return dev_inst->hw_dev->ACIFC_SR;
}

/// @cond
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond

 /** @} */

/**
 * \page asfdoc_sam_drivers_acifc_extra Extra Information for Analog Comparator Interface Driver
 *
 * \section asfdoc_sam_drivers_acifc_extra_acronyms Acronyms
 * Below is a table listing the acronyms used in this module, along with their
 * intended meanings.
 *
 * <table>
 *  <tr>
 *      <th>Acronym</th>
 *      <th>Definition</th>
 *  </tr>
 *  <tr>
 *      <td>NVIC</td>
 *      <td>Nested Vectored Interrupt Controller</td>
 * </tr>
 *  <tr>
 *      <td>PES</td>
 *      <td>Peripheral Event System</td>
 * </tr>
 *  <tr>
 *      <td>QSG</td>
 *      <td>Quick Start Guide</td>
 * </tr>
 * </table>
 *
 *
 * \section asfdoc_sam_drivers_acifc_extra_dependencies Dependencies
 * This driver has the following dependencies:
 *
 *  - None
 *
 *
 * \section asfdoc_sam_drivers_acifc_extra_errata Errata
 * There are no errata related to this driver.
 *
 *
 * \section asfdoc_sam_drivers_acifc_extra_history Module History
 * An overview of the module history is presented in the table below, with
 * details on the enhancements and fixes made to the module since its first
 * release. The current version of this corresponds to the newest version in
 * the table.
 *
 * <table>
 *	<tr>
 *		<th>Changelog</th>
 *	</tr>
 *	<tr>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>
 */
 
/**
 * \page asfdoc_sam_drivers_acifc_exqsg Examples for Analog Comparator Interface
 *
 * This is a list of the available Quick Start guides (QSGs) and example
 * applications for \ref asfdoc_sam_drivers_acifc_group. QSGs are simple examples with
 * step-by-step instructions to configure and use this driver in a selection of
 * use cases. Note that a QSG can be compiled as a standalone application or be
 * added to the user application.
 *
 *  - \subpage asfdoc_sam_drivers_acifc_qsg
 *  - \subpage asfdoc_sam_drivers_acifc_example
 *
 * \page asfdoc_sam_drivers_acifc_document_revision_history Document Revision History
 *
 * <table>
 *	<tr>
 *		<th>Doc. Rev.</td>
 *		<th>Date</td>
 *		<th>Comments</td>
 *	</tr>
 *	<tr>
 *		<td>42323B
 *		<td>05/2014</td>
 *		<td>Updated title of application note and added list of supported devices</td>
 *	</tr>
 *	<tr>
 *		<td>42323A</td>
 *		<td>05/2014</td>
 *		<td>Initial document release</td>
 *	</tr>
 * </table>
 *
 */

/**
 * \page asfdoc_sam_drivers_acifc_qsg Quick Start Guide for SAM4L Analog Comparator Driver
 *
 * This is the quick start guide for the \ref asfdoc_sam_drivers_acifc_group,
 * with step-by-step instructions on how to configure and use the driver in a
 * selection of use cases.
 *
 * The use cases contain several code fragments. The code fragments in the
 * steps for setup can be copied into a custom initialization function, while
 * the steps for usage can be copied into, e.g., the main application function.
 *
 * \section acifc_basic_use_case Basic Use Case
 * In this basic use case ACIFC channel 0 is configured to compare the inputs
 * from ACAP0 and ACAN0.
 *
 * \subsection sam_acifc_quickstart_prereq Prerequisites
 * - \ref sysclk_group "System Clock Management (SysClock)"
 *
 * \section acifc_basic_use_case_setup Setup Steps
 * \subsection acifc_basic_use_case_setup_code Example Code
 * Enable the following macros in the header file conf_clock.h:
 * \code #define CONFIG_SYSCLK_SOURCE       SYSCLK_SRC_DFLL \endcode
 * \code #define CONFIG_DFLL0_SOURCE        GENCLK_SRC_OSC32K \endcode
 *
 * Add the following code to your application C-file:
 * \code sysclk_init(); \endcode
 *
 * \subsection acifc_basic_use_case_setup_flow Workflow
 * -# Set system clock source as DFLL:
 * \code #define CONFIG_SYSCLK_SOURCE       SYSCLK_SRC_DFLL \endcode
 * -# Set DFLL source as OSC32K:
 * \code #define CONFIG_DFLL0_SOURCE        GENCLK_SRC_OSC32K \endcode
 * -# Initialize the system clock:
 * \code sysclk_init(); \endcode
 *
 * \section acifc_basic_use_case_usage Usage Steps
 * \subsection acifc_basic_use_case_usage_code Example Code
 * Add the following to, for example, the main loop in your application C-file:
 * \snippet acifc_example.c acifc_define_channel
 * \snippet acifc_example.c acifc_struct_declaration
 * \snippet acifc_example.c acifc_init_and_configure
 * \code while (!ac_is_comparison_done(&ac_device)); \endcode
 *
 * \subsection acifc_basic_use_case_usage_flow Workflow
 * -# Get the default configuration and initialize the module:
 * \snippet acifc_example.c acifc_init_and_configure_1
 * -# Enable the module:
 * \code  ac_enable(&ac_device); \endcode
 * -# Get the default configuration to initialize channel 0:
 * \snippet acifc_example.c acifc_define_channel
 * \snippet acifc_example.c acifc_init_and_configure_2
 * -# Start a single comparison:
 * \code  ac_user_trigger_single_comparison(&ac_device); \endcode
 * -# Check if the comparison has completed:
 * \code  while (!ac_is_comparison_done(&ac_device)); \endcode
 */

#endif /* ACIFC_H_INCLUDED */
