#include "bsp.h"
#include "stm32g0xx.h"
#include <stdlib.h>
#include <stdbool.h>

#define EEPROM_ADR1 0x50
#define EEPROM_ADR2 0x54

#define MAX_TRACK_BYTE_SIZE 32000


typedef enum STATE
{
	START,
	IDLE,
	RECORD,
	PLAY,
	STATUS,
	FULL,
	
}STATE;

typedef enum TRACK
{
	TRACK1 = 1,
	TRACK2,
	TRACK3,
	TRACK4
}TRACK;




uint8_t* buffer_write;
uint8_t* buffer_read;
uint16_t bw_index = 0;
uint16_t read_index = 0;
uint16_t record_size = 0;
uint16_t curr_memaddr = 0;
uint8_t curr_devaddr = EEPROM_ADR1;
bool can_read = true;
bool canButtonPress = true;
uint8_t buttonPressCounter = 0;
uint16_t idle_counter = 0;
char disp_letters[4];
uint8_t track_size = 0;
uint8_t record_time = 5;
uint16_t read_init_memaddr = 0;
uint8_t curr_readdevaddr = EEPROM_ADR1;
uint16_t curr_readmemaddr = 0;
bool playable_track[4];




TRACK selected_track;

STATE curr_state;


void delay(volatile unsigned int s)
{
	for (; s > 0; s--);
}

void clearRow(void) {
	GPIOB->BRR = (1U << 6);
	GPIOB->BRR = (1U << 7);
	GPIOA->BRR = (1U << 0);
}
void setRow(void) {
	GPIOB->ODR |= (1U << 6);
	GPIOB->ODR |= (1U << 7);
	GPIOA->ODR |= (1U << 0);
}


void SetStateProperty(STATE state)
{
	switch(state)
	{
	    case START:
		disp_letters[0] = '1';
		disp_letters[1] = '8';
		disp_letters[2] = '9';
		disp_letters[3] = '8';
	    break;

		case IDLE:
		disp_letters[0] = 'i';
		disp_letters[1] = 'd';
		disp_letters[2] = 'l';
		disp_letters[3] = 'e';
		break;
		case FULL:
		disp_letters[0] = 'f';
		disp_letters[1] = 'u';
		disp_letters[2] = 'l';
		disp_letters[3] = 'l';
		break;
		case RECORD:
		disp_letters[0] = 'r';
		disp_letters[1] = 'c';
		disp_letters[2] = 'd';
		disp_letters[3] = IntToChar(record_time);
		break;
		case PLAY:
		disp_letters[0] = 'p';
		disp_letters[1] = 'l';
		disp_letters[2] = 'b';
		disp_letters[3] = IntToChar(selected_track);
		break;
		case STATUS:
		disp_letters[0] = 'a';
		disp_letters[1] = 'v';
		disp_letters[2] = 'a';
		disp_letters[3] = IntToChar(track_size);
		break;
		default:
		break;
	}
}

void INIT_PWM()
{
	RCC->APBENR1 |= (1U << 1);

	GPIOB->MODER &= ~(3U << 2 * 4);
	GPIOB->MODER |= (2U << 2 * 4);

	GPIOB->AFR[0] |= 1U << 4 * 4;

	TIM3->CCR1 = 500;

	TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);

	TIM3->CCMR1 |= (TIM_CCMR1_OC1PE);

	TIM3->CR1 |= TIM_CR1_ARPE;

	TIM3->CCER &= ~(TIM_CCER_CC1P);

	TIM3->CCER |= TIM_CCER_CC1E;

	TIM3->PSC = 2;

	TIM3->ARR = 255;

	TIM3->DIER = (1 << 0);
	TIM3->CR1 = (1 << 0);
}

