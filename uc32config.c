
#include <stdint.h>  
#include <C:\msys64\opt\mcb32tools\include\pic32mx.h>  
#include "snakeheader.h"




// INITIALIZATION
void IO_init(void)
{
    TRISDSET = (0b111 << 5); //BTN4,3,2
    TRISFSET = 0x2; //BTN1


    T2CON = 0; //clear timer
    TMR2 = 0x0; // initial timer value =0
    PR2 = ((80000000/256) / 100); // period value = with 80 Mhz/256 prescale/ 100hz = 3125. DETTA ger 10 ms 
    T2CONSET = (7 << 4); //prescale = 256  (111 << 4 = 111 0000 = bit 4-6)  CONSET?
    T2CONSET = 0x8000; // Start the timer  

  

    //timer 4 used for random number.
    //    T4CON = 0; //clear timer
    //    TMR4 = 0x0; // initial timer value =0
    //  PR4 = 1337; 
    //  T4CONSET = 0x8000; // Start the timer 




    //enable interupt
    //enableEI();

    //enable interrupt on Sw1, prio 2
    //IECSET(0) = (1<<7);
    //IPCSET(1) = (2 << 26);

    //enable interupt on sw4
    //IECSET(0) = (1<<19);
    //IPCSET(4) = (2 << 26);

    //enable timer2 interrupts for changing direction, prio 1
    //IECSET(0) = (1<<4);
    //IPCSET(2) = (3<<2);

    

    return;
}



void user_isr(){
    //SW4
    if (IFS(0) & (1<<26))
    {
        IFSCLR(0) = (1<<26);
        if(direction == 'U')
            direction = 'L';
        if(direction == 'L')
            direction = 'D';
        if(direction == 'D')
            direction = 'R';
        if(direction == 'R')
            direction = 'U';
        
        

    }

    //SW1
    if (IFS(0) & (1<<7))
    {
        IFSCLR(0) = (1<<7);
        if(direction == 'U')
            direction = 'R';
        if(direction == 'R')
            direction = 'D';
        if(direction == 'D')
            direction = 'L';
        if(direction == 'L')
            direction = 'U';
        
        

    }



    // if timeout (100ms) check doesnt work
    // if (IFS(0) & (1<<8))
    // {
    //     //clearflag
    //     (IFSCLR(0) & (1<<8));
    //     oldchangeDirection();
    
    // }
    // return;


}





//returns a number between 1-100;
int randomnumber(int minvalue, int maxvalue)
{   
    //srand((unsigned int) TMR4);
    //return (rand()% maxvalue);
    int random = 2;
    
    
    return random;
}



// get button values
int btnvalues()
{
    return ((PORTD >> 5) & 0x7);
}






// wait10ms waits for 10 ms n times
void wait10ms(int n)
{
    
    int wait10ms =0;

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
