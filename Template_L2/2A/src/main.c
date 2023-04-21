/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include "stm32l476xx.h"

void GPIO_Init(void){	
	// Enable GPIO Clock For Port C
	RCC->AHB2ENR |= (1<<2);

	// Enable OUTPUT FOR PINS 5,6,8,9
	GPIOC->MODER &= ~(1<<11); //5
	GPIOC->MODER &= ~(1<<13); //6
	GPIOC->MODER &= ~(1<<17); //8
	GPIOC->MODER &= ~(1<<19); //9

	//Output Speed
	GPIOC->OSPEEDR |= (1<<11);
	GPIOC->OSPEEDR |= (1<<13);
	GPIOC->OSPEEDR |= (1<<17);
	GPIOC->OSPEEDR |= (1<<19);

	GPIOC->OSPEEDR &= (~(1<<10));
	GPIOC->OSPEEDR &= (~(1<<12));
	GPIOC->OSPEEDR &= (~(1<<16));
	GPIOC->OSPEEDR &= (~(1<<18));

	//Output Type Push-Pull
	GPIOC->OTYPER &= (~(1<<5));
	GPIOC->OTYPER &= (~(1<<6));
	GPIOC->OTYPER &= (~(1<<8));
	GPIOC->OTYPER &= (~(1<<9));

	// Pull Up No Pull Down
	GPIOC->PUPDR &= ~((1<<11)+(1<<10));
	GPIOC->PUPDR &= ~((1<<13)+(1<<12));
	GPIOC->PUPDR &= ~((1<<17)+(1<<16));
	GPIOC->PUPDR &= ~((1<<19)+(1<<18));
}


#define DELAY 6000	// delay between steps of the sequences


void Full_Stepping_Clockwise(void){
	// [TODO]

	if ( ((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD9) == GPIO_ODR_OD9) )  {
		GPIOC->ODR |= GPIO_ODR_OD5;			// AB' -> AB
		GPIOC->ODR &= ~(GPIO_ODR_OD6);
		GPIOC->ODR |= GPIO_ODR_OD8;			
		GPIOC->ODR &= ~(GPIO_ODR_OD9);
	}

	else if ( ((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8) )  {
		GPIOC->ODR &= ~(GPIO_ODR_OD5);		// AB -> A'B
		GPIOC->ODR |= GPIO_ODR_OD6;			
		GPIOC->ODR |= GPIO_ODR_OD8;			
		GPIOC->ODR &= ~(GPIO_ODR_OD9);
	}

	else if ( ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) && ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8) )  {
		GPIOC->ODR &= ~(GPIO_ODR_OD5);		// A'B -> A'B'
		GPIOC->ODR |= GPIO_ODR_OD6;	
		GPIOC->ODR &= ~(GPIO_ODR_OD8);
		GPIOC->ODR |= GPIO_ODR_OD9;
	}

	else if ( ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) && ((GPIOC->ODR & GPIO_ODR_OD9) == GPIO_ODR_OD9) )  {
		GPIOC->ODR |= GPIO_ODR_OD5;			// A'B' -> AB
		GPIOC->ODR &= ~(GPIO_ODR_OD6);
		GPIOC->ODR &= ~(GPIO_ODR_OD8);
		GPIOC->ODR |= GPIO_ODR_OD9;
	}
}

void Full_Stepping_CounterClockwise(void){
	// [TODO]

	// 5: A
	// 6: A'
	// 8: B
	// 9: B'

	if ( ((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD9) == GPIO_ODR_OD9) )  {
		GPIOC->ODR &= ~(GPIO_ODR_OD5);		
		GPIOC->ODR |= GPIO_ODR_OD6;
		GPIOC->ODR &= ~GPIO_ODR_OD8;			
		GPIOC->ODR |= (GPIO_ODR_OD9);
	}

	else if ( ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) && ((GPIOC->ODR & GPIO_ODR_OD9) == GPIO_ODR_OD9) )  {
		GPIOC->ODR &= ~(GPIO_ODR_OD5);			
		GPIOC->ODR |= GPIO_ODR_OD6;
		GPIOC->ODR |= GPIO_ODR_OD8;			
		GPIOC->ODR &= ~(GPIO_ODR_OD9);
	}

	else if ( ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) && ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8) )  {
		GPIOC->ODR |= (GPIO_ODR_OD5);			
		GPIOC->ODR &= ~GPIO_ODR_OD6;
		GPIOC->ODR |= GPIO_ODR_OD8;			
		GPIOC->ODR &= ~(GPIO_ODR_OD9);
	}

	else if ( ((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8) )  {
		GPIOC->ODR |= (GPIO_ODR_OD5);			
		GPIOC->ODR &= ~GPIO_ODR_OD6;
		GPIOC->ODR &= ~GPIO_ODR_OD8;			
		GPIOC->ODR |= (GPIO_ODR_OD9);
	}

}

