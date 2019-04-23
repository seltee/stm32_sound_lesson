#include <string.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

unsigned int msCounter = 0;
bool inUp = false;

// 3th octave
#define NOTE_3DO 1046
#define NOTE_3RE 1174
#define NOTE_3MI 1318
#define NOTE_3FA 1396
#define NOTE_3SO 1568
#define NOTE_3LY 1720
#define NOTE_3SI 1975

// 4th octave
#define NOTE_4DO 2093
#define NOTE_4RE 2349
#define NOTE_4MI 2637
#define NOTE_4FA 2793
#define NOTE_4SO 3136
#define NOTE_4LY 3440
#define NOTE_4SI 3951

#define WAIT 40

void initHardware(){
	// CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// Setting up timer for 1000 ms delay
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM2->PSC = 72000000 / 10000 - 1;
	TIM2->ARR = 10;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM2_IRQn);
}

extern "C" void TIM2_IRQHandler(void)
{
  TIM2->SR &= ~TIM_SR_UIF;
	msCounter++;
}

void playFreq(unsigned int freq){
	TIM3->PSC = 2000000 / freq - 1;
	TIM3->ARR = 72;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_CEN;
	
	if (freq){
		NVIC_EnableIRQ(TIM3_IRQn);
	}else{
		NVIC_DisableIRQ(TIM3_IRQn);
		inUp = false;
		GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, Bit_SET);
	}
}

extern "C" void TIM3_IRQHandler(void)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_0 | GPIO_Pin_1, inUp ? Bit_RESET : Bit_SET);
	inUp = !inUp;
	TIM3->SR &= ~TIM_SR_UIF;
}

void delay(unsigned int ms){
	ms += msCounter;
	while(msCounter < ms);
}

int main(void)
{
	// Init all of the cpu hardware. Note: display has it's own hardware initialization
	initHardware();
	
	while(1){
		//Mario
		playFreq(NOTE_4MI);
		delay(WAIT*3);
		playFreq(0);
		delay(WAIT*1);
		
		playFreq(NOTE_4MI);
		delay(WAIT*3);
		playFreq(0);
		delay(WAIT*5);
		
		playFreq(NOTE_4MI);
		delay(WAIT*7);
		playFreq(0);
		delay(WAIT*5);
		
		playFreq(NOTE_4DO);
		delay(WAIT*3);
		playFreq(0);
		delay(WAIT*1);
		
		playFreq(NOTE_4MI);
		delay(WAIT*3);
		playFreq(0);
		delay(WAIT*1);		
		
		playFreq(NOTE_4SO);
		delay(WAIT*3);
		playFreq(0);
		delay(WAIT*13);	
		
		playFreq(NOTE_3SO);
		delay(WAIT*7);
		playFreq(0);
		delay(WAIT*9);	
		
		for (int i = 0; i < 2; i++){
			playFreq(NOTE_4DO);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*9);
			
			playFreq(NOTE_3SO);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*9);
			
			playFreq(NOTE_3MI);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*9);
			
			playFreq(NOTE_3LY);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*5);
			
			playFreq(NOTE_3SI);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*5);
			
			playFreq(NOTE_3SI);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*1);
			
			playFreq(NOTE_3LY);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*5);
			
			playFreq(NOTE_3SO);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*1);
			
			playFreq(NOTE_4MI);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*1);
			
			playFreq(NOTE_4SO);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*1);
			
			playFreq(NOTE_4LY);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*5);
			
			playFreq(NOTE_4FA);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*5);
			
			playFreq(NOTE_4SO);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*5);
			
			playFreq(NOTE_4MI);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*5);
			
			playFreq(NOTE_4DO);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*1);
			
			playFreq(NOTE_4RE);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*1);
			
			playFreq(NOTE_3SI);
			delay(WAIT*3);
			playFreq(0);
			delay(WAIT*13);
		}
	}
}
