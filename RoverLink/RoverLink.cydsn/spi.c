
#include "spi.h"
// Just noticed something about GNU static inline at http://www.greenend.org.uk/rjk/tech/inline.html
// Commenting out this file.
/*
extern uint8_t transferSPI(uint8_t data){
	SPIM_WriteTxData(data);
	while(!(SPIM_ReadTxStatus() & SPIM_STS_SPI_DONE));
	return( ((uint8_t) (SPIM_ReadRxData())) );
}

*/