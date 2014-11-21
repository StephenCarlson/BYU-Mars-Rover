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
//         24           P0.0            Digital Input       Init(1), Pullup     WIZ_INT
//         25           P0.1            Digital Input       Init(0)             WIZ_RDY
//         26           P0.2            Digital Output      Init(1), Pullup     WIZ_RST
//         27           P0.3            Digital Output      Init(1)             WIZ_SS
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
	for(uint8_t i=0; i<100; i++) if(WIZ_RDY_Read()) continue; else CyDelay(1);
	WIZ_RST_Write(0); // This resets the 5500 IC and the on-board PIC uC
	CyDelay(10);
	WIZ_RST_Write(1); // The PIC uC configures the 5500 MAC, finished in 33 ms.
	CyDelay(50); // This (50ms) allows a margin if the PIC takes its time.
	wiznetInit(3u,1u,27015u);
	
	
	// Compose Tasks and Routines
	
	// Application Init and Warmup
	printf("BYU Mars Rover 2015\n");
	
	sys_health.systemState = STARTUP;

	rover_status.systemState = STARTUP;
	rover_status.roverVoltage = 12268;
	rover_status.roverCurrent = 200;
	rover_status.mAhCounter = 784;
	rover_status.dragonLinkRSSI = 50;


	imu.xacc = 	-14;
	imu.yacc = 	68;
	imu.zacc = 	-510;
	imu.xgyro =	0;
	imu.ygyro =	14;
	imu.zgyro =	2;
	imu.xmag = 	214;
	imu.ymag = 	15;
	imu.ymag = 	127;

	lrs.ppmCh1 = 1479;
	lrs.ppmCh2 = 1513;
	lrs.ppmCh3 = 1501;
	lrs.ppmCh4 = 1442;
	lrs.ppmCh5 = 1089;
	lrs.ppmCh6 = 1101;
	lrs.ppmCh7 = 1178;
	lrs.ppmCh8 = 2011;

	drive.driveFwd	= 11;
	drive.driveTurn	= -3;

	video.gimbalPan		= 0;
	video.gimbalTilt	= -40;
	video.gimbalZoom	= 0;
	video.camSelect		= 0;

	liferay.laserDutyCycle = 0;
	
	// *((char *) &custom1) = 'T';
	// for(uint8_t i=0; i<sizeof(custom1); i++){
	// 	*((char *) &custom1 + i) = 'T';
		// printf("%x",((uint8_t *) &custom1)[i]);
	// }
	
	// const char testArray[] = "TestTest";
	// memcpy((void *) &custom1,(void *)testArray,sizeof(testArray));
	

	// Console Hints Printouts
	// printHelpInfo();

	// Development/Debugging
	
}

void loop(void){
	// printf("%d\n",SW_Read());
	// static uint16_t forcedWritePointer = 0;
	
	LED_Write(!WIZ_RDY_Read()); // Warns if the Wiznet Ready Line isn't high.
	
	if(WIZ_INT_Read() == LOW){
		wiznetClearInterrupts();
		LED_Write(HIGH);
	}
	
	if(BUTTON_PRESSED){
		wiznetClearInterrupts();
		// uint8_t testPacket[] = "Test Packet! 16"; //{0x00,0x55,0x55,0x55,0x55,0xFE}; //
		// wiznetWriteUdpFrame(GPS_STRING,sizeof(GPS_STRING));
		// wiznetSend();
		
		// wiznetWriteUdpFrame(((uint8_t *) &custom1),sizeof(custom1));
		// wiznetSend();
		
		// wiznetWriteUdpFrame(((void *) &lrs),16);
		// wiznetSend();
		
		// gps_position.lat = ;
		
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_SYS_HEALTH_MSG_ID,sys_health);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_ROVER_STATUS_MSG_ID,rover_status);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_ARM_STATUS_MSG_ID,arm_status);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_GPS_MSG_ID,gps);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_IMU_MSG_ID,imu);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_LRS_MSG_ID,lrs);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_DRIVE_MSG_ID,drive);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_VIDEO_MSG_ID,video);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_ARM_MSG_ID,arm);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_ISHAAMA_MSG_ID,ishaama);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_LIFERAY_MSG_ID,liferay);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_CUSTOM_DEBUG_1_MSG_ID,custom_debug_1);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_CUSTOM_DEBUG_2_MSG_ID,custom_debug_2);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_CUSTOM_DEBUG_3_MSG_ID,custom_debug_3);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_PING_MSG_ID,ping);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_TELEMETRY_CONFIG_MSG_ID,telemetry_config);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_REQ_FIELDS_ONCE_MSG_ID,req_fields_once);
		roverlinkSendFrame(BASESTATION_1,ROVERLINK_BROADCAST_MSG_ID,broadcast);
		
		// while(BUTTON_PRESSED);
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
	
	
	// CyDelay(100);
	// CyDelayUs();
	LED_Write(0); // turn LED off
}

uint8_t cypressInit(void){
	WIZ_RST_Write(1);
	SPIM_Start();
	UART_Start();
	init_printf(NULL,putdata);
	
	
	//if(SW1_Read()) {  // if button is not being pressed
            
	LED_Write(0); // turn LED off
	
	
	
	CyGlobalIntEnable;
	return 1;
}

// void roverlinkSendFrame(uint8_t srcAddr, uint8_t dstAddr, uint8_t wrReqRes, uint8_t msgId, void *payload, uint8_t len){
void roverlinkSendFrame(uint8_t dstAddr, uint8_t msgId, void *payload){
	uint8_t srcAddr = board_id; // MAIN_CONTROLLER; // SYSTEM_ID;
	uint8_t wrReqRes = 0x00; // WRITE_BIT | REQUEST_BIT; //REQUEST_BIT WRITE_BIT
	
	static uint8_t seqCount = 0;
	
	// Slightly Dangerous way of generating the len variable; relies on 0-15 and 60-63. ) if ROVERLINK_REQUEST Packet
	uint8_t len = (wrReqRes == ROVERLINK_REQUEST)? 0 : (msgId<16)? ROVERLINK_MSG_LEN_ARRAY[msgId] :
			((msgId > 59) && (msgId<64))? ROVERLINK_MSG_LEN_ARRAY[msgId-44] : 0;
	
	
	#ifdef ROVERLINK_PREAMBLE_SYNC_BYTE
		#define ROVERLINK_HEADER_LEN 5
		uint8_t header[ROVERLINK_HEADER_LEN] = {0xFE,((srcAddr<<4)&0xF0)|(dstAddr&0x0F),(ROVERLINK_RESPONSE&0xC0)|(msgId&0x3F),seqCount,len};
	#else
		#define ROVERLINK_HEADER_LEN 4
		uint8_t header[ROVERLINK_HEADER_LEN] = {((srcAddr<<4)&0xF0)|(dstAddr&0x0F),(ROVERLINK_RESPONSE&0xC0)|(msgId&0x3F),seqCount,len};
	#endif

	wiznetWriteUdpFrame(header,ROVERLINK_HEADER_LEN);
	// printf("Length = %u\n",len);
	if(len > 0) wiznetWriteUdpFrame((uint8_t *)payload,len);
	wiznetSend();
	
	seqCount++;	
}









