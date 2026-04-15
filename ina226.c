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

/* External I2C handle (defined in main.c or i2c.c) */
extern I2C_HandleTypeDef hi2c1;

/* =========================================================
 * LOW LEVEL I2C FUNCTIONS
 * ========================================================= */

HAL_StatusTypeDef INA226_WriteRegister(uint8_t reg, uint16_t value)
{
    uint8_t data[2];

    data[0] = (value >> 8) & 0xFF;  // MSB
    data[1] = value & 0xFF;         // LSB

    return HAL_I2C_Mem_Write(
        &hi2c1,
        INA226_ADDR,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        2,
        HAL_MAX_DELAY
    );
}

HAL_StatusTypeDef INA226_ReadRegister(uint8_t reg, uint16_t *value)
{
    uint8_t data[2];

    HAL_StatusTypeDef ret = HAL_I2C_Mem_Read(
        &hi2c1,
        INA226_ADDR,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        data,
        2,
        HAL_MAX_DELAY
    );

    if (ret == HAL_OK)
    {
        *value = (data[0] << 8) | data[1];
    }

    return ret;
}

/* =========================================================
 * INITIALISATION
 * ========================================================= */

void INA226_SetConfig(uint16_t config)
{
    INA226_WriteRegister(INA226_REG_CONFIG, config);
}

void INA226_SetCalibration(uint16_t calib)
{
    INA226_WriteRegister(INA226_REG_CALIB, calib);
}

void INA226_Init(void)
{
    /* Simple default configuration:
     * Continuous mode, stable readings (example)
     */
    INA226_SetConfig(0x4127);

    /* Example calibration (USER MUST ADJUST FOR SHUNT) */
    INA226_SetCalibration(0x068E);
}

/* =========================================================
 * MEASUREMENTS
 * ========================================================= */

float INA226_ReadBusVoltage_V(void)
{
    uint16_t raw;
    INA226_ReadRegister(INA226_REG_BUS_V, &raw);

    /* LSB = 1.25 mV */
    return raw * 0.00125f;
}

float INA226_ReadCurrent_A(float current_LSB)
{
    uint16_t raw;
    INA226_ReadRegister(INA226_REG_CURRENT, &raw);

    return (int16_t)raw * current_LSB;
}

float INA226_ReadPower_W(float power_LSB)
{
    uint16_t raw;
    INA226_ReadRegister(INA226_REG_POWER, &raw);

    return raw * power_LSB;
}
