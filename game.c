#include <stdint.h>  
#include <C:\msys64\opt\mcb32tools\include\pic32mx.h>  
#include "snakeheader.h"

#define MAXP (128)
#define MAXPAGES 4


const int SCREENWIDTH = 128;
const int SCREENPAGES = 4;
const int MAXPIXELS = 512;
const int MAXBIT = 0xFF;


// 2 arrays to keep track of all the snakes bodyparts
// for x coordinate just keep tack of where all the parts are between 0-127
// for y coordinates, snake y = page 0-3
// snakeBit = bit value in that page
int snakeX[MAXP *4]; 
int snakeY[512];
int snakeBit[512];



int bodyParts = 5;
int appleX;
int appleY;
int appleB;
int score = 0;
char direction = 'R'; // R for RIGHT osv
char gameON = 'F'; //F for false, T for True 
int timer2counter =0; //global counter for timer2








// initalize the snakes's head
void snakeInit()
{
    snakeX[0] = 1;
    snakeY[0] = 1;
    snakeBit[0] = 1;


    //resetting old array
    int i;
    for(i =1; i < 512 ; i++)
        snakeBit[i] = 0;
    



}




//needs random
void newApple()
{
    appleX = 50;
    appleY = 1;
    appleB = 1;
    bufferUpdate();

}











void moveSnake()
{   
    //hur får jag denna att kallas på av interrupt med timer??
    oldchangeDirection();

    
    //updating the coordinates
    int i;
    for(i = bodyParts ; i > 0; i--)
    {
        snakeX[i] = snakeX[i-1];        //bodypart = ett index bakom nästa body
        snakeY[i] = snakeY[i-1];        //body follows head
        snakeBit[i] = snakeBit[i-1];    
    }
    
    //this doesnt work ?? body doesnt increment 
    // for(i = bodyParts; i <= bodyparts; i++)
    // {
    //     snakeX[i] = snakeX[i-1];
    //     snakeY[i] = snakeY[i-1];
    //     snakeBit[i] = snakeBit[i-1];
    // }



    if (direction == 'R' && snakeX[0] < 127)   //moving the head one index
        snakeX[0] += 1;


    if(direction == 'L' && snakeX[0] > 0)
        snakeX[0] -= 1;


    // if dir == down and the position is less than (1111 1111 1111 = page 3 lowest pixel)
    if (direction == 'D')
    {   
        if (snakeBit[0] < 7)
            snakeBit[0]++;

        else if ((snakeBit[0] == 7) && snakeY[0] < 3)
        {
            snakeY[0]++;
            snakeBit[0] = 0;
        }
       
    }

    if (direction == 'U')
    {
        if (snakeBit[0] > 0) 
            snakeBit[0]--;
        else if (snakeBit[0] == 0 && snakeY[0] > 0)
        {
            snakeY[0]--;
            snakeBit[0] = 7; 
        }  
    }






    //uppdating the buffer for oled
    bufferUpdate();

}







void checkCollision()
{
    //checking collision with apple, if true make new apple and increment length
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
        //     oledUpdate();
        // }
        

    }




    //checking collision with the tail.
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







void resetGame()
{   
    bufferReset();
    direction = 'R';
    bodyParts = 4;
    appleB = 0;
    score = 0;

    return;

}




void gameOver()
{   
    bufferReset();
    resetGame();
    //show score press button to start again
    delay(2000);
    return;
}




void startGame()
{   
    
    newApple();
    snakeInit();
    oledUpdate();
    gameON = 'T';
    

    while (gameON == 'T')
    {  

        if (IFS(0) & (1<<8))
        {
            IFSCLR(0) = (1<<8);
            timer2counter++;
        }
        if (timer2counter == 10) //borde ge 100 ms
        {   
            timer2counter = 0;
            moveSnake();
            checkCollision();
            oledUpdate();
        }
    

        

    }
    timer2counter = 0;
    while (1)
    {
        if (IFS(0) & (1<<8))
        {
            IFSCLR(0) = (1<<8);
            timer2counter++;
        }
        if (timer2counter == 500) //10 ms*1000 = 10s
        {
            timer2counter=0;
            break;

        }
            
    }
    

    gameOver();

    return;

}


