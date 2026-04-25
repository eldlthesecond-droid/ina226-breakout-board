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
#include "ina226.h"

/* =========================
   INTERNAL I2C FUNCTIONS
   ========================= */

static HAL_StatusTypeDef i2c_write(INA226_t *dev, uint8_t reg, uint16_t val)
{
    uint8_t data[2] = { val >> 8, val & 0xFF };

    return HAL_I2C_Mem_Write(dev->hi2c,
                             dev->address,
                             reg,
                             I2C_MEMADD_SIZE_8BIT,
                             data,
                             2,
                             100);
}

static HAL_StatusTypeDef i2c_read(INA226_t *dev, uint8_t reg, uint16_t *val)
{
    uint8_t data[2];

    HAL_StatusTypeDef st = HAL_I2C_Mem_Read(dev->hi2c,
                                            dev->address,
                                            reg,
                                            I2C_MEMADD_SIZE_8BIT,
                                            data,
                                            2,
                                            100);

    if (st != HAL_OK)
        return st;

    *val = (data[0] << 8) | data[1];
    return HAL_OK;
}

/* =========================
   INIT
   ========================= */

HAL_StatusTypeDef INA226_Init(INA226_t *dev,
                             I2C_HandleTypeDef *hi2c,
                             uint8_t addr_7bit)
{
    dev->hi2c = hi2c;
    dev->address = addr_7bit << 1; // HAL uses 8-bit
    dev->current_lsb = 0.0f;
    dev->power_lsb = 0.0f;

    return HAL_OK;
}

/* =========================
   CONFIG
   ========================= */

HAL_StatusTypeDef INA226_Configure(INA226_t *dev, uint16_t config)
{
    return i2c_write(dev, INA226_REG_CONFIG, config);
}

/* =========================
   CALIBRATION
   ========================= */

HAL_StatusTypeDef INA226_Calibrate(INA226_t *dev,
                                   float shunt,
                                   float max_current)
{
    if (shunt <= 0.0f || max_current <= 0.0f)
        return HAL_ERROR;

    dev->current_lsb = max_current / 32768.0f;

    uint16_t cal = (uint16_t)(0.00512f / (dev->current_lsb * shunt));
    if (cal == 0) cal = 1;

    dev->power_lsb = 25.0f * dev->current_lsb;

    return i2c_write(dev, INA226_REG_CALIB, cal);
}

/* =========================
   READ FUNCTIONS
   ========================= */

HAL_StatusTypeDef INA226_ReadCurrent(INA226_t *dev, float *current)
{
    uint16_t raw;

    if (i2c_read(dev, INA226_REG_CURRENT, &raw) != HAL_OK)
        return HAL_ERROR;

    *current = (int16_t)raw * dev->current_lsb;
    return HAL_OK;
}

HAL_StatusTypeDef INA226_ReadPower(INA226_t *dev, float *power)
{
    uint16_t raw;

    if (i2c_read(dev, INA226_REG_POWER, &raw) != HAL_OK)
        return HAL_ERROR;

    *power = raw * dev->power_lsb;
    return HAL_OK;
}

HAL_StatusTypeDef INA226_ReadBusVoltage(INA226_t *dev, float *v)
{
    uint16_t raw;

    if (i2c_read(dev, INA226_REG_BUS_V, &raw) != HAL_OK)
        return HAL_ERROR;

    *v = raw * 1.25e-3f;
    return HAL_OK;
}

HAL_StatusTypeDef INA226_ReadShuntVoltage(INA226_t *dev, float *v)
{
    uint16_t raw;

    if (i2c_read(dev, INA226_REG_SHUNT_V, &raw) != HAL_OK)
        return HAL_ERROR;

    *v = (int16_t)raw * 2.5e-6f;
    return HAL_OK;
}

/* =========================
   SETUP (ALL-IN-ONE)
   ========================= */

void INA226_Setup(INA226_t *dev,
                  I2C_HandleTypeDef *hi2c,
                  uint8_t addr)
{
    INA226_Init(dev, hi2c, addr);

    uint16_t config =
        (0x4 << 9) |   // AVG = 16
        (0x4 << 6) |   // VBUSCT
        (0x4 << 3) |   // VSHCT
        (0x7);         // continuous mode

    if (INA226_Configure(dev, config) != HAL_OK)
        Error_Handler();

    HAL_Delay(10);

    if (INA226_Calibrate(dev, 0.01f, 2.0f) != HAL_OK)
        Error_Handler();
}
