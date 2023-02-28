/* game.c
   This file written 2023 by P Morin
   

   For copyright and licensing, see file COPYING */



//game.c
extern char direction;
extern int bodyParts;
extern int snakeX[];
extern int snakeY[];
extern int snakeBit[];
extern int appleX;
extern int appleY;
extern int appleB;
void gameMeny();
void oldchangeDirection();


// oleddisplay.c
void displayInit(void);
void display_string(int, char *);
void display_update(void);
void printStrings(char* , char* , char* , char*  );
void GameBufferUpdate();
void whiteDisplay();
void blackDisplay();



//mipslabfunc.c
extern char textbuffer[4][16];
extern const uint8_t const font[];





//uc32Config.c
void IO_init(void);
void user_isr();
void wait10ms(int);
int btnvalues();
int randomnumber(int, int);


//labwork.S
void enableEI(void);
void delay(int);


