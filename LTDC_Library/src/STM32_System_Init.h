#ifndef __STM32_INIT_LTDC_H__
#define __STM32_INIT_LTDC_H__

// Include Arduino header file.
#include "Arduino.h"

// Include header file for addidional HAL modules that needs to be enabled.
//#include "hal_conf_extra.h"

// Include STM32 HAL Files
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_sram.h"
#include "stm32f7xx_hal_ltdc.h"

// Include GPIO Pin names.
#include "STM32_Pins.h"

// Include library defines.
#include "defines.h"

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

// Init for LCD Driver for STM32
void MX_LTDC_Init(void);

// Init for DMA for loading the data into framebuffer quickly.
void MX_DMA_Init(void);

// Init for the FMC (flexable memory controller) for accessing external RAM.
// Framebuffer of the LCD is stored inside external RAM.
void MX_FMC_Init(void);

// Must be declared as extern "C" for HAL library in the Arduino STM32 Core can find them.
extern "C" void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc);
extern "C" void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef* hltdc);
extern "C" void HAL_FMC_MspInit(void);
extern "C" void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* hsdram);
extern "C" void HAL_FMC_MspDeInit(void);
extern "C" void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* hsdram);

// Return the DMA instance (needed in the library).
DMA_HandleTypeDef *getDMAInstance();

// Return the SDRAM instance (needed in the library).
SDRAM_HandleTypeDef *getSDRAMInstance();

extern "C" void LTDC_IRQHandler(void);

#endif