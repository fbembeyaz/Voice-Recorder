#include "bsp.h"

#include <string.h>

void EEPROM_INIT()
{
	//PB8 as AF6
	GPIOB->MODER &= ~(3U << 2*8);
	GPIOB->MODER |= (2U << 2*8);
	GPIOB->OTYPER|= (1U << 8);

	GPIOB->AFR[1] &= ~(0xFU << 4*0);
	GPIOB->AFR[1] |= (6 << 4*0);

	//PB9 as AF6
	GPIOB->MODER &= ~(3U << 2*9);
	GPIOB->MODER |= (2U << 2*9);
	GPIOB->OTYPER|= (1U << 9);

	GPIOB->AFR[1] &= ~(0xFU << 4*1);
	GPIOB->AFR[1] |= (6 << 4*1);

	
	RCC->APBENR1 |= (1U << 21);

	I2C1->CR1 = 0;
	I2C1->CR1 |= (1 << 7);


	I2C1->TIMINGR |= (3 << 28);
	I2C1->TIMINGR |= (0x13 << 0);
	I2C1->TIMINGR |= (0xF << 8);
	I2C1->TIMINGR |= (0x2 << 16);
	I2C1->TIMINGR |= (0x4 << 20);

	I2C1->CR1 = (1U << 0);

	NVIC_SetPriority(I2C1_IRQn,0);
	NVIC_EnableIRQ(I2C1_IRQn);
}


void SetSegmentA() {
	GPIOA->MODER &= ~(3U << 2 * 4);
	GPIOA->MODER |= (1U << 2 * 4);
}

void SetSegmentB() {
	GPIOA->MODER &= ~(3U << 2 * 5);
	GPIOA->MODER |= (1U << 2 * 5);
}

void SetSegmentC() {
	GPIOA->MODER &= ~(3U << 2 * 6);
	GPIOA->MODER |= (1U << 2 * 6);
}
void SetSegmentD() {
	GPIOA->MODER &= ~(3U << 2 * 7);
	GPIOA->MODER |= (1U << 2 * 7);
}
void SetSegmentE() {
	GPIOA->MODER &= ~(3U << 2 * 8);
	GPIOA->MODER |= (1U << 2 * 8);
}
void SetSegmentF() {
	GPIOA->MODER &= ~(3U << 2 * 9);
	GPIOA->MODER |= (1U << 2 * 9);
}
void SetSegmentG() {

	GPIOA->MODER &= ~(3U << 2 * 10);
	GPIOA->MODER |= (1U << 2 * 10);
}

void SetNumberZero()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();
	SetSegmentF();
}
void SetNumberOne()
{
	SetSegmentB();
	SetSegmentC();
}
void SetNumberTwo()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentD();
	SetSegmentE();
	SetSegmentG();
}
void SetNumberThree()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentC();
	SetSegmentD();
	SetSegmentG();

}
void SetNumberFour()
{
	SetSegmentB();
	SetSegmentC();
	SetSegmentF();
	SetSegmentG();
}
void SetNumberFive()
{
	SetSegmentA();
	SetSegmentC();
	SetSegmentD();
	SetSegmentF();
	SetSegmentG();
}
void SetNumberSix()
{
	SetSegmentA();
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();
	SetSegmentF();
	SetSegmentG();
}
void SetNumberSeven()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentC();
}
void SetNumberEight()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();
	SetSegmentF();
	SetSegmentG();
}
void SetNumberNine()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentC();
	SetSegmentD();
	SetSegmentF();
	SetSegmentG();
}
void SetNegativeSign()
{
	SetSegmentG();
}

void SetLetterA()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentC();
	SetSegmentE();
	SetSegmentF();
	SetSegmentG();
}
void SetLetterB()
{
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();
	SetSegmentF();
	SetSegmentG();
}
void SetLetterC()
{
	SetSegmentD();
	SetSegmentE();
	SetSegmentG();
}
void SetLetterD()
{
	SetSegmentB();
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();
	SetSegmentG();
}
void SetLetterE()
{
	SetSegmentA();
	SetSegmentD();
	SetSegmentE();
	SetSegmentF();
	SetSegmentG();
}
void SetLetterI()
{
	SetSegmentE();
	SetSegmentF();
}
void SetLetterN()
{
	SetSegmentC();
	SetSegmentE();
	SetSegmentG();
}
void SetLetterV()
{
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();

}
void SetLetterO()
{
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();
	SetSegmentG();
}
void SetLetterU()
{
	SetSegmentB();
	SetSegmentC();
	SetSegmentD();
	SetSegmentE();
	SetSegmentF();
}
void SetLetterF()
{
	SetSegmentA();
	SetSegmentE();
	SetSegmentF();
	SetSegmentG();
}
void SetLetterL()
{
	SetSegmentD();
	SetSegmentE();
	SetSegmentF();
}
void SetLetterP()
{
	SetSegmentA();
	SetSegmentB();
	SetSegmentE();
	SetSegmentF();
	SetSegmentG();
}
void SetLetterR()
{
	SetSegmentE();
	SetSegmentG();
}

void TurnBeginning()
{
	GPIOB->MODER &= ~(3U << 2 * 0);
	GPIOB->MODER |= (1U << 2 * 0);
	GPIOB->BRR = (1U << 0);
	GPIOB->ODR |= (1U << 0);
	GPIOB->MODER &= ~(0U << 2 * 1);
	GPIOB->BRR = (1U << 1);
	GPIOB->MODER &= ~(0U << 2 * 2);
	GPIOB->BRR = (1U << 2);

	GPIOB->MODER &= ~(0U << 2 * 3);
	GPIOB->BRR = (1U << 3);
}

