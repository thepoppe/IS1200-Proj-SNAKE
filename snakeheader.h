



//testmove
void moveAPixel();
void oldchangeDirection();



//game.c
extern char direction;
extern int bodyParts;
extern int snakeX[512];
extern int snakeY[512];
extern int snakeBit[512];
extern int appleX;
extern int appleY;
extern int appleB;

void startGame();


//oleddisplay
extern uint8_t displayBuffer[4][128];
extern uint8_t display[];
void oledUpdate(void);
void displayInit(void);
void bufferReset();
void bufferUpdate();
void whiteDisplay();
void showBorder();



//uc32Config
void IO_init(void);
void user_isr();
int randomnumber(int, int);
int btnvalues();

//labwork.S
void enableEI();
void delay(int);


