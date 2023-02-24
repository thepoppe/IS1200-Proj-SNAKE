#include <stdint.h>  
#include <C:\msys64\opt\mcb32tools\include\pic32mx.h>  
#include "snakeheader.h"

#define SCREENWIDTH 128
#define SCREENPAGES 4
#define PIXELBITS 8
#define UNITSIZE 2  //pixel x pixel
#define MAXPIXEL ((SCREENWIDTH * SCREENPAGES )/ UNITSIZE) //128 * 4 / unitsize
#define MAXBIT (PIXELBITS/UNITSIZE)



// 2 arrays to keep track of all the snakes bodyparts
// for x coordinate just keep tack of where all the parts are between 0-127
// for y coordinates, snake y = page 0-3
// snakeBit = bit value in that page
int snakeX[MAXPIXEL]; //2d => 512/2= 256
int snakeY[MAXPIXEL];
int snakeBit[MAXPIXEL];

// global variable for the snakes bodyparts
int bodyParts = 4;

//apple coordinates 
int appleX[UNITSIZE];
int appleY[UNITSIZE];
int appleB[UNITSIZE];

// general gameattriubutes
int score = 0;
char direction = 'R'; // R for RIGHT osv
char gameON = 'F'; //F for false, T for True 
int timer2counter = 0; //global counter for timer2





// initalize the snakes's head aka startpos
void snakeInit()
{
    


    //resetting old snakearray
    int i;
    for(i =0; i < 512 ; i++)
    {

        snakeBit[i] = 0;
        snakeX[i] = 0;
        snakeY[i] =5;

    }
        snakeX[0] = 1;
        snakeY[0] = 1;
        snakeBit[0] = 1;
    
    

}




//needs random**************************************************************
// creates a new eatable apple at a random pixel
void newApple()
{
    appleX[0] = 50;
    appleY[0] = 1;
    appleB[0] = 1;

    appleX[1] = appleX[0] -1;
    appleY[1] = appleY[0];
    appleB[1] = appleB[0] -1;


}






/*
    function to move the coordinates of the snake by the 
*/
void moveSnake()
{   

    //updating the coordinates for the tail. The tail follows the head
    int i;
    for(i = bodyParts * UNITSIZE ; i > 0; i--)
    {
        snakeX[i] = snakeX[i-1];        //bodypart = ett index bakom nästa body
        snakeY[i] = snakeY[i-1];        
        snakeBit[i] = snakeBit[i-1];    
    }
    



    //updating the heads (index 0) position depending on the current direction 
    if (direction == 'R')   //moving the head one index to the right
        snakeX[0] += 1;
        


    if(direction == 'L')
        snakeX[0] -= 1;


    // moving downwards and upwars need an if statement to cross between pages
    if (direction == 'D')
    {   
        if (snakeBit[0] < 7)
            snakeBit[0]++;

        else if ((snakeBit[0] == 7))
        {
            snakeY[0]++;
            snakeBit[0] = 0;
        }
       
    }

    if (direction == 'U')
    {
        if (snakeBit[0] > 0) 
            snakeBit[0]--;
        else if (snakeBit[0] == 0)
        {
            snakeY[0]--;
            snakeBit[0] = 7; 
        }  

    }

}

uint8_t displayBuffer2D[4][128];
void GameBufferUpdate2D()
{	
	 // resets the buffer
	bufferReset();


	// adds the snake to the array
	int i = bodyParts * UNITSIZE;
	while(i >= 0)
	{	
		
		displayBuffer2D[ snakeY[i] ][ snakeX[i] ] |= (1 << snakeBit[i]);
		i--;
    }
}











void startGame2d()
{   
    
    
    snakeInit();
    newApple();
    
    
    gameON = 'T';
    moveSnake();
    GameBufferUpdate();
    wait10ms(10); //10 ms * 100  = 100ms 
    

    while (gameON == 'T')
    {  

        
        if (IFS(0) & (1<<8))
        {
            IFSCLR(0) = (1<<8);
            timer2counter++;
        }
        if (timer2counter ==    10) //borde ge 100 ms
        {   
            timer2counter = 0;
            
            moveSnake();
            GameBufferUpdate();
            checkCollision();
            oldchangeDirection();
            
        }

        

    

        

    }
    
    //wait 1 seconds
    wait10ms(100);

    //Call gameover
    gameOver();

    return;

}