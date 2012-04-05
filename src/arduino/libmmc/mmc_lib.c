#include <avr/io.h>
#include "mmc_lib.h"

static unsigned char cmd[6];

inline static void spi_send_byte(unsigned char data)
{
	SPDR=data;
	loop_until_bit_is_set(SPSR, SPIF); // wait for byte transmitted...
}

static unsigned char send_cmd(void)
{
	unsigned short i;
	
	spi_send_byte(0xFF); //Dummy delay 8 clocks
	MMC_PORT &= ~(1<<MMC_CS);	//MMC Chip Select -> Low (activate)

	for (i=0; i<6; i++)
	{//Send 6 Bytes
		spi_send_byte(cmd[i]);
	}

	unsigned char result;
	
	for(i=0; i<128; i++)
	{//waiting for response (!0xff)
		spi_send_byte(0xFF);
		
		result = SPDR;
		
		if ((result & 0x80) == 0)
			break;
	}
	
	return(result); // TimeOut !
}

unsigned char mmc_init(void)
{
	MMC_DDR &= ~(1<<SPI_MISO);	//MMC Data Out -> Input
	MMC_DDR |= 1<<SPI_CLK | 1<<SPI_MOSI | 1<<MMC_CS;	//MMC Chip Select -> Output
		
	SPCR = 1<<SPE | 1<<MSTR | SPI_INIT_CLOCK; //SPI Enable, SPI Master Mode
	SPSR = SPI_DOUBLE_SPEED << SPI2X;
	
	unsigned char i;
	
	for (i=0; i<10; i++) { //Pulse 80+ clocks to reset MMC
		spi_send_byte(0xFF);
	}
	
	unsigned char res;

	cmd[0] = 0x40 + MMC_GO_IDLE_STATE;
	cmd[1] = 0x00; cmd[2] = 0x00; cmd[3] = 0x00; cmd[4] = 0x00; cmd[5] = 0x95;
	
	for (i=0; i<10; i++)
	{
		res=send_cmd(); //store result of reset command, should be 0x01
		MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
		spi_send_byte(0xFF);
		if (res == 0x01)
			break;
	}
	
	if (res != 0x01) //Response R1 from MMC (0x01: IDLE, The card is in idle state and running the initializing process.)
		return(MMC_INIT);
	
	cmd[0]=0x40 + MMC_SEND_OP_COND;
		
	i=0;
	
	while((send_cmd() != 0) && (i < 0xFF)) {
		MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
		spi_send_byte(0xFF);
		i++;
	}
	MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
	
	if (i < 0xFF)
		return(MMC_OK);

	return(MMC_TIMEOUT);
}

static unsigned char wait_start_byte(void)
{
	unsigned char i=0;
	
	do
	{
		spi_send_byte(0xFF);
		i++;
	} while ((SPDR != 0xFE) && (i<255));
	
	if (i<255)
		return MMC_OK;
	else
		return MMC_NOSTARTBYTE;
}

unsigned char mmc_start_read_block(unsigned long adr, unsigned char adrshift)
{
	adr = adr << adrshift;
	
	cmd[0] = 0x40 + MMC_READ_SINGLE_BLOCK;
	cmd[1] = (adr & 0xFF000000) >> 0x18;
	cmd[2] = (adr & 0x00FF0000) >> 0x10;
	cmd[3] = (adr & 0x0000FF00) >> 0x08;
	cmd[4] = (adr & 0x000000FF);
	cmd[5] = 0x95;
	
	SPCR = 1<<SPE | 1<<MSTR | SPI_READ_CLOCK; //SPI Enable, SPI Master Mode
	
	if (send_cmd() != 0x00) {
		MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
		return(MMC_CMDERROR); //wrong response!
	}
	
	if (wait_start_byte())
	{
		MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
		return MMC_NOSTARTBYTE;
	}
	
	return(MMC_OK);
}

void mmc_read_buffer(unsigned char *buf, unsigned short len)
{
	while (len--)
	{
		spi_send_byte(0xFF);
		*(buf++) = SPDR;
	}
}

void mmc_stop_read_block(void)
{
	//read 2 bytes CRC (not used);
	spi_send_byte(0xFF);
	spi_send_byte(0xFF);
	MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
}

void mmc_set_blocklen(unsigned short blocklen)
{
	cmd[0] = 0x40 + MMC_SET_BLOCKLEN;
	cmd[1] = 0x00;
	cmd[2] = 0x00;
	cmd[3] = blocklen >> 8;
	cmd[4] = blocklen;
	cmd[5] = 0x95;
	send_cmd();
}
	
unsigned char mmc_start_write_block(unsigned long adr, unsigned char adrshift)
{
	adr = adr << adrshift;
	
	cmd[0] = 0x40 + MMC_WRITE_BLOCK;
	cmd[1] = (adr & 0xFF000000) >> 0x18;
	cmd[2] = (adr & 0x00FF0000) >> 0x10;
	cmd[3] = (adr & 0x0000FF00) >> 0x08;
	cmd[4] = (adr & 0x000000FF);
	cmd[5] = 0x95;
	
	SPCR = 1<<SPE | 1<<MSTR | SPI_WRITE_CLOCK; //SPI Enable, SPI Master Mode
	
	if (send_cmd() != 0x00) {
		MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
		return(MMC_CMDERROR); //cmd24 wrong response!
	}

	unsigned char i=0;
	
	for (i=0;i<255;i++) { //delay before sending datablock to MMC
		spi_send_byte(0xFF);
	}
	spi_send_byte(0xFE); //send startbyte
	return (MMC_OK);
}

void mmc_write_buffer(unsigned char *buf, unsigned short len)
{
	while (len--)
	{//send datablock to MMC
		spi_send_byte(*buf++);
	}
}

void mmc_stop_write_block(void)
{
	//write 2 dummy bytes to MMC (CRC);
	spi_send_byte(0xFF);
	spi_send_byte(0xFF);
	
	do
	{
		spi_send_byte(0xFF);
	} while (SPDR != 0xFF);
	
	MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
}	

unsigned char mmc_read_csd(char *buffer)
{
	cmd[0]=0x40 + MMC_SEND_CSD;
	send_cmd();
	
	if (wait_start_byte())
	{
		MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
		return MMC_NOSTARTBYTE;
	}
	
	mmc_read_buffer(buffer, 16);
	mmc_stop_read_block();
	return(MMC_OK);
}

unsigned char mmc_read_cid(char *buffer)
{
	cmd[0]=0x40 + MMC_SEND_CID;
	send_cmd();
	
	if (wait_start_byte())
	{
		MMC_PORT |= 1<<MMC_CS; //MMC Chip Select -> High (deactivate);
		return MMC_NOSTARTBYTE;
	}
	
	mmc_read_buffer(buffer, 16);
	mmc_stop_read_block();
	return(MMC_OK);
}

