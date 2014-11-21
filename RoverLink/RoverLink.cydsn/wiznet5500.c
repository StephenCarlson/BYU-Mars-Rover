// BYU Mars Rover WizNet Library
// Authors:
// 		Steve Carlson
// 		<Your Name Here>

// Totally Bare-bones, written-in-24-hours.
// Recommend using https://github.com/Wiznet/WIZ_Ethernet_Library later

#include <project.h>
#include <stdint.h>
#include "wiznet5500.h"
#include "spi.h"
// #include "rover_main.h" // For some reason, including this breaks it all. 
// 		Only needed for the following macros:
#define HIGH			1
#define LOW				0

// Application-Scope Defines

#define WIZ_MAX_MESSAGE_SEGMENT		100

// Global Variables
static uint16_t wiznetTxPointer;

void wiznetInit(uint8_t ownIpAddr, uint8_t dstIpAddr, uint16_t dstUdpPort){
	// This is terrible, need a better way.
	uint8_t cmdArray[] = {0x00,192u,168u,1u,1u,255u,255u,255u,0u,0x00,0x08,0xDC,0xFF,0xFF,0xFF,192u,168u,1u,254u};
	//                    --- | Gateway Addr  | Subnet Mask     | WizNet's MAC Address        | WizNet's Own IP Address

	uint8_t udpConfig[] = {0x02,0x00,0xFF,0x00,0xFE,0x01,0x00,0x00,0x00,0x00,0x00,0x00,192u,168u,1u,1u,0x69,0x87};
	//                     UDP |IDLE|Clr |----| :65025  |  No Dst Hw Address          | 192.168.1.1   | :27015
	
	
	// Destination IP Address Ending
	// cmdArray[4] = dstIpAddr; // Gateway should always be 192.168.1.1
	udpConfig[15] = dstIpAddr;
	udpConfig[16] = dstUdpPort>>8;
	udpConfig[17] = dstUdpPort&0xFF;
	
	// Learn Current MAC
	uint8_t wiznetOldMacAddr[6];
	wiznetReadArray(WIZNET_SHAR, WIZNET_BLK_COMMON, wiznetOldMacAddr, 6); // sizeof(wiznetOldMacAddr)
	
	// WizNet Module IP Address Ending
	cmdArray[18] = ownIpAddr;
	
	// Ephemeral Port (WizNet's own UDP Port when sending UDP, usually value above 0xC000)
	uint16_t ephemUdpPort = (0xC000 + ((wiznetOldMacAddr[5]*(1+ownIpAddr))&0x3FFF) ); //Nice way to generate unique Ephemeral Port
	udpConfig[4] = ephemUdpPort>>8;
	udpConfig[5] = ephemUdpPort&0xFF;
	
	
	// Write all the thus prepared values to the WizNet Module and Open the Socket
	wiznetWriteArray(WIZNET_MR,WIZNET_BLK_COMMON,cmdArray,sizeof(cmdArray));
	wiznetWriteArray(WIZNET_Sn_MR,WIZNET_BLK_S0_REG,udpConfig,sizeof(udpConfig));
	wiznetWriteArray(WIZNET_SHAR,WIZNET_BLK_COMMON,wiznetOldMacAddr,6);
	wiznetWrite16b(WIZNET_Sn_MSSR,WIZNET_BLK_S0_REG,WIZ_MAX_MESSAGE_SEGMENT);
	CyDelay(100);
	wiznetOpen();
	
	// Configure Wiznet Interrupts
	wiznetConfigInterrupts(0x00,WIZNET_Sn_IMR_SEND_OK);

	// Write sequential values to the TX Buffer, to know where on earth it is writing from.
	WIZ_SS_Write(LOW); // Begin a WizNet SPI Frame. Three Phases: Address, Control, Data
		// Address Phase
		transferSPI(0>>8);
		transferSPI(0&0xFF);
		
		// Control Phase
		transferSPI(((WIZNET_BLK_S0_TX&0x1F)<<3)|(0x01<<2)|0x00);		
		
		// Data Phase
		for(uint16_t k=0; k<1024; k++){
			transferSPI((uint8_t)(k>>8));
			transferSPI((uint8_t)(k&0xFF));
		}
	WIZ_SS_Write(HIGH); // WizNet SPI Frame Finished


}

// Public Functions
void wiznetOpen(void){
	wiznetWrite8b(WIZNET_Sn_CR,WIZNET_BLK_S0_REG,0x01); // OPEN UDP on Socket 0
}

