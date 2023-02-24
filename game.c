#include <stdint.h>  
#include <C:\msys64\opt\mcb32tools\include\pic32mx.h>  
#include "snakeheader.h"

#define SCREENWIDTH 128
#define SCREENPAGES 4
#define PIXELBITS 8
#define UNITSIZE 1  //pixel x pixel
#define MAXPIXEL ((SCREENWIDTH * SCREENPAGES )/ UNITSIZE) //128 * 32 / unitsize



// 2 arrays to keep track of all the snakes bodyparts
// for x coordinate just keep tack of where all the parts are between 0-127
// for y coordinates, snake y = page 0-3
// snakeBit = bit value in that page
int snakeX[MAXPIXEL]; 
int snakeY[MAXPIXEL];
int snakeBit[MAXPIXEL];

// global variable for the snakes bodyparts
int bodyParts = 4;

//apple coordinates 
int appleX;
int appleY;
int appleB;

// general gameattriubutes
int score = 0;
char direction = 'R'; // R for RIGHT osv
char gameON = 'F'; //F for false, T for True 
int timer2counter = 0; //global counter for timer2





// initalize the snakes's head aka startpos
void snakeInit()
{
    


    //resetting old array
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
    appleX = 50;
    appleY = 1;
    appleB = 1;

}






/*
    function to move the coordinates of the snake by the 
*/
void moveSnake()
{   

    //updating the coordinates for the tail. The tail follows the head
    int i;
    for(i = bodyParts ; i > 0; i--)
    {
        snakeX[i] = snakeX[i-1];        //bodypart = ett index bakom nästa body
        snakeY[i] = snakeY[i-1];        
        snakeBit[i] = snakeBit[i-1];    
    }
    
    //this doesnt work ?? body doesnt increment 
    // for(i = bodyParts; i <= bodyparts; i++)
    // {
    //     snakeX[i] = snakeX[i-1];
    //     snakeY[i] = snakeY[i-1];
    //     snakeBit[i] = snakeBit[i-1];
    // }



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







void checkCollision()
{
    //checking collision with apple, if true make new apple and increment bodyParts
    if ( (snakeX[0] == appleX) && (snakeY[0] == appleY) && (snakeBit[0] == appleB))
    {
        
        score++;
        bodyParts +=20;
        newApple();
        
        // int i;
        // for (i=0; i < 10 ;i++)
        // {
        //     bodyParts++;
        //     moveSnake();
        //     
        // }
        

    }




    //checking collision with the tail, if true gameON = False.
    int i;
    for (i = 1; i < bodyParts ; i++ )
    {
        if ( (snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i] && (snakeBit[0] == snakeBit[i])) )
            gameON = 'F';
    }
        




    //checking collsion with outer borders, if true gameON = False
    if ( snakeX[0] == 127)
        gameON = 'F';
    if ( snakeX[0] == 0)
        gameON = 'F';
    if ( snakeY[0] == 0 && snakeBit[0]== 1)
        gameON = 'F';
    if ( snakeY[0] == 3 && snakeBit[0]== 7)
        gameON = 'F';
}


// changes the direction with 4 BTNS
void oldchangeDirection()
{

    if (PORTF & (0x2) && direction != 'L')
		direction = 'R';
			
	if (PORTD & (1<<5) && direction != 'U')
		direction = 'D';
    //INPUT BTN 3
    if (PORTD & (1<<6) && direction != 'D')
		direction = 'U';
	//INPUT BTN 4
	if (PORTD & (1<<7) && direction != 'R')
		direction = 'L';
}






void resetGame()
{   
    
    direction = 'R';
    bodyParts = 4;
    appleB = 0;
    score = 0;

    return;

}




void gameOver()
{   
    whiteDisplay();
    wait10ms(300);
    
    //show score press button to start again


    //check for highscore and store (IF TIME ALLOWS)


    //quick reset of variables
    resetGame();

    return;
}




void startGame()
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


