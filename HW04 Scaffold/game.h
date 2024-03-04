#ifndef GAME_H
#define GAME_H

// extern values
extern int score;
extern int lives;
extern int playerCollidedWithEnemy;

// add colors to palette (took inspiration from lab 6)
extern unsigned short colors[6];
enum {BLACKID=(256 - 6), ORANGEID, DARKREDID, YELLOWID, LAVPINKID, TURQUOISEID};

// game function prototypes
void initGame();
void updateGame();
void drawGame();





// player struct
typedef struct {
	int x;
	int y;
    int oldX;
    int oldY;
    int width; 
    int height;
} PLAYER;

extern PLAYER player;

// player function prototypes
void initPlayer(); 
void updatePlayer();
void drawPlayer();






// enemy struct
typedef struct {
	int x;
	int y;
    int oldX;
    int oldY;
    int width; 
    int height;
    unsigned short color;
} ENEMY;

extern ENEMY enemy;

// enemy function prototypes
void initEnemy(); 
void updateEnemy();
void drawEnemy(); 







// cheese struct
typedef struct {
    int x;
    int y;
    int oldX;
    int oldY;
    int width;
    int height;
    int active;
    int erased; 
    unsigned short color; 
} CHEESE;

// cheese properties
#define CHEESECOUNT 12
extern CHEESE cheese[CHEESECOUNT];

// cheese function prototypes
void initCheese();
void updateCheese();
void drawCheese();
void newCheese();





// slow block struct
typedef struct {
    int x;
    int y;
    int width;
    int height;
    unsigned short color;
} SLOWBLOCK;

extern SLOWBLOCK slowBlock;

// slow block function prototypes
void initSlowBlock();
void drawSlowBlock();

#endif