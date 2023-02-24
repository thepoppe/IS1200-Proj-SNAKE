
//testmove
void moveAPixel();
void oldchangeDirection();



//game.c
extern char direction;
extern int bodyParts;
extern int snakeX[];
extern int snakeY[];
extern int snakeBit[];
extern int appleX;
extern int appleY;
extern int appleB;
void startGame();



// oleddisplay
void displayInit(void);
void GameBufferUpdate();
void whiteDisplay();





//uc32Config
void IO_init(void);
void user_isr();
void wait10ms(int);

//labwork.S
void enableEI(void);
void delay(int);




//advancegame
void GameBufferUpdate2D();