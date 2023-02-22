#include <stdint.h>  
#include <C:\msys64\opt\mcb32tools\include\pic32mx.h>  
#include "snakeheader.h"





// ***OBS Copied from lab 3, time4io ***
#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)
#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)
#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)
#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)


// ***OBS Copied from lab 3, time4io ***
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

// ***OBS Copied from lab 3, time4io ***
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08));
	SPI2BUF = data;
	while(!(SPI2STAT & 1));
	return SPI2BUF;
}


// ***OBS Copied from lab 3, time4io ***
void displayInit(void) {

    DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}






//buffer array with all pixels. NOT USED
uint8_t display[512];

//buffer array for the oled that is 128 pixel times 4 pages of 8 pixels
uint8_t displayBuffer[4][128];





// old buffer to update one single pixel
void pixelBufferUpdate()
{
	bufferReset();
	//displayBuffer[pixelPosY][pixelPosX] = (1 << pixelBIT);  
}





// turns all pixels on, created for testing
void whiteDisplay()
{
  int i,j;
  for (i=0; i < 4; i++)
	for(j=0; j<128; j++)
      	displayBuffer[i][j]= 255;
  return;
}


// bufferReset clears the array of numbers so the screen turns black
void bufferReset()
{
    int i,j;
    for (i=0; i < 4; i++)
		for(j=0; j<128; j++)
    	  	displayBuffer[i][j]= 0;

	
}











// bufferUpdate updates the array displayBuffer with the pixels of the snakes's and apple's location
void bufferUpdate()
{	
	 // resets the buffer
	bufferReset();

	// adds the apple to the array
	displayBuffer[appleY][appleX] |= (1<<appleB);


	// adds the snake to the array
	int i = bodyParts;
	while(i >= 0)
	{	
		//if (snakeBit[i] == 7)
		//	page--;
		
		displayBuffer[ snakeY[i] ][ snakeX[i] ] |= (1 << snakeBit[i]);
		i--;

/*		
		//ex 3 bodyparts,  moving down		
		head x[0]=10
		head y[0]= 1
		head b[0]= 5
		page = 1
		displatbuffer[ 1 ][ 10 ] |= 0b00100000 ( 1<< 5)

		tail1 x[1]=[x0-1]= "moving down" = x[0] = 10
		tail1 y[1]=[y0-1]= "moving down" = y[0] = 1
		tail1 b[1]=[b0-1]= "moving down = -1" = 4
		page = 1
		displatbuffer[ 1 ][ 10 ] |= 0b00110000 ( 1<<4 )

		tail2 x[2]=[x1-1]= "moving down" = x[1] = 10
		tail2 y[2]=[y1-1]= "moving down" = y[1] = 1
		tail2 b[2]=[b1-1]= "moving down = - 1 "  = 4
		page = 1
		displatbuffer[ 1 ][ 10 ] |= 0b00111000 ( 1<< 3)
		  
*/
	}
	
}







// ***OBS Copied from lab 3, time4io. Some changes made to the code ***
void oledUpdate() 
{	
	
	bufferUpdate();
	int i, j;
	for(i = 0; i < 4; i++) 
	{
		DISPLAY_CHANGE_TO_COMMAND_MODE;
		spi_send_recv(0x22);
		spi_send_recv(i);

		spi_send_recv(0x0);
		spi_send_recv(0x10);

		DISPLAY_CHANGE_TO_DATA_MODE;
		for (j = 0; j < 128; j++)
			spi_send_recv(displayBuffer[i][j]);
	
	}
}







