#ifndef WIZNET5500_DEF_H
#define WIZNET5500_DEF_H

#include <stdint.h>




// Block Select Values
#define WIZNET_BLK_COMMON	0b00000
#define WIZNET_BLK_S0_REG	0b00001
#define WIZNET_BLK_S0_TX	0b00010
#define WIZNET_BLK_S0_RX	0b00011
#define WIZNET_BLK_S1_REG	0b00101
#define WIZNET_BLK_S1_TX	0b00110
#define WIZNET_BLK_S1_RX	0b00111
// We don't need to use more than two sockets

// Common Register Addresses (Block Select = 00000 only)
#define WIZNET_MR			0x0000	// Mode Register
#define WIZNET_GAR			0x0001	// Gateway Address
#define WIZNET_SUBR			0x0005	// Subnet Mask Address
#define WIZNET_SHAR			0x0009	// Source Hardware Address
#define WIZNET_SIPR			0x000F	// Source IP Address
#define WIZNET_INTLEVEL		0x0013	// Interrupt Low-Level Timer
#define WIZNET_IR			0x0015	// Interrupt
#define WIZNET_IMR			0x0016	// Interrupt Mask
#define WIZNET_SIR			0x0017	// Socket Interrupt
#define WIZNET_SIMR			0x0018	// Socket Interrupt Mask
// Skip a bunch that we don't care about
#define WIZNET_PHYCFGR		0x002E	// PHY Configuration


// Socket Register Addresses (Block Select = 00001,00101,01001, etc)
#define WIZNET_Sn_MR		0x0000	// Socket Mode
	#define WIZNET_Sn_MR_UDP		0x02	// UDP is 0010
#define WIZNET_Sn_CR		0x0001	// Socket Command
	#define WIZNET_Sn_CR_OPEN		0x01	// Initialize and open
	#define WIZNET_Sn_CR_CLOSE		0x10	// Close
	#define WIZNET_Sn_CR_SEND		0x20	// Send Packet via ARP Address Resolution
	#define WIZNET_Sn_CR_SEND_MAC	0x21	// Send Packet using MAC address, no ARP/IP
	#define WIZNET_Sn_CR_RECV		0x40	// Enables Receiving Processing
#define WIZNET_Sn_IR		0x0002	// Socket Interrupt (Read-Only)
#define WIZNET_Sn_SR		0x0003	// Socket Status (Read-Only)
	#define WIZNET_Sn_SR_SOCK_UDP	0x22	// Socket is in UDP mode and is OPEN
#define WIZNET_Sn_PORT		0x0004	// Socket Source Port
#define WIZNET_Sn_DHAR		0x0006	// Socket Destination Hardware Address
#define WIZNET_Sn_DIPR		0x000C	// Socket Destination IP Address
#define WIZNET_Sn_DPORT		0x0010	// Socket Destination Port
#define WIZNET_Sn_MSSR		0x0012	// Socket Max Segment Size
// Skip a few
#define WIZNET_Sn_RXBUF_SZ	0x001E	// Socket Receive Buffer Size
#define WIZNET_Sn_TXBUF_SZ	0x001F	// Socket Transmit Buffer Size
#define WIZNET_Sn_TX_FSR	0x0020	// Socket Transmit Free Size (Read-Only)
#define WIZNET_Sn_TX_RD		0x0022	// Socket Transmit Read Pointer (Read-Only)
#define WIZNET_Sn_TX_WR		0x0024	// Socket Transmit Write Pointer
#define WIZNET_Sn_RX_RSR	0x0026	// Socket Receive Data Size (Read-Only)
#define WIZNET_Sn_RX_RD		0x0028	// Socket Receive Read Pointer
#define WIZNET_Sn_RX_WR		0x002A	// Socket Receive Write Pointer (Read-Only)
#define WIZNET_Sn_IMR		0x002C	// Socket Interrupt Mask
	#define WIZNET_Sn_IMR_SEND_OK	0x10 // 0b00010000
	#define WIZNET_Sn_IMR_RECV		0x04 // 0b00000100
// Skip the rest





void wiznetInit(uint8_t, uint8_t, uint16_t);
void wiznetOpen(void);
void wiznetClose(void);
void wiznetSend(void);
// void wiznetReceive(void);
// uint8_t wiznetReadInterrupts(void);
void wiznetClearInterrupts(void);
void wiznetConfigInterrupts(uint8_t, uint8_t);
uint16_t wiznetWriteUdpFrame(uint8_t *, uint16_t);
void wiznetPrintRegisters(uint8_t);

void wiznetWrite8b(uint16_t, uint8_t, uint8_t);
void wiznetWrite16b(uint16_t, uint8_t, uint16_t);
void wiznetWriteArray(uint16_t, uint8_t, uint8_t *, uint8_t);
uint8_t wiznetRead8b(uint16_t, uint8_t);
uint16_t wiznetRead16b(uint16_t, uint8_t);
void wiznetReadArray(uint16_t , uint8_t , uint8_t *, uint8_t );



#endif // WIZNET5500_DEF_H