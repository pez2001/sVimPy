/*-------------------------------------------------
	 mmc_lib.h C-header file for AVRGCC
	------------------------------------
	Interface for MultiMediaCard via SPI
	
	Author:
		Stefan Seegel
		dahamm@gmx.net
		ICQ UIN 14964408
		
	Thanks to:
		Ulrich Radig for his code
		Simon Lehmayr for good ideas
		ape for timing improovment
		
	Version:
		2.0 / 10.05.2005
	
	History:
		2.0 / 10.05.2005:
			- splited read & write commands in start..., read/write & stop...
			- Improved protocol handling for more supported cards
			- Improved description in header file
			- Added different SPI speeds for initialization, read and write
			- Supports SD cards
			- Added macros for CID register access
	
		1.2 / 11.08.2004:
			- Changed "mmc_send_byte()" to inline
			- Changed parameter "count" of "mmc_read_sector()" to unsigned short (16 bit)
			
		1.1 / 29.07.2004:
			-Removed typedef struct for CSD
			-Added different init-clock rate
			
		1.0 / 28.07.2004:
			-First release
	
---------------------------------------------------*/

//Port & Pin definitions. Be sure to use a pin of the same port as SPI for CS (Chip Select) !
//Settings below are recommended for a MEGA16
#ifndef MMC_LIB_H
#define MMC_LIB_H

