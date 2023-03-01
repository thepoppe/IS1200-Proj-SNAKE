/* uc32config.c
   This file written 2023 by P Morin
   Some parts copied from https://stackoverflow.com/questions/26237419/faster-than-rand
   and functions user_isr() is taken from lab3 time4io.

   For copyright and licensing, see file COPYING */
//#include <stdlib.h>
#include <stdint.h>  
#include <C:\msys64\opt\mcb32tools\include\pic32mx.h>  
#include "snakeheader.h"




// INITIALIZATION for btns and tmrs
void IO_init(void)
{
    TRISDSET = (0b111 << 5); //BTN4,3,2
    TRISFSET = 0x2; //BTN1

    //timer 2 used for menues etc flag on pin 8
    T2CON = 0; //clear timer
    TMR2 = 0x0; // initial timer value =0
    PR2 = ((80000000/256) / 100); // period value = with 80 Mhz/256 prescale/ 100hz = 3125. DETTA ger 10 ms 
    T2CONSET = (7 << 4); //prescale = 256  (111 << 4 = 111 0000 = bit 4-6) 
    T2CONSET = 0x8000; // Start the timer  

  

    //timer 4 used for gamespeed. flag on pin 16
    T4CON = 0; //clear timer
    TMR4 = 0x0; // initial timer value =0
    PR4 = ((80000000/256) / 1000) ;// 1 ms = 0,001s => f=1000
    T4CONSET = (7 << 4); //prescale = 256
    T4CONSET = 0x8000; // Start the timer 

    return;
}


//copied from lab3
void user_isr(){
  
}






/*
* OBS copied from https://stackoverflow.com/questions/26237419/faster-than-rand
* This is because importing srand and rand didnt work. 
*/

/*************************************
    copied part starts here
*************************************/
static unsigned int g_seed;
// Used to seed the generator.           
inline void fast_srand(int seed) {
    g_seed = seed;
}
// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}
/*************************************
	copied part ends here
*************************************/



//randomnumber returns a number between minvalue and maxvalue;
int randomnumber(int minvalue, int maxvalue)
{   
    fast_srand(TMR4);
    int random;
    while ( 1 )
    {
        random = fast_rand()% maxvalue;
        if ( random >= minvalue && random <= maxvalue )
            break;

    }
    
    return random;
}




// get button values from  BTN 4,3,2
int btnvalues()
{
    return ((PORTD >> 5) & 0x7);
}




// wait10ms waits for 10 ms  n times
void wait10ms(int n)
{
    int wait10ms = 0;

    while (wait10ms < (n) )
    {
        if (IFS(0) & (1<<8))
        {
            IFSCLR(0) = (1<<8);
            wait10ms++;
        }

    }

    return;
    
}
