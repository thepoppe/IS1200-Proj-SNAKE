
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
void waitAWhile(int s);


//labwork.S
void enableEI();
void delay(int);


