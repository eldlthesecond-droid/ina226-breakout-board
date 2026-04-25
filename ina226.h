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

#include "main.h"

/* Registers */
#define INA226_REG_CONFIG      0x00
#define INA226_REG_SHUNT_V     0x01
#define INA226_REG_BUS_V       0x02
#define INA226_REG_POWER       0x03
#define INA226_REG_CURRENT     0x04
#define INA226_REG_CALIB       0x05

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint16_t address;        // HAL expects 8-bit address
    float current_lsb;
    float power_lsb;
} INA226_t;

/* API */
HAL_StatusTypeDef INA226_Init(INA226_t *dev, I2C_HandleTypeDef *hi2c, uint8_t addr_7bit);
HAL_StatusTypeDef INA226_Configure(INA226_t *dev, uint16_t config);
HAL_StatusTypeDef INA226_Calibrate(INA226_t *dev, float shunt, float max_current);

HAL_StatusTypeDef INA226_ReadCurrent(INA226_t *dev, float *current);
HAL_StatusTypeDef INA226_ReadPower(INA226_t *dev, float *power);
HAL_StatusTypeDef INA226_ReadShuntVoltage(INA226_t *dev, float *v);
HAL_StatusTypeDef INA226_ReadBusVoltage(INA226_t *dev, float *v);

void INA226_Setup(INA226_t *dev, I2C_HandleTypeDef *hi2c, uint8_t addr);

#endif
