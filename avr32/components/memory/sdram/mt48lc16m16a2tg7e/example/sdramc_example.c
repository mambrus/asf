/*****************************************************************************
 *
 * \file
 *
 * \brief SDRAMC on EBI example application.
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
 ******************************************************************************/

/*! \mainpage
 * \section intro Introduction
 * This is the documentation for the data structures, functions, variables,
 * defines, enums, and typedefs for the SDRAMC on EBI driver.
 *
 * This example demonstrates how to use the SDRAMC on EBI driver with the
 * onboard SDRAM. It initializes the SDRAM, tests it and then returns the number
 * of errors found.
 *
 * To use this code in your own application, make a pointer to the base address
 * of the SDRAM and access it like you would normally access a memory-mapped
 * device.
 *
 * \section files Main Files
 * - sdramc.c: SDRAMC on EBI driver;
 * - sdramc.h: SDRAMC on EBI driver header file;
 * - mt48lc16m16a2tg7e.h: MT48LC16M16A2TG-7E SDRAM driver;
 * - sdramc_example.c: SDRAMC on EBI example application.
 *
 * \section compilinfo Compilation Information
 * This software is written for GNU GCC for AVR32 and for IAR Embedded Workbench
 * for Atmel AVR32. Other compilers may or may not work.
 *
 * \section deviceinfo Device Information
 * All AVR32 devices with an SDRAMC and a USART module can be used.
 *
 * \section configinfo Configuration Information
 * This example has been tested with the following configuration:
 * - EVK1100, UC3C_EK, EVK1104 or EVK1105 evaluation kit;
 * - CPU clock:
 *      -- 12 MHz : EVK1100, EVK1104 or EVK1105 evaluation kit;
 *      -- 16 MHz : UC3C_EK evaluation kit;
 * - USART1 (on EVK1100) connected to a PC serial port via a standard RS232 DB9
 *   cable, or USART0 (on EVK1105) or USART1 (on EVK1104) or USART2 (on UC3C_EK)
 *   abstracted with a USB CDC connection to a PC;
 * - PC terminal settings:
 *   - 57600 bps,
 *   - 8 data bits,
 *   - no parity bit,
 *   - 1 stop bit,
 *   - no flow control.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.microchip.com/design-centers/32-bit /">Atmel AVR32</A>.\n
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */


#include "board.h"
#include "print_funcs.h"
#include "cycle_counter.h"
#include "power_clocks_lib.h"
#include "sdramc.h"


/*! \name LEDs to Use for the Example
 */
//! @{
#if BOARD == EVK1100
  #define LED_SDRAM_WRITE     LED_BI0_RED
  #define LED_SDRAM_READ      LED_BI0_GREEN
  #define LED_SDRAM_ERRORS    LED_BI1_RED
  #define LED_SDRAM_OK        LED_BI1_GREEN
#elif BOARD == EVK1104 || BOARD == EVK1105 || BOARD == UC3C_EK
  #define LED_SDRAM_WRITE     LED0
  #define LED_SDRAM_READ      LED1
  #define LED_SDRAM_ERRORS    (LED0 | LED1 | LED2 | LED3)
  #define LED_SDRAM_OK        (LED0 | LED1 | LED2 | LED3)
#else
  #error The LEDs to use for this example must be defined here.
#endif
//! @}


/*! \brief Sets the SDRAM Controller up, initializes the SDRAM found on the
 *         board and tests it.
 */
int main(void)
{
  unsigned long sdram_size, progress_inc, i, j, tmp, noErrors = 0;
  volatile unsigned long *sdram = SDRAM;

  // Switch to external oscillator 0.
  pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

  // Initialize the debug USART module.
  init_dbg_rs232(FOSC0);

  // Calculate SDRAM size in words (32 bits).
  sdram_size = SDRAM_SIZE >> 2;
  print_dbg("\x0CSDRAM size: ");
  print_dbg_ulong(SDRAM_SIZE >> 20);
  print_dbg(" MB\r\n");

  // Initialize the external SDRAM chip.
  sdramc_init(FOSC0);
  print_dbg("SDRAM initialized\r\n");

  // Determine the increment of SDRAM word address requiring an update of the
  // printed progression status.
  progress_inc = (sdram_size + 50) / 100;

  // Fill the SDRAM with the test pattern.
  for (i = 0, j = 0; i < sdram_size; i++)
  {
    if (i == j * progress_inc)
    {
      LED_Toggle(LED_SDRAM_WRITE);
      print_dbg("\rFilling SDRAM with test pattern: ");
      print_dbg_ulong(j++);
      print_dbg_char('%');
    }
    sdram[i] = i;
  }
  LED_Off(LED_SDRAM_WRITE);
  print_dbg("\rSDRAM filled with test pattern       \r\n");

  // Recover the test pattern from the SDRAM and verify it.
  for (i = 0, j = 0; i < sdram_size; i++)
  {
    if (i == j * progress_inc)
    {
      LED_Toggle(LED_SDRAM_READ);
      print_dbg("\rRecovering test pattern from SDRAM: ");
      print_dbg_ulong(j++);
      print_dbg_char('%');
    }
    tmp = sdram[i];
    if (tmp != i)
    {
      noErrors++;
    }
  }
  LED_Off(LED_SDRAM_READ);
  print_dbg("\rSDRAM tested: ");
  print_dbg_ulong(noErrors);
  print_dbg(" corrupted word(s)       \r\n");
  if (noErrors)
  {
    LED_Off(LED_SDRAM_ERRORS);
    while (1)
    {
      LED_Toggle(LED_SDRAM_ERRORS);
      cpu_delay_ms(200, FOSC0);   // Fast blink means errors.
    }
  }
  else
  {
    LED_Off(LED_SDRAM_OK);
    while (1)
    {
      LED_Toggle(LED_SDRAM_OK);
      cpu_delay_ms(1000, FOSC0);  // Slow blink means OK.
    }
  }
}