void Half_Stepping_Clockwise(void){
	// [TODO]	

	if (((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD8) != GPIO_ODR_OD8) 
        && ((GPIOC->ODR & GPIO_ODR_OD6) != GPIO_ODR_OD6)) {
        GPIOC->ODR &= ~GPIO_ODR_OD9;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD8;
    } 
    else if (((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8) 
        && ((GPIOC->ODR & GPIO_ODR_OD6) != GPIO_ODR_OD6)) {
        GPIOC->ODR &= ~GPIO_ODR_OD5;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD6;
    } 
    else if (((GPIOC->ODR & GPIO_ODR_OD5) != GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8)
        && ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6)) {
        GPIOC->ODR &= ~GPIO_ODR_OD8;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD9;
    }
    else if (((GPIOC->ODR & GPIO_ODR_OD5) != GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD8) != GPIO_ODR_OD8)
        && ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6)) {
        GPIOC->ODR &= ~GPIO_ODR_OD6;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD5;
    }
}

void Half_Stepping_CounterClockwise(void){
	// [TODO]

	if ( ((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD6) != GPIO_ODR_OD6) 
	&& ((GPIOC->ODR & GPIO_ODR_OD8) != GPIO_ODR_OD8) && ((GPIOC->ODR & GPIO_ODR_OD9) == GPIO_ODR_OD9) ) {
        GPIOC->ODR &= ~GPIO_ODR_OD5;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD6;
    } 

	else if ( ((GPIOC->ODR & GPIO_ODR_OD5) != GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) 
	&& ((GPIOC->ODR & GPIO_ODR_OD8) != GPIO_ODR_OD8) && ((GPIOC->ODR & GPIO_ODR_OD9) == GPIO_ODR_OD9) ) {
        GPIOC->ODR &= ~GPIO_ODR_OD9;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD8;
    }

	else if ( ((GPIOC->ODR & GPIO_ODR_OD5) != GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD6) == GPIO_ODR_OD6) 
	&& ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8) && ((GPIOC->ODR & GPIO_ODR_OD9) != GPIO_ODR_OD9) ) {
        GPIOC->ODR &= ~GPIO_ODR_OD6;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD5;
    }

	else if ( ((GPIOC->ODR & GPIO_ODR_OD5) == GPIO_ODR_OD5) && ((GPIOC->ODR & GPIO_ODR_OD6) != GPIO_ODR_OD6) 
	&& ((GPIOC->ODR & GPIO_ODR_OD8) == GPIO_ODR_OD8) && ((GPIOC->ODR & GPIO_ODR_OD9) != GPIO_ODR_OD9) ) {
        GPIOC->ODR &= ~GPIO_ODR_OD8;
        for(int i = 0; i < DELAY; i++);
        GPIOC->ODR |= GPIO_ODR_OD9;
    }
}


int main(void){
	GPIO_Init();
	
	// Rotate 360 degrees either clockwise or counter-clockwise
	// [TODO]

	GPIOC->ODR |= GPIO_ODR_OD5;		
	GPIOC->ODR &= ~(GPIO_ODR_OD6);
	GPIOC->ODR &= ~(GPIO_ODR_OD8);
	GPIOC->ODR |= GPIO_ODR_OD9;

	while(1) {
		Full_Stepping_Clockwise();
		// Full_Stepping_CounterClockwise();
		// Half_Stepping_Clockwise();
		// Half_Stepping_CounterClockwise();

		for (int i = 0; i < DELAY; i++) {
		}

	}
}