void wiznetClose(void){
	wiznetWrite8b(WIZNET_Sn_CR,WIZNET_BLK_S0_REG,0x10); // Close UDP on Socket 0
}

void wiznetSend(void){
	wiznetWrite16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG,wiznetTxPointer);
	wiznetWrite8b(WIZNET_Sn_CR,WIZNET_BLK_S0_REG,0x20);
	wiznetTxPointer = wiznetRead16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG);
}

// void wiznetReceive(void){
	
// }

// uint8_t wiznetReadInterrupts(void){
// }

void wiznetClearInterrupts(void){
	wiznetWrite8b(WIZNET_Sn_IR,WIZNET_BLK_S0_REG, 0xFF);
	wiznetWrite8b(WIZNET_SIR,WIZNET_BLK_COMMON, 0xFF);
	wiznetWrite8b(WIZNET_IR,WIZNET_BLK_COMMON, 0xFF); // Counter-intuitive, but datasheet specifies writing '1' to clear to '0'.
}

void wiznetConfigInterrupts(uint8_t commonInt, uint8_t socketInt){
	wiznetWrite8b(WIZNET_Sn_IMR,WIZNET_BLK_S0_REG, socketInt);
	wiznetWrite8b(WIZNET_SIMR,WIZNET_BLK_COMMON, 0x01); // Hardwired to just Socket 0
	wiznetWrite8b(WIZNET_IMR,WIZNET_BLK_COMMON, commonInt);
}

uint16_t wiznetWriteUdpFrame(uint8_t *array, uint16_t len){
	
	// wiznetTxPointer = wiznetRead16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG);
	// wiznetWriteArray(wiznetTxPointer,WIZNET_BLK_S0_TX,array,len);
	// wiznetTxPointer += len;																	
	// wiznetWrite16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG,wiznetTxPointer);
	
	// Re-ordered the above and put the pointer manip in the Send method due to fluke.
	wiznetWriteArray(wiznetTxPointer,WIZNET_BLK_S0_TX,array,len);
	wiznetTxPointer += len;
	
	// CyDelay(10); // These two lines proved that the fluke with the WizNet is not time-variant
	// wiznetRead16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG);
	return wiznetTxPointer;
}

void wiznetPrintRegisters(uint8_t blkSelect){
	uint8_t readArray[256];
	wiznetReadArray(0, blkSelect, readArray, 255);
	
	printf("\n\t");
	for(uint8_t c=0; c<16; c++)	printf("%X\t",c);
	printf("\n");
	for(uint8_t j=0; j<8; j++){
		printf("%X\t",j);
		for(uint8_t k=0; k<16; k++){
			printf("%X\t",readArray[(16*j)+k]);
		}
		printf("\n");
	}
	printf("\n");
}

// Private-like Functions, but visible until this is re-written in C++
void wiznetWrite8b(uint16_t regAddr, uint8_t blkSel, uint8_t data){
	WIZ_SS_Write(LOW); // Begin a WizNet SPI Frame. Three Phases: Address, Control, Data
		// Address Phase
		transferSPI(regAddr>>8);
		transferSPI(regAddr&0xFF);
		
		// Control Phase
		transferSPI(((blkSel&0x1F)<<3)|(0x01<<2)|0x00);		
		
		// Data Phase
		transferSPI(data);
	WIZ_SS_Write(HIGH); // WizNet SPI Frame Finished
	// return response;
}

void wiznetWrite16b(uint16_t regAddr, uint8_t blkSel, uint16_t data){
	WIZ_SS_Write(LOW); // Begin a WizNet SPI Frame. Three Phases: Address, Control, Data
		// Address Phase
		transferSPI(regAddr>>8);
		transferSPI(regAddr&0xFF);
		
		// Control Phase
		transferSPI(((blkSel&0x1F)<<3)|(0x01<<2)|0x00);		
		
		// Data Phase
		transferSPI(data>>8);
		transferSPI(data&0xFF);
	WIZ_SS_Write(HIGH); // WizNet SPI Frame Finished
	// return response;
}

