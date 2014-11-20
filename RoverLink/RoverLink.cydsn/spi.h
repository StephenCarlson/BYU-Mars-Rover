#ifndef CYPRESS_ROVER_SPI_DEF_H
#define CYPRESS_ROVER_SPI_DEF_H

#include <project.h>
#include <stdint.h>

// Trying the C99 Definition, as per http://www.greenend.org.uk/rjk/tech/inline.html
// inline uint8_t transferSPI(uint8_t);


// Try defining the whole thing in the header:
// inline uint8_t transferSPI(uint8_t data){ // This results in compile failure, must be static
static inline uint8_t transferSPI(uint8_t data){
	SPIM_WriteTxData(data);
	while(!(SPIM_ReadTxStatus() & SPIM_STS_SPI_DONE));
	return( ((uint8_t) (SPIM_ReadRxData())) );
}

#endif // CYPRESS_ROVER_SPI_DEF_H