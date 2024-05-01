// Written by: Rivoo Bagchi BGCRIV001
// Date created: 30/04/2024
// Programmed in: STM32CubeIDE
// Target: STM32F051
// Description: MEC4126F

// DEFINES AND INCLUDES
#define STM32F051
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"
#define HIGH 1
#define LOW 0

// GLOBAL VARIABLES
enum motion{back=-1, stop, forward, right, left};
struct robot_status{
	enum motion movement;
	_Bool ir_left;
	_Bool ir_right;
} robot_state;

// FUNCTION DECLARATIONS
void ResetClockTo48Mhz(void);
void main(void);
void init_TIM3(void);
void init_TIM14 (void);

// MAIN FUNCTION
void main(void)
{
  	ResetClockTo48Mhz();
    // Power on phase
	// Robot_state initialised
	robot_state.movement = stop;
	robot_state.ir_left = LOW;
	robot_state.ir_right = LOW;
  	while(1)
	{

	}
}

// OTHER FUNCTIONS
/* Description:
* This function resets the STM32 Clocks to 48 MHz
*/
void ResetClockTo48Mhz(void)
{
    if ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)
    {
        RCC->CFGR &= (uint32_t) (~RCC_CFGR_SW);
        while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
    }
    RCC->CR &= (uint32_t)(~RCC_CR_PLLON);
    while ((RCC->CR & RCC_CR_PLLRDY) != 0);
    RCC->CFGR = ((RCC->CFGR & (~0x003C0000)) | 0x00280000);
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);
    RCC->CFGR |= (uint32_t) (RCC_CFGR_SW_PLL);
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void init_TIM3(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1 | GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1;
    GPIOB->AFR[0] |= 0x01 << (4 * 4) | 0x01 << (5 * 4) | 0x01 << (0 * 4) | 0x01 << (1 * 4);
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    TIM3->PSC = 31;
    TIM3->ARR = 100;
    TIM3->CCR1 = 50;
    TIM3->CCR2 = 50;
    TIM3->CCR3 = 50;
    TIM3->CCR4 = 50;


    TIM3->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
    TIM3->CCER |= TIM_CCER_CC1E;


    TIM3->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
    TIM3->CCER |= TIM_CCER_CC2E | TIM_CCER_CC2P;


    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
    TIM3->CCER |= TIM_CCER_CC3E;


    TIM3->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4PE;
    TIM3->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P;

    TIM3->CR1 |= TIM_CR1_CEN;
}

void init_TIM14 (void) {
	RCC -> APB1ENR |= RCC_APB1ENR_TIM14EN;
	TIM14 -> ARR = 369;
	TIM14 -> PSC = 6486;
	TIM14 -> DIER |= TIM_DIER_UIE;
	TIM14 -> CR1 |=TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM14_IRQn);
}

// INTERRUPT HANDLERS --------------------------------------------------------|