void wiznetWriteArray(uint16_t regAddr, uint8_t blkSel, uint8_t *srcArray, uint8_t len){
	WIZ_SS_Write(LOW); // Begin a WizNet SPI Frame. Three Phases: Address, Control, Data
		// Address Phase
		transferSPI(regAddr>>8);
		transferSPI(regAddr&0xFF);
		
		// Control Phase
		transferSPI(((blkSel&0x1F)<<3)|(0x01<<2)|0x00);		
		
		// Data Phase
		for(uint8_t k=0; k<len; k++){
			transferSPI(srcArray[k]);
		}
	WIZ_SS_Write(HIGH); // WizNet SPI Frame Finished
	// return response;
}

uint8_t wiznetRead8b(uint16_t regAddr, uint8_t blkSel){
	WIZ_SS_Write(LOW); // Begin a WizNet SPI Frame. Three Phases: Address, Control, Data
		// Address Phase
		transferSPI(regAddr>>8);
		transferSPI(regAddr&0xFF);
		
		// Control Phase
		transferSPI(((blkSel&0x1F)<<3)|(0x00<<2)|0x00);		
		
		// Data Phase
		uint8_t result = transferSPI(0x00);
	WIZ_SS_Write(HIGH); // WizNet SPI Frame Finished
	return result;
}

uint16_t wiznetRead16b(uint16_t regAddr, uint8_t blkSel){
	uint16_t result;
	WIZ_SS_Write(LOW); // Begin a WizNet SPI Frame. Three Phases: Address, Control, Data
		// Address Phase
		transferSPI(regAddr>>8);
		transferSPI(regAddr&0xFF);
		
		// Control Phase
		transferSPI(((blkSel&0x1F)<<3)|(0x00<<2)|0x00);		
		
		// Data Phase
		result = transferSPI(0x00)<<8;
		result |= transferSPI(0x00);
	WIZ_SS_Write(HIGH); // WizNet SPI Frame Finished
	return result;
}

void wiznetReadArray(uint16_t regAddr, uint8_t blkSel, uint8_t *dstArray, uint8_t len){
	WIZ_SS_Write(LOW); // Begin a WizNet SPI Frame. Three Phases: Address, Control, Data
		// uint8_t count = 0;
		
		// Address Phase
		transferSPI(regAddr>>8);
		transferSPI(regAddr&0xFF);
		
		// Control Phase
		transferSPI(((blkSel&0x1F)<<3)|(0x00<<2)|0x00);	
		
		// Data Phase
		for(uint8_t k=0; k<len; k++){
			dstArray[k] = transferSPI(0x00);
			// count++;
			// printf("%u",k);
		}
	WIZ_SS_Write(HIGH); // WizNet SPI Frame Finished
	// return count;
}

/* As the following code has comments, just use a define as a glorified comment block: */
#ifdef WIZNET_NOTES_ALPHA_DO_NOT_COMPILE_THIS
	// The following is what was in the if(BUTTON_PRESSED) function in the main loop.
	LED_Write(1);
			
	// Endian-ness is killing the attempts to read and write the ring pointers, fatal blunder.
	uint16_t wiznetTxPointer;
	uint8_t testPacket[] = {0x00,0x55,0x55,0x55,0x55,0xFE}; //"Test Packet!";
	
	wiznetRead16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG,wiznetTxPointer);			// Read the Current Pointer Position
	printf("%u\t",wiznetTxPointer);														
	
	wiznetWriteArray(wiznetTxPointer,WIZNET_BLK_S0_TX,testPacket,sizeof(testPacket));	// Record the array at that point
	
	wiznetRead16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG,wiznetTxPointer);			// Does the WizNet Update this for us? Nope, unchanged
	printf("%u\n",wiznetTxPointer);														
	
	// wiznetWriteArray(0,WIZNET_BLK_S0_TX,testPacket,sizeof(testPacket));
	wiznetTxPointer += 8;																// Lets force the increment beyond what it should be.		
	wiznetWrite16b(WIZNET_Sn_TX_WR,WIZNET_BLK_S0_REG,wiznetTxPointer);
	// forcedWritePointer += 24;
	// wiznetWriteArray(0x24,WIZNET_BLK_S0_REG,((uint8_t*) &forcedWritePointer),2);
	
	wiznetSend();								// Initiate Send
	
	CyDelay(100);
	
	// wiznetPrintRegisters(WIZNET_BLK_COMMON);
	wiznetPrintRegisters(WIZNET_BLK_S0_REG);
	wiznetPrintRegisters(WIZNET_BLK_S0_TX);
	
	while(BUTTON_PRESSED);
	CyDelay(100);






#endif








