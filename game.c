/* game.c
   This file written 2023 by P Morin
   

   For copyright and licensing, see file COPYING */

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

// general gameattributes
int score = 0;
char direction = 'R'; // R for RIGHT osv
char gameON = 'F'; //F for false, T for True 
int timer2counter = 0; //global counter for timer2
int gamespeed = 6; // less should give faster. //dificulty = medium on default

int highscore[3*sizeof(int)];
char highscoreSTRINGS[3][16];





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





// creates a new eatable apple at a random pixel
void newApple()
{
    appleX = randomnumber( 5, 125 );
    appleY = randomnumber( 0, 3 );
    appleB = randomnumber( 1, 7 );

}




//function to move the coordinates of the snake by the 
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
    // for(i = 0; i < bodyParts; i++)
    // {
    //     snakeX[i+1] = snakeX[i];
    //     snakeY[i+1] = snakeY[i];
    //     snakeBit[i+1] = snakeBit[i];
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



//chechcollision cjecks if the coordinates of the head crosses eatables, another bodypart or the borders
void checkCollision()
{
    //checking collision with apple, if true make new apple and increment bodyParts
    if ( (snakeX[0] == appleX) && (snakeY[0] == appleY) && (snakeBit[0] == appleB))
    {
        score++;
        bodyParts += 4;
        newApple();

    }


    //checking collision with the tail, if true gameON = False.
    int i;
    for (i = 1; i < bodyParts ; i++ )
    {
        if ( (snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i] && (snakeBit[0] == snakeBit[i])) )
            gameON = 'F';
    }
        

    //checking collsion with outer borders, if true gameON = False
    if ( snakeX[0] > 127)
        gameON = 'F';
    if ( snakeX[0] < 0)
        gameON = 'F';
    if ( snakeY[0] < 0)
        gameON = 'F';
    if ( snakeY[0] > 3 )
        gameON = 'F';

}



// changes the variable direction with 4 BTNS
void oldchangeDirection()
{
    //INPUT BTN 1
    if (PORTF & (0x2) && direction != 'L')
		direction = 'R';
	//INPUT BTN 2		
	if (PORTD & (1<<5) && direction != 'U')
		direction = 'D';
    //INPUT BTN 3
    if (PORTD & (1<<6) && direction != 'D')
		direction = 'U';
	//INPUT BTN 4
	if (PORTD & (1<<7) && direction != 'R')
		direction = 'L';
}



// helperfunction to convert the score to a string 
void integerToString(char * list, int listSize, int number)
{
    int i;
    int temp =0x0;
    int size = sizeof(number);
    

    for ( i= 0; i < size; i++)
    {   
        temp = number % 10; //to split the number into digits. 
        number = number /10;
        temp+= 0x30;
        if( i < listSize) // just to be safe
            list[size - i -1] = temp;

    }
    list[size] = 0x00;
}
   


//helperfunction to add 2 strings together
void concatenateString(char* s1, char* s2)
{
    int i, j = 0;
    
    int size1 = 16;
    
    for ( i = 0; i < size1; i++ )
    {  
        if ( s1[i] == 0x0 && s2[j] != 0x0)
           s1[i] = s2[j++];    
      
    }
   
}



//checks if score is a a top 3 highscore, if yes replaces it
void checkHighscore()
{
    int i;
    if (score > highscore[0])
    {   
        //moving the scores one spot
        for (i = 2; i > 0; i--)
        {   
            if (highscore[i-1] != 0) // to avoid third being filled with false score
                highscore[i] = highscore[i-1];
        }
            
        highscore[0] = score;
        printStrings( "HIGHSCORE!", "Your score is", "number 1", "1 to continue"  );

    }
    else if (score > highscore[1])
    {
        highscore[2] = highscore[1];
        highscore[1] = score;
        printStrings( "HIGHSCORE!", "Your score is", "number 2", "1 to continue"  );

    }
    else if ( score > highscore[2])
    {
        highscore[2] = score;
        printStrings( "HIGHSCORE!", "Your score is", "number 3", "1 to continue"  );
    }    
    else {
        printStrings( "* * * * * * * *", "Your score was", "too low...", "1 to continue"  );
        return;
    }


    //updating the chararray
    for (i = 0; i < 3; i++)
        integerToString(highscoreSTRINGS[i], sizeof(highscoreSTRINGS[i]),highscore[i]);

}




// gameover prints game over, shows the score and checks for highscore
void gameOver()
{   
    blackDisplay();
    wait10ms(100);
    
 //show score press a button to start again
    char yourscore[16], finaltext[16] = "Score:   ";
    integerToString(yourscore, 16,  score);
    concatenateString(finaltext, yourscore);
    //strcat(text, yourscore);
    printStrings( "GAME OVER!", finaltext, "" , "1 to continue"  );


    while(btnvalues() == 0)
    {
        //wait
    }

    blackDisplay();
    wait10ms(40);

    
    //check for highscore and store (IF TIME ALLOWS)
    checkHighscore();
    while(btnvalues() == 0)
    {
        //wait
    }

    blackDisplay();
    wait10ms(20);


    //quick reset of variables
    direction = 'R';
    bodyParts = 4;
    appleB = 0;
    score = 0;

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
        if (timer2counter ==  gamespeed) // 10 on default 100ms
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

}



// game meny uses buttons to toggle to the game,level and highscore
void gameMeny()
{   
    //A LOGO WOULD BE COOL


    //wait 2 seconds
    wait10ms(200);
    

    while(1)
    {   
        printStrings("  *  SNAKE  *  ", "1 - Start game!", "2 - Difficulty", "3 - Highscores");

        while(1)
        {
            //start game
            if (btnvalues() & (1<<2))
            {
                blackDisplay();
                wait10ms(30);
                startGame();
                break;
            }


            //Highscore menu
            if (btnvalues() & ( 1 << 1))
            {
                blackDisplay();
                wait10ms(30);
                printStrings("  *  LEVEL  *  ","1 - Easy","2 - Medium","3 - Hard");
                int choice = 0;
                while(choice == 0)
                {
                    choice = btnvalues();
                    wait10ms(10);
                }
                if( choice & (1<<2))
                    gamespeed = 9;
                        
                if( choice & (1<<1))
                    gamespeed = 6;

                if( choice & ( 1 ))
                    gamespeed = 3;

            
                break;
            }


            //Highscore menu
            if (btnvalues() & 1 )
            {
                blackDisplay();
                wait10ms(30);
                printStrings("* Highscores *", highscoreSTRINGS[0], highscoreSTRINGS[1], highscoreSTRINGS[2]);

                while (btnvalues() == 0)
                {
                    //wait
                }
                
            break;
            }
            
        }

        //turn the screen black
        blackDisplay();
        //wait 0.4 seconds
        wait10ms(40);
    }

}

