/*
 * INA226 Driver for STM32 (HAL I2C)
 * ----------------------------------
 * Simple, beginner-friendly driver for Texas Instruments INA226.
 *
 * IMPORTANT NOTICE:
 * - This code is provided "as-is" without any warranty.
 * - No guarantee of correctness, fitness for purpose, or safety.
 * - Use at your own risk.
 *
 * Copyright (c) 2026
 */

#ifndef INA226_H
#define INA226_H

#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"

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

/* Low-level I2C access */
HAL_StatusTypeDef INA226_WriteRegister(uint8_t reg, uint16_t value);
HAL_StatusTypeDef INA226_ReadRegister(uint8_t reg, uint16_t *value);

/* Setup */
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
