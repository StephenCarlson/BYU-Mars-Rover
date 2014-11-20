// BYU Mars Rover Primary Control System - Main Controller Board
// Authors:
// 		Steve Carlson
// 		<Your Name Here>
/*
// Written in Notepad++, compiled in Cypress PSOC 3.0, Tab=4, 
// 		recommend Notepad++ as it blows away PSOC's IDE: Split-screen, Search for all instances, etc
// File Manifest:
//		main.c			This file
//		rover_main.h	Main include
//		roverlink.h		RoverLink Specification, referenced by rover_main.h
// 
// Port Configuration:
//   -- IC Package -- --PCB Label--
//       TQFP-44        CY8CKIT         Periph Used         Assignment          Code Label
//         1            VSS             Gnd
//         2            P2.0
//         3            P2.1
//         4            P2.2
//         5            P2.3
//         6            P2.4
//         7            P2.5
//         8            P2.6
//         9            P2.7
//         10           VSS             Gnd
//         11           P3.0
//         12           P3.1
//         13           P3.2
//         14           P3.3
//         15           P3.4
//         16           P3.5
//         17           P3.6
//         18           P3.7
//         19           VDD             +5.0v
//         20           P4.0            SCB0 UART           SCB0:UART:RX        UART_UartGetChar(c)
//         21           P4.1            SCB0 UART           SCB0:UART:TX        printf()/UART_UartPutChar(c)
//         22           P4.2            
//         23           P4.3
//         24           P0.0            Digital Input       External Interrupt  WIZ_INT
//         25           P0.1            Digital Output                          WIZ_RDY
//         26           P0.2            Digital Output      SCB0:SPI:SS3        WIZ_RST
//         27           P0.3            Digital Output                          WIZ_SS
//         28           P0.4            SCB1 SPI            SCB1:SPI:MOSI       MOSI
//         29           P0.5            SCB1 SPI            SCB1:SPI:MISO       MISO
//         30           P0.6            SCB0 SPI            SCB0:SPI:SCLK       SCLK
//         31           P0.7            Digital Input                           SW -and- BUTTON_PRESSED
//         32           XRES
//         33           VCCD            +1.8v
//         34           VDDD            +5.0v
//         35           VDDA            +5.0v
//         36           VSSA            Gnd
//         37           P1.0
//         38           P1.1
//         39           P1.2
//         40           P1.3
//         41           P1.4
//         42           P1.5
//         43           P1.6            Digital Output                          LED
//         44           P1.7
*/


#include <project.h>		// Cypress-Specific Includes
#include "rover_main.h"		// BYU Rover Includes, architecture-agnostic.
#include "wiznet5500.h"		// Wiznet Module Library, implements rover and Cypress elements.

// CY_ISR(CY_TIMER1_OVF_vect){
	
// }

// Cypress tinyprintf
void putdata ( void* p, char c){
	if(c=='\n') UART_UartPutChar('\r');
	UART_UartPutChar(c);
}

int main(){
	setup();
	
	for(;;){
		loop();
	}
}

void setup(void){
	// Core System Init
	cypressInit();
	
	// Init Peripherals
	WIZ_RST_Write(0);
	CyDelay(100);
	WIZ_RST_Write(1);
	CyDelay(100);
	wiznetInit(3u,1u,27015u);
	
	
	// Compose Tasks and Routines
	
	// Application Init and Warmup
	printf("BYU Mars Rover 2015\n");
	
	
	

	// Console Printouts
	// printHelpInfo();

	// Development/Debugging
	
}

void loop(void){
	
	// printf("%d\n",SW_Read());
	// static uint16_t forcedWritePointer = 0;
	
	if(BUTTON_PRESSED){
		// uint8_t testPacket[] = "Test Packet! 16"; //{0x00,0x55,0x55,0x55,0x55,0xFE}; //
		wiznetWriteUdpFrame(GPS_STRING,sizeof(GPS_STRING));
		wiznetSend();
		
		while(BUTTON_PRESSED);
		CyDelay(100);
	}
	
	
	SPIM_ClearTxBuffer();
	SPIM_ClearRxBuffer();
	
	// WIZ_SS_Write(LOW);
		// transferSPI(0x55);
	// WIZ_SS_Write(HIGH);
	
	// temp = SPIM_ReadTxStatus();

	// /* Ensure that previous SPI read is done, or SPI is idle before sending data */
	// if(temp & (SPIM_STS_SPI_DONE | SPIM_STS_SPI_IDLE))
	// {
	// SPIM_WriteTxData(adcReading);
	// SPIS_ClearRxBuffer();
	// }
	
	
	CyDelay(100);
	// CyDelayUs();
	LED_Write(0); // turn LED off
}

uint8_t cypressInit(void){
	
	SPIM_Start();
	UART_Start();
	init_printf(NULL,putdata);
	
	
	//if(SW1_Read()) {  // if button is not being pressed
            
	LED_Write(0); // turn LED off
	
	
	
	CyGlobalIntEnable;
	return 1;
}



