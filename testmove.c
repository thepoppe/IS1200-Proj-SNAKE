
#include <stdint.h>  
#include <C:\msys64\opt\mcb32tools\include\pic32mx.h>  
#include "snakeheader.h"


int pixelPosX;  // 0 - 127
int pixelPosY;  // 0 - 3 dvs pages
int pixelBIT;   // 0-7 bitposition.


// pow2 raises 2 to the power of n and returns it. NOT USED
int pow2(int n)
{
    int i;
    char sum = 1;
    for (i = 0; i < n; i++)
    {
        sum *= 2;
    }

    return sum;
}

//creates a pixel
void createPixel()
{
    
    //displayBuffer[0][0] = 1;
    pixelPosX = 0;
    pixelPosY = 0;
    pixelBIT = 0;
    
    return;
}




// changes the direction with 4 BTNS
void oldchangeDirection()
{

    if (PORTF & 0x2)// && direction != 'L')
		direction = 'R';
			
	if (PORTD & (1<<5) && direction != 'U')
		direction = 'D';
    //INPUT BTN 3
    if (PORTD & (1<<6) && direction != 'D')
		direction = 'U';
	//INPUT BTN 4
	if (PORTD & (1<<7))// && direction != 'R')
		direction = 'L';
}


// function to move the pixel by incrementing the index
void movePixel()
{ 
    
    
    if (direction == 'R' && pixelPosX < 127)
        pixelPosX ++;


    if(direction == 'L' && pixelPosX > 0)
        pixelPosX --;



    if (direction == 'U')
    {
        if (pixelBIT == 0 && pixelPosY > 0)
        {
            pixelPosY --;
            pixelBIT = 7;
        }
        else if (pixelBIT > 0) 
            pixelBIT--;
        
        
    }

    if (direction == 'D')
    {

        if (pixelBIT == 7 && pixelPosY < 3)
        {
            pixelPosY ++;
            pixelBIT = 0;
        }
        else if (pixelBIT < 7)
            pixelBIT++;
    }
    

}




// moveAPixel adds the functions together to be called from main.
void moveAPixel()
{
    while (1)
    {
        createPixel();
        movePixel();
        oldchangeDirection();
        delay(75);
    }
}