#ifdef __cplusplus
extern "C"  {
#endif

#define MMC_PORT PORTB
#define MMC_DDR DDRB

//#define SS    2
// arduino D11
//#define MOSI  3
// arduino D12
//#define MISO  4	
// arduino D13
//#define SCK    5

#define SPI_MISO	4		//DataOut of MMC 
#define SPI_MOSI	3		//DataIn of  MMC
#define SPI_CLK  	5		//Clock of MMC
#define MMC_CS		2		//ChipSelect of MMC


//Clockrate while initialisation / reading / writing
#define SPI_INIT_CLOCK 1<<SPR1 | 1<<SPR0
#define SPI_READ_CLOCK 1<<SPR1 | 1<<SPR0
#define SPI_WRITE_CLOCK 1<<SPR1 | 0<<SPR0

#define SPI_DOUBLE_SPEED 1 //0: normal speed, 1: double speed

//MMC Commandos
#define MMC_GO_IDLE_STATE 0
#define MMC_SEND_OP_COND 1
#define MMC_SEND_CSD	9
#define MMC_SEND_CID 10
#define MMC_SET_BLOCKLEN 16
#define MMC_READ_SINGLE_BLOCK 17
#define MMC_WRITE_BLOCK 24

// Result Codes
#define MMC_OK 				0
#define MMC_INIT 1
#define MMC_NOSTARTBYTE	2
#define MMC_CMDERROR	3
#define MMC_TIMEOUT		4

extern unsigned char mmc_init(void);
/*			
*		Call mmc_init one time after a card has been connected to the µC's SPI bus!
*	
*		return values:
*			MMC_OK:				MMC initialized successfully
*			MMC_INIT:			Error while trying to reset MMC
*			MMC_TIMEOUT:	Error/Timeout while trying to initialize MMC
*/

extern unsigned char mmc_start_read_block(unsigned long adr, unsigned char adrshift);
/*
*		mmc_start_read_sector initializes the reading of a sector
*
*		Parameters:
*			adr: specifies address to be read from
*			adrshift: the specified address is shifted left by <adrshift> bits
*								e.g. if you use a blocksize of 512 bytes you should set
*								adrshift to 9 in order to specify the blocknumber in adr.
*
*		Return values:
*			MMC_OK:						Command successful
*			MMC_CMDERROR:			Error while sending read command to mmc
*			MMC_NOSTARTBYTE:	No start byte received
*		
*		Example Code:
*			unsigned char mmc_buf[512];
*			mmc_init();	//Initializes MMC / SD Card
*			set_blocklen(512);	//Sets blocklen to 512 bytes (default)
*			mmc_start_read_block(1000, 9);	//start reading sector 1000
*			mmc_read_buffer(&mmc_buf[0], 100);
*			mmc_read_buffer(&mmc_buf[100], 412);
*			mmc_stop_read_block();
*
*		Notes:
*			After calling mmc_start_read_block data is read with
*			mmc_read_buffer. Make sure to exactly read the amount of bytes
*			of the current blocklen (default 512 bytes).  This has not to be
*			done in a single mmc_read_buffer function.
*/

extern void mmc_read_buffer(unsigned char *buf, unsigned short len);
/*
*		mmc_read_buffer reads data from mmc after calling mmc_start_read_block
*
*		Parameters:
*			buf: pointer to a buffer where read-data is stored
*			len: amount of bytes to be read to buf
*
*		Notes:
*			See mmc_start_read_buffer
*/

extern void mmc_stop_read_block(void);
/*-----------------------------------------------------------------------------
*		mmc_stop_read_block must be called after the amount of bytes set by
*			mmc_setblocklen	has been read
-----------------------------------------------------------------------------*/


extern unsigned char mmc_start_write_block(unsigned long adr, unsigned char adrshift);
/*
*		mmc_start_write_sector initializes the writing of a sector
*
*		Parameters:
*			adr: specifies address to be read from
*			adrshift: the specified address is shifted left by <adrshift> bits
*								e.g. if you use a blocksize of 512 bytes you should set
*								adrshift to 9 in order to specify the blocknumber in adr.
*
*		Return values:
*			MMC_OK:	Command successful
*			MMC_CMDERROR:	Error while sending write command to mmc
*		
*		Example Code:
*			unsigned char mmc_buf[512];
*			mmc_init();	//Initializes MMC / SD Card
*			set_blocklen(512);	//Sets blocklen to 512 bytes (default)
*			mmc_start_write_block(1000, 9);	//start writing sector 1000
*			mmc_write_buffer(&mmc_buf[0], 200);
*			mmc_write_buffer(&mmc_buf[200], 200);
*			mmc_write_buffer(&mmc_buf[400], 112);
*			mmc_stop_write_block();
*
*		Notes:
*			After calling mmc_start_write_block data is written with
*			mmc_write_buffer. Make sure to exactly write the amount of bytes
*			of the current blocklen (default 512 bytes). This has not to be
*			done in a single mmc_write_buffer function.
*/

extern void mmc_write_buffer(unsigned char *buf, unsigned short len);

extern void mmc_stop_write_block(void);


extern void mmc_set_blocklen(unsigned short blocklen);
/*-----------------------------------------------------------------------------
*		mmc_set_blocklen	sets the amount of bytes to be read/written
*
*		Parameters:
*			blocklen: Blocksize in bytes
*
*		Notes:
*			Not all MMC support read buffers <> 512 bytes, and most MMC
*			do not support write buffers <> 512 bytes!
-----------------------------------------------------------------------------*/

extern unsigned char mmc_read_csd(char *buffer);
/*	
*		reads the CSD (Card Specific Data) register from MMC.
*		Make sure that buffer points to an allocated memory with at least 16 bytes!
*
*		Parameters:
*			buffer:	pointer to allocated memory where to store data
*
*		Example Code:
*			unsigned char mmc_buf[512];
*			unsigned char blklen;
*			mmc_init();	//Initializes MMC / SD Card
*			mmc_read_csd(mmc_buf);	//read csd register
*			blklen = MMC_CSD_READ_BLK_LEN(mmc_buf);
*			
*
*		Return values:
*			0: block read successfully
*			1: No response after read command from MMC
*			2: No startbyte received
*/

extern unsigned char mmc_read_cid(char *buffer);
/*	
*		reads the CID (Card Identification) register from MMC.
*		Make sure that buffer points to an allocated memory with at least 16 bytes!

*
*		Parameters:
*			buffer:	pointer to allocated memory where to store data (at least 16 bytes)
*
*		Return values:
*			0: block read successfully
*			2: No startbyte received
*/

//Macros for accesing the CSD register. buf must be a pointer to unsigned char,
//pointing to the first byte of the csd data read with mmc_read_csd
#define MMC_CSD_STRUCTURE(buf)					(buf[0]>>6)
#define MMC_CSD_SPEC_VERSION(buf)				((buf[0]>>2) & 0x0F)
#define MMC_CSD_TAAC(buf)								(buf[1])
#define MMC_CSD_NSAC(buf)								(buf[2])
#define MMC_CSD_TRAN_SPEED(buf)					(buf[3])
#define MMC_CSD_CCC(buf)								((buf[4]<<4) | ((buf[5]>>4) & 0x0F))
#define MMC_CSD_READ_BLK_LEN(buf)				(buf[5] & 0x0F)
#define MMC_CSD_READ_BLK_PARTIAL(buf)		((buf[6]>>7) & 0x01)
#define MMC_CSD_WRITE_BLK_MISALIGN(buf)	((buf[6]>>6) & 0x01)
#define MMC_CSD_READ_BLK_MISALIGN(buf)	((buf[6]>>5) & 0x01)
#define MMC_CSD_DSR_IMP(buf)						((buf[6]>>4) & 0x01)
#define MMC_CSD_C_SIZE(buf) 						(((buf[6] & 0x03) << 10) | (buf[7] << 2) | ((buf[8] >> 6) & 0x03))
#define MMC_CSD_VDD_R_CURR_MIN(buf)			((buf[8] >> 3) & 0x07)
#define MMC_CSD_VDD_R_CURR_MAX(buf)			(buf[8] & 0x07)
#define MMC_CSD_VDD_W_CURR_MIN(buf)			((buf[9] >> 5) & 0x07)
#define MMC_CSD_VDD_W_CURR_MAX(buf)			((buf[9] >> 2) & 0x07)
#define MMC_CSD_C_SIZE_MULT(buf)				(((buf[9] & 0x03) << 1) | ((buf[10] >> 7) & 0x01))
#define MMC_CSD_ERASE_GRP_SIZE(buf)			((buf[10] >> 2) & 0x1F)
#define MMC_CSD_ERASE_GRP_MULT(buf)			(((buf[10] & 0x03) << 3) | ((buf[11] >> 5) & 0x07))
#define MMC_CSD_WP_GRP_SIZE(buf)				(buf[11] & 0x1F)
#define MMC_CSD_WP_GRP_ENABLE(buf)			((buf[12] >> 7) & 0x01)
#define MMC_CSD_DEFAULT_ECC(buf)				((buf[12] >> 5) & 0x03)
#define MMC_CSD_R2W_FACTOR(buf)					((buf[12] >> 2) & 0x07)
#define MMC_CSD_WRITE_BLK_LEN(buf)			(((buf[12] & 0x03) << 2) | ((buf[13] >> 6) & 0x03))
#define MMC_CSD_WRITE_BLK_PARTIAL(buf)	((buf[13] >> 5) & 0x01)
#define MMC_CSD_FILE_FORMAT_GRP(buf)		((buf[14] >> 7) & 0x01)
#define MMC_CSD_COPY(buf)								((buf[14] >> 6) & (0x01)
#define MMC_CSD_PERM_WRITE_PROTECT(buf)	((buf[14] >> 5) & 0x01)
#define MMC_CSD_TMP_WRITE_PROTECT(buf)	((buf[14] >> 4) & 0x01)
#define MMC_CSD_FILE_FORMAT(buf)				((buf[14] >> 2] & 0x03)
#define MMC_CSD_ECC(buf)								(buf[14] & 0x03)
#define MMC_CSD_CRC7(buf)								((buf[15] >> 1) & 0x7F)

#ifdef __cplusplus
} 
#endif

#endif
