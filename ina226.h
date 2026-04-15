/*
 * INA226 Driver for STM32 (HAL I2C)
 * ----------------------------------
 * Simple, beginner-friendly driver for Texas Instruments INA226.
 *
 * MIT License
 * 
 * Copyright (c) 2026 [Emidio Albuquerque]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef INA226_H
#define INA226_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32fxxx_hal.h"

/* ---------------- I2C ADDRESS ---------------- */
#define INA226_ADDR (0x40 << 1)   // default 7-bit address shifted for STM32 HAL

/* ---------------- REGISTERS ---------------- */
#define INA226_REG_CONFIG     0x00
#define INA226_REG_SHUNT_V    0x01
#define INA226_REG_BUS_V      0x02
#define INA226_REG_POWER      0x03
#define INA226_REG_CURRENT    0x04
#define INA226_REG_CALIB      0x05
#define INA226_REG_MASK       0x06
#define INA226_REG_ALERT      0x07

/* ---------------- FUNCTION PROTOTYPES ---------------- */
HAL_StatusTypeDef INA226_WriteRegister(uint8_t reg, uint16_t value);
HAL_StatusTypeDef INA226_ReadRegister(uint8_t reg, uint16_t *value);

void INA226_Init(void);
void INA226_SetConfig(uint16_t config);
void INA226_SetCalibration(uint16_t calib);

/* Measurements */
float INA226_ReadBusVoltage_V(void);
float INA226_ReadCurrent_A(float current_LSB);
float INA226_ReadPower_W(float power_LSB);

#ifdef __cplusplus
}
#endif

#endif /* INA226_H */
