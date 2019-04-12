#include "main.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint32_t cnt = 0;

// Delay function
void delay(unsigned int nCount)
{
    unsigned int i, j;

    for (i = 0; i < nCount; i++)
        for (j = 0; j < 0x2AFF; j++);
}

void initLeds(){
	// тактирование порта G
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;

	GPIOG->MODER |= GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0;

	// PP OD
	// GPIOG->OTYPER |= GPIO_OTYPER_OT0;

	// port output speed register
	GPIOG->OSPEEDR |= GPIO_OSPEEDR_OSPEED1_1;

	GPIOG->BSRR |= GPIO_BSRR_BS13 | GPIO_BSRR_BS14;
}


void initTimer(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	//  Установить предделитель чтоб таймер не тикал быстро (TIMx_PSC)
	TIM6->PSC = 8000 - 1;
	// Задать предел до которого таймер должен дотикать перед своим сбросом (TIMx_ARR)
	TIM6->ARR = 1000;
	// Включить отсчет битом CEN в регистре TIMx_CR1
	// разрешаем прерывание от таймера
	TIM6->CR1 |= TIM_CR1_CEN;
	// Включить прерывание по переполнению битом UIE в регистре TIMx_DIER
	TIM6->DIER |= TIM_DIER_UIE;

	 // При выполнении следующей строки генерируется прерывание
	// (при этом сам таймер пока ещё остановлен: бит включения
	// счёта TIM_CR1_CEN сброшен в 0).
	// TIM6->EGR|=TIM_EGR_UG;

	// Разрешение TIM6_DAC_IRQn прерывания
	NVIC_EnableIRQ(TIM6_DAC_IRQn);

	// __enable_irq();
}

int main() {

	initLeds();
	initTimer();

	while(1){

	}

	return 0;
}

void TIM6_DAC_IRQHandler(void){
	TIM6->SR &= ~TIM_SR_UIF;
	cnt++;

	GPIOG->ODR ^= ~GPIO_ODR_OD13;

	/* if(TIM6->SR & TIM_SR_UIF){
		if((GPIOG->ODR & GPIO_ODR_OD13) != 0) {
			GPIOG->BSRR ^= ~GPIO_BSRR_BS13;
		} else {
			GPIOG->BSRR |= GPIO_BSRR_BS13;
		}
	} */
}
