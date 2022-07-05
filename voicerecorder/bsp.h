#ifndef BSP_H_
#define BSP_H_
#include "stm32g0xx.h"


void EEPROM_INIT();
void WriteSingleByte(uint8_t devAddr, uint16_t destAddr, uint8_t data);
void WriteMultipleByte(uint8_t devAddr, uint16_t startAddr, uint8_t* data, uint8_t size);
void ReadSingleByte(uint8_t devAddr, uint16_t resAddr, uint8_t* data);
void ResetDisplay();

void ReadMultipleByte(uint8_t devAddr, uint16_t resAddr, uint8_t* data, uint16_t size);

#endif