void ShiftDigit(unsigned int currIndex)
{
	if (currIndex >= 3)
	{
		TurnBeginning();
		return;
	}
	GPIOB->MODER &= ~(0U << 2 * (currIndex));
	GPIOB->MODER &= ~(3U << 2 * (currIndex + 1));
	GPIOB->MODER |= (1U << 2 * (currIndex + 1));
	GPIOB->BRR = (1U << currIndex);
	GPIOB->ODR |= (1U << (currIndex + 1));

}

void DisplayChar(char ch)
{
	switch (ch)
	{
	case '0':
		SetNumberZero();
		break;
	case '1':
		SetNumberOne();
		break;
	case '2':
		SetNumberTwo();
		break;
	case '3':
		SetNumberThree();
		break;
	case '4':
		SetNumberFour();
		break;
	case '5':
		SetNumberFive();
		break;
	case '6':
		SetNumberSix();
		break;
	case '7':
		SetNumberSeven();
		break;
	case '8':
		SetNumberEight();
		break;
	case '9':
		SetNumberNine();
		break;
	case 'a':
		SetLetterA();
		break;
	case 'b':
		SetLetterB();
		break;
	case 'c':
		SetLetterC();
		break;
	case 'd':
		SetLetterD();
		break;
	case 'e':
		SetLetterE();
		break;
	case 'i':
		SetLetterI();
		break;
	case 'n':
		SetLetterN();
		break;
	case 'v':
		SetLetterV();
		break;
	case 'o':
		SetLetterO();
		break;
	case 'u':
		SetLetterU();
		break;
	case 'f':
		SetLetterF();
		break;
	case 'l':
		SetLetterL();
		break;
	case 'p':
		SetLetterP();
		break;
	case 'r':
		SetLetterR();
		break;
	default:
		break;
	}
}

char IntToChar(uint8_t digit)
{
	switch (digit)
	{
	  case 0:
		return '0';
	  case 1:
		return '1';
	  case 2:
		return '2';
	  case 3:
		return '3';
	  case 4:
		return '4';
	  case 5:
		return '5';
	  case 6:
		return '6';
	  case 7:
		return '7';
	  case 8:
		return '8';
	  case 9:
		return '9';
	  default:
		break;
	 }
	return '0';
}

void ResetDisplay()
{
	uint8_t index;
	for(index = 4;index < 11;index++)
	{
		GPIOA->MODER &= ~(3U << 2 * index);
		GPIOA->MODER |= (3U << 2 * index);
		GPIOA->ODR &= ~(1U << index);
	}

}



void WriteSingleByte(uint8_t devAddr, uint16_t destAddr, uint8_t data)
{

	I2C1->CR2=0;
	I2C1->CR2 |= ((uint32_t)(devAddr << 1));
	I2C1->CR2 |= (3U << 16);
	I2C1->CR2 |= (1U << 25);
	I2C1->CR2 |= (1U << 13);


	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(destAddr >> 8);

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(destAddr & 0xFF);

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)data;
}

void WriteMultipleByte(uint8_t devAddr, uint16_t startAddr, uint8_t* data, uint8_t size)
{
	if(size > 128 && size <= 0)
	{
		return;
	}

	I2C1->CR2=0;
	I2C1->CR2 |= ((uint32_t)(devAddr << 1));
	I2C1->CR2 |= ((uint32_t)(2U + size) << 16);
	I2C1->CR2 |= (1U << 25);
	I2C1->CR2 |= (1U << 13);


	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(startAddr >> 8);

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(startAddr & 0xFF);

	uint8_t i;
	for(i = 0; i < size; i++)
	{
		while(!(I2C1->ISR & (1 << 1)));
		I2C1->TXDR = (uint32_t)data[i];
	}
}

void ReadSingleByte(uint8_t devAddr, uint16_t resAddr, uint8_t* data)
{

	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)(devAddr << 1));
	I2C1->CR2 |= (2U << 16);
	I2C1->CR2 |= (1U << 13);

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(resAddr >> 8);
	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(resAddr & 0xFF);

	while(!(I2C1->ISR & (1 << 6)));

	I2C1->CR2=0;
	I2C1->CR2 |= ((uint32_t)(devAddr << 1));
	I2C1->CR2 |= (1U << 10);
	I2C1->CR2 |= (1U << 16);
	I2C1->CR2 |= (1U << 15);
	I2C1->CR2 |= (1U << 25);
	I2C1->CR2 |= (1U << 13);
	while(!(I2C1->ISR & (1 << 2)));
	*data = (uint8_t)I2C1->RXDR;

}

void ReadMultipleByte(uint8_t devAddr, uint16_t resAddr, uint8_t* data, uint16_t size)
{

	I2C1->CR2 = 0;
	I2C1->CR2 |= ((uint32_t)(devAddr << 1));
	I2C1->CR2 |= (2U << 16);
	I2C1->CR2 |= (1U << 13);

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(resAddr >> 8);

	while(!(I2C1->ISR & (1 << 1)));
	I2C1->TXDR = (uint32_t)(resAddr & 0xFF);

	while(!(I2C1->ISR & (1 << 6)));

	I2C1->CR2=0;
	I2C1->CR2 |= ((uint32_t)(devAddr << 1));
	I2C1->CR2 |= (1U << 10);
	I2C1->CR2 |= ((1U * size) << 16);
	I2C1->CR2 |= (1U << 25);
	I2C1->CR2 |= (1U << 13);

	uint16_t i;
	for(i = 0; i < size; ++i)
	{
		while(!(I2C1->ISR & (1 << 2)));
		data[i] = (uint8_t)I2C1->RXDR;
	}
	I2C1->CR2 |= (1U << 15);
