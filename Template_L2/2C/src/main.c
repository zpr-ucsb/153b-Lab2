/*
 * ECE 153B - Spring 2023
 *
 * Name(s):
 * Section:
 * Lab: 2C
 */
 
#include "stm32l476xx.h"

#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

char strTime[12] = {0};
char strDate[12] = {0};

void RTC_Set_Alarm(void) {
	RTC_Disable_Write_Protection();

	RTC->CR &= ~(RTC_CR_ALRAE); 
	RTC->CR &= ~(RTC_CR_ALRBE);	
	RTC->CR &= ~(RTC_CR_ALRAIE);
	RTC->CR &= ~(RTC_CR_ALRBIE);

	while ((RTC->CR & RTC_ISR_ALRAWF == 0) || (RTC->CR & RTC_ISR_ALRBWF == 0)) {}

	// Clear Alarm A Trigger Register & set masks to 1-4 dont care
	RTC->ALRMAR &= 0;				 
	RTC->ALRMAR |= RTC_ALRMAR_MSK4;
	RTC->ALRMAR |= RTC_ALRMAR_MSK3;
	RTC->ALRMAR |= RTC_ALRMAR_MSK2;		
	RTC->ALRMAR |= RTC_ALRMAR_ST_1;
	RTC->ALRMAR |= RTC_ALRMAR_ST_0;		

	// Clear Alarm B Trigger Register & set masks to 1-4 dont care
	RTC->ALRMBR &= 0;					
	RTC->ALRMBR |= RTC_ALRMBR_MSK1;
	RTC->ALRMBR |= RTC_ALRMBR_MSK2;
	RTC->ALRMBR |= RTC_ALRMBR_MSK3;
	RTC->ALRMBR |= RTC_ALRMBR_MSK4;
	
	// Enable alarms and alarm interrupts
	RTC->CR |= RTC_CR_ALRAE; 
	RTC->CR |= RTC_CR_ALRBE; 
	RTC->CR |= RTC_CR_ALRAIE; 
	RTC->CR |= RTC_CR_ALRBIE; 

	RTC_Enable_Write_Protection();
}

void RTC_Alarm_Enable(void) {
	EXTI->RTSR1 |= EXTI_RTSR1_RT18;		// Configure the interrupt to trigger on the rising edge in EXTI_RT18.
	EXTI->IMR1 |= EXTI_IMR1_IM18;			// Set the interrupt mask in EXTI_IMR18 
	EXTI->EMR1 |= EXTI_EMR1_EM18;			// and the event mask in EXTI_EMR18.
	EXTI->PR1 |= EXTI_PR1_PIF18;			// Clear the pending interrupt in EXTI_PR18 by writing a 1 to the bit
	NVIC_EnableIRQ(RTC_Alarm_IRQn);		
	NVIC_SetPriority(RTC_Alarm_IRQn, 0);
}

void RTC_Alarm_IRQHandler(void) {
	EXTI->PR1 |= EXTI_PR1_PIF18;    	// Clear Interrupt Pending Bit

	if (RTC->ISR & RTC_ISR_ALRAF) {		// If Alarm A was triggered:
		RTC->ISR &= ~(RTC_ISR_ALRAF);		// Clear Alarm A Event Flag
		RTC_Disable_Write_Protection();		
		RTC->CR ^= RTC_CR_ALRBE;				// Disable Alarm B
		Green_LED_Toggle();							// Toggle Light		
		RTC_Enable_Write_Protection();
	}

	if (RTC->ISR & RTC_ISR_ALRBF) {		// If Alarm B was triggered:
		RTC->ISR &= ~(RTC_ISR_ALRBF);		// Clear Alarm B Event Flag
		Green_LED_Toggle();							// Toggle Light
	}
}

void EnableAlarmA(void) {

	RTC_Disable_Write_Protection();		// Disable Write Protection

	RTC->CR |= RTC_CR_ALRAE; 			// Enable Alarm A
	RTC->CR |= RTC_CR_ALRAIE; 			// Enable Alarm A Interrupt
	RTC->CR &= ~(RTC_CR_ALRBE);			// Disable Alarm B
	RTC->CR &= ~(RTC_CR_ALRBIE);		// Disable Alarm B Interrupt

	RTC_Enable_Write_Protection();		// Enable Write Protection

}

void EnableAlarmB(void) {

	RTC_Disable_Write_Protection();		// Disable Write Protection

	RTC->CR |= RTC_CR_ALRBE; 			// Enable Alarm B
	RTC->CR |= RTC_CR_ALRBIE; 			// Enable Alarm B Interrupt
	RTC->CR &= ~(RTC_CR_ALRAE);			// Disable Alarm A
	RTC->CR &= ~(RTC_CR_ALRAIE);		// Disable Alarm A Interrupt

	RTC_Enable_Write_Protection();		// Enable Write Protection

}


int main(void) {	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	LED_Init();
	
	RTC_Init();
	RTC_Alarm_Enable();
	RTC_Set_Alarm();
	
	EnableAlarmB();
	
	while(1) {
		Get_RTC_Calendar(strTime, strDate);
	}
}