void INIT_TIMER2()
{
	RCC->APBENR1 |= (1U);

	TIM2->CR1 = 0;
	TIM2->CR1 |= (1 << 7);
	TIM2->CNT = 0;

	// 6300Hz(Sampling Freq) = 1600000 / (ARR + 1) * (PSC + 1)
	TIM2->PSC = 1;
	TIM2->ARR = 1268;

	TIM2->DIER = (1 << 0);
	TIM2->CR1 = (1 << 0);

	NVIC_SetPriority(TIM2_IRQn, 0);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void INIT_TIMER14()
{
	RCC->APBENR2 |= (1U << 15);

	TIM14->CR1 = 0;
	TIM14->CR1 |= (1 << 7);
	TIM14->CNT = 0;

	TIM14->PSC = 1;
	TIM14->ARR = 16000;//1MS

	TIM14->DIER = (1 << 0);
	TIM14->CR1 = (1 << 0);

	NVIC_SetPriority(TIM14_IRQn, 0);
	NVIC_EnableIRQ(TIM14_IRQn);
}

void PlaySound(uint8_t sound)
{
	TIM3->CCR1 = (uint32_t)sound;
}

void TIM2_IRQHandler()
{
	if(curr_state == RECORD)
	{
		ADC1->CR |= (1U << 2);

		while (!(ADC1->ISR & (1U << 2))) {}

		if (bw_index < 128)
		{
			buffer_write[bw_index] = ADC1->DR;
			++bw_index;
		}
		if (bw_index >= 127)
		{

			WriteMultipleByte(curr_devaddr, curr_memaddr, buffer_write, 128);
			record_size = (uint16_t)(record_size + 128);
			curr_memaddr= (uint16_t)(curr_memaddr + 128);

			// 6400 bytes(32000/5=6400)
			if(record_size == 6400 || record_size == 12800 || record_size == 19200 || record_size == 25600 || record_size == 32000)
			{
				record_time = (uint8_t)(record_time - 1);
				disp_letters[3] = IntToChar(record_time);
			}
			if (record_size >= MAX_TRACK_BYTE_SIZE)
			{

				uint8_t track_recorded = 0;
				if(curr_memaddr == 32000)
				{
					track_recorded = curr_devaddr == EEPROM_ADR1 ? 0 : 2;
					playable_track[track_recorded] = true;
				}
				else if(curr_memaddr == 64000)
				{
					track_recorded = curr_devaddr == EEPROM_ADR1 ? 1 : 3;
					playable_track[track_recorded] = true;
				}
				curr_state = IDLE;
				SetStateProperty(curr_state);
				bw_index = 0;
				record_size = 0;
				record_time = 5;
				track_size = (uint8_t)(track_size + 1);
			}
			else
			{
				bw_index = 0;
			}
			if(curr_memaddr >= MAX_TRACK_BYTE_SIZE * 2)
			{
				curr_memaddr = 0;
				curr_devaddr = curr_devaddr == EEPROM_ADR2 ? EEPROM_ADR1 : EEPROM_ADR2;
			}


		}
	}

	else if(curr_state == PLAY)
	{
		if(can_read)
		{
			ReadMultipleByte(curr_readdevaddr, curr_readmemaddr, buffer_read, 128);
			can_read = false;
		}

		if(read_index < 128 && !can_read)
		{
			PlaySound(buffer_read[read_index]);
			read_index++;
		}
		else if(read_index >= 128 && !can_read)
		{
			if(curr_readmemaddr >= ((read_init_memaddr + MAX_TRACK_BYTE_SIZE) - 128))
			{
				curr_readmemaddr = 0;
				curr_readdevaddr = EEPROM_ADR1;
				read_index = 0;
				curr_state = IDLE;
				SetStateProperty(curr_state);
				can_read = true;
			}
			else
			{
				curr_readmemaddr = (uint16_t)(curr_readmemaddr + 128);
				read_index = 0;
				can_read = true;
			}
		}
	}
	
	TIM2->SR &= ~(1U << 0);
}

void TIM14_IRQHandler(void)
{
	if(!canButtonPress)
	{
		idle_counter = 0;
		buttonPressCounter++;
		if (buttonPressCounter >= 100)
		{
			buttonPressCounter = 0;
			canButtonPress = true;
		}
	}
	else if(curr_state != RECORD && curr_state != PLAY && curr_state != IDLE)
	{
		idle_counter++;
		if(idle_counter >= 10000)
		{
			idle_counter = 0;
			curr_state = IDLE;
			SetStateProperty(curr_state);
		}
	}

	uint8_t offset = 0;
	int iterator;
	for(iterator = 3; iterator >= 0; --iterator)
	{
		DisplayChar(disp_letters[iterator]);
		delay(40);
		ResetDisplay();
		ShiftDigit((unsigned int)offset);
		offset = (uint8_t)(offset + 1);
	}
	TIM14->SR &= ~(1U << 0);
}

void EXTI4_15_IRQHandler(void)
{
  if (((GPIOB->IDR >> 5) & 1))
  {
	if(canButtonPress)
	{
		clearRow();
		GPIOB->ODR ^= (1U << 7);
		if(((GPIOB->IDR >> 5) & 1))
		{
			canButtonPress = false;
			selected_track = TRACK1;
		}
		GPIOB->ODR ^= (1U << 7);

		GPIOB->ODR ^= (1U << 6);
		if((((GPIOB->IDR >> 5) & 1)))
		{
			canButtonPress = false;
			selected_track = TRACK4;
		}
		GPIOB->ODR ^= (1U << 6);

		GPIOA->ODR ^= (1U << 0);
		if((((GPIOB->IDR >> 5) & 1)))
		{
			canButtonPress = false;
			SetStateProperty(curr_state);

		}
		GPIOA->ODR ^= (1U << 0);
		setRow();
	}
	EXTI->RPR1 |= (1U << 5);
  }
  if (((GPIOA->IDR >> 11) & 1))
  {
	if(canButtonPress)
	{
		clearRow();
		GPIOB->ODR ^= (1U << 7);
		if (((GPIOA->IDR >> 11) & 1) && curr_state != PLAY)
		{
			canButtonPress = false;
			selected_track = TRACK2;
		}
		GPIOB->ODR ^= (1U << 7);

		GPIOB->ODR ^= (1U << 6);
		if((((GPIOA->IDR >> 11) & 1)) && selected_track > 0)
		{
			canButtonPress = false;
			bool pass_init = true;

			if(curr_state == PLAY && playable_track[selected_track-1])
			{
				pass_init = false;
				SetStateProperty(curr_state);
			}

			else if(playable_track[selected_track-1])
			{
				pass_init = false;
				curr_state = PLAY;
				SetStateProperty(curr_state);
			}

			if(pass_init && playable_track[selected_track-1])
			{
				if((selected_track == TRACK1 && playable_track[0]) || (selected_track == TRACK2 && playable_track[1]))
				{
					curr_readdevaddr = EEPROM_ADR1;
					curr_readmemaddr = selected_track == TRACK1 ? 0 : 32000;
					curr_state = PLAY;
					SetStateProperty(curr_state);
				}
				else if((selected_track == TRACK3 && playable_track[2]) || (selected_track == TRACK4 && playable_track[3]))
				{
					curr_readdevaddr = EEPROM_ADR2;
					curr_readmemaddr = selected_track == TRACK3 ? 0 : 32000;
					curr_state = PLAY;
					SetStateProperty(curr_state);
				}
				read_init_memaddr = curr_readmemaddr;
			}
		}
		GPIOB->ODR ^= (1U << 6);

		GPIOA->ODR ^= (1U << 0);
		if ((((GPIOA->IDR >> 11) & 1)))
		{
			canButtonPress = false;
			curr_state = STATUS;
			SetStateProperty(curr_state);
		}
		GPIOA->ODR ^= (1U << 0);
		setRow();
	}
	EXTI->RPR1 |= (1U << 11);
  }
  if (((GPIOA->IDR >> 12) & 1))
  {
	if(canButtonPress)
	{
		clearRow();
		GPIOB->ODR ^= (1U << 7);
		if (((GPIOA->IDR >> 12) & 1))
		{
			canButtonPress = false;
			selected_track = TRACK3;
		}
		GPIOB->ODR ^= (1U << 7);

		GPIOB->ODR ^= (1U << 6);
		if ((((GPIOA->IDR >> 12) & 1)))
		{
			canButtonPress = false;
			if(track_size < 4)
			{
				curr_state = RECORD;
				SetStateProperty(curr_state);
				if(!playable_track[0] || !playable_track[1])
				{
					curr_devaddr = EEPROM_ADR1;
					curr_memaddr = !playable_track[0] ? 0 : 32000;
				}
				else if(!playable_track[2] || !playable_track[3])
				{
					curr_devaddr = EEPROM_ADR2;
					curr_memaddr = !playable_track[2] ? 0 : 32000;
				}
			}
			else
			{
				curr_state = FULL;
				SetStateProperty(curr_state);
			}
		}
		GPIOB->ODR ^= (1U << 6);

		GPIOA->ODR ^= (1U << 0);
		if ((((GPIOA->IDR >> 12) & 1)))
		{
			canButtonPress = false;
		}
		GPIOA->ODR ^= (1U << 0);
		setRow();
	}
	EXTI->RPR1 |= (1U << 12);
  }
}
int main(void)
{
	buffer_write = (uint8_t*)malloc(sizeof(uint8_t) * 128);
	buffer_read = (uint8_t*)malloc(sizeof(uint8_t) * 128);

	playable_track[0]=playable_track[1]=playable_track[2]=playable_track[3]=false;

	curr_state = START;
	SetStateProperty(curr_state);


	ADCInit();
	INIT_PWM();
	EEPROM_INIT();
	INIT_TIMER14();
	Keypad_SSD_GPIO_Init();
	INIT_TIMER2();
	while (1)
	{

	}
	free(buffer_read);
	free(buffer_write);
}

void ADCInit()
{
	RCC->IOPENR |= (3U);

	RCC->APBENR2 |= (1U << 20);

	ADC1->CR |= (1U << 28);
	delay(300000);

	ADC1->CFGR1 |= (1U << 4);

	ADC1->CR |= (1U << 31);
	while (!(ADC1->ISR & (1U << 11)));

	ADC1->SMPR |= (1U);

	ADC1->CHSELR |= (1U << 1);

	ADC1->CR |= (1U);
	while (!(ADC1->ISR & 1U));
}

void Keypad_SSD_GPIO_Init(void)
{
	GPIOB->MODER &= ~(3U << 2 * 6);
	GPIOB->MODER |= (1U << 2 * 6);

	GPIOB->MODER &= ~(3U << 2 * 7);
	GPIOB->MODER |= (1U << 2 * 7);

	GPIOA->MODER &= ~(3U << 2 * 0);
	GPIOA->MODER |= (1U << 2 * 0);

	GPIOB->MODER &= ~(3U << 2 * 5);
	GPIOB->PUPDR |= (2U << 2 * 5);

	GPIOA->MODER &= ~(3U << 2 * 11);
	GPIOA->PUPDR |= (2U << 2 * 11);

	GPIOA->MODER &= ~(3U << 2 * 12);
	GPIOA->PUPDR |= (2U << 2 * 12);

	GPIOB->MODER &= ~(3U << 2 * 0);
	GPIOB->MODER |= (1U << 2 * 0);
	GPIOB->ODR |= (1U << 1 * 0);

	GPIOB->ODR |= (1U << 6);
	GPIOB->ODR |= (1U << 7);
	GPIOA->ODR |= (1U << 0);

	EXTI->EXTICR[1] |= (1U << 8 * 1);
	EXTI->RTSR1 |= (1U << 5);
	EXTI->IMR1 |= (1U << 5);

	EXTI->EXTICR[2] |= (0U << 8 * 3);
	EXTI->RTSR1 |= (1U << 11);
	EXTI->IMR1 |= (1U << 11);

	EXTI->EXTICR[3] |= (0U << 8 * 0);
	EXTI->RTSR1 |= (1U << 12);
	EXTI->IMR1 |= (1U << 12);

	NVIC_SetPriority(EXTI4_15_IRQn, 0);
	NVIC_EnableIRQ(EXTI4_15_IRQn);

}
