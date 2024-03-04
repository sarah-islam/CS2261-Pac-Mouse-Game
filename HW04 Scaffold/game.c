#include "game.h"
#include "gba.h"
#include "mode4.h"
#include "print.h"
#include "mouse.h"
#include "analogSound.h"

// struct vars
PLAYER player;
ENEMY enemy;
SLOWBLOCK slowBlock;
CHEESE cheese[CHEESECOUNT];

// game vars
int score;
int lives;
int playerCollidedWithEnemy = 0;




// GAME LOGIC

// initializes game
void initGame() {
    initPlayer();
    initEnemy();
    initCheese();
    initSlowBlock();

    score = 0;
    lives = 3;
    DMANow(3, mousePal, BG_PALETTE, 256);

    // add more colors to end of palette (inspiration from lab 6)
    unsigned short colors[6] = {BLACK, ORANGE, DARKRED, YELLOW, LAVPINK, TURQUOISE};
    for (int i = 0; i < 6; i++) {
        BG_PALETTE[256 - 6 + i] = colors[i];
    }
}

// updates game
void updateGame() {
    updateCheese();
    updatePlayer();
    updateEnemy();

    if (BUTTON_PRESSED(BUTTON_A)) {
        newCheese();
    }
}

// draws game
void drawGame() {
    fillScreen4(BLACKID);
    drawSlowBlock();
    drawCheese();
    drawPlayer();
    drawEnemy();
}







// PLAYER LOGIC

// initializes player
void initPlayer() {
    player.x = 0;
    player.y = 100;
    player.oldX = player.x;
    player.oldY = player.y;
    player.height = 24;
    player.width = 24;
}

// updates player: movement, speed when colliding with slow block (changes colors), collision with enemy (decreases lives)
void updatePlayer() {
    player.oldX = player.x;
    player.oldY = player.y;

    if (collision(player.x, player.y, player.width, player.height, slowBlock.x, slowBlock.y, slowBlock.width, slowBlock.height)) {
        slowBlock.color = BLACKID;
        if (BUTTON_HELD(BUTTON_UP)) {
            player.y -= 2;
            if (player.y < 0) {
                player.y = 0;
            }
        }
        if (BUTTON_HELD(BUTTON_DOWN)) {
            player.y += 2;
            if (player.y > SCREENHEIGHT - player.height) {
                player.y = SCREENHEIGHT - player.height;
            }
        }
        if (BUTTON_HELD(BUTTON_RIGHT)) {
            player.x += 2;
            if (player.x > SCREENWIDTH - player.width) {
                player.x = SCREENWIDTH - player.width;
            }
        }
        if (BUTTON_HELD(BUTTON_LEFT)) {
            player.x -= 2;
            if (player.x < 0) {
                player.x = 0;
            }
        }
    } else {
        slowBlock.color = DARKREDID;
        if (BUTTON_HELD(BUTTON_UP)) {
            player.y -= 4;
            if (player.y < 0) {
                player.y = 0;
            }
        }
        if (BUTTON_HELD(BUTTON_DOWN)) {
            player.y += 4;
            if (player.y > SCREENHEIGHT - player.height) {
                player.y = SCREENHEIGHT - player.height;
            }
        }
        if (BUTTON_HELD(BUTTON_RIGHT)) {
            player.x += 4;
            if (player.x > SCREENWIDTH - player.width) {
                player.x = SCREENWIDTH - player.width;
            }
        }
        if (BUTTON_HELD(BUTTON_LEFT)) {
            player.x -= 4;
            if (player.x < 0) {
                player.x = 0;
            }
        }
    }
    
    if (collision(player.x, player.y, player.width, player.height, enemy.x, enemy.y, enemy.width, enemy.height) && lives > 0 && !playerCollidedWithEnemy) {
        playAnalogSound(10);
        lives--;
        playerCollidedWithEnemy = 1;
    }
    // resets collision var if player no longer colliding with enemy
    if (!collision(player.x, player.y, player.width, player.height, enemy.x, enemy.y, enemy.width, enemy.height)) {
        playerCollidedWithEnemy = 0;
    }
}

// draws player
void drawPlayer() {
    drawImage4(player.x, player.y, player.width, player.height, mouseBitmap);
}







// ENEMY LOGIC

// initializes enemy
void initEnemy() {
    enemy.x = 10;
    enemy.y = 10;
    enemy.oldX = enemy.x;
    enemy.oldY = enemy.y;
    enemy.height = 16;
    enemy.width = 16;
    enemy.color = ORANGEID;
}

// updates enemy: follows player
void updateEnemy() {
    enemy.oldX = enemy.x;
    enemy.oldY = enemy.y;

    if (player.x > enemy.x) {
        enemy.x += 1;
    } else if (player.x < enemy.x) {
        enemy.x -= 1;
    }
    if (player.y > enemy.y) {
        enemy.y += 1;
    } else if (player.y < enemy.y) {
        enemy.y -= 1;
    }
}

// draws enemy
void drawEnemy() {
    drawRect4(enemy.oldX, enemy.oldY, enemy.width, enemy.height, BLACKID);
    drawRect4(enemy.x, enemy.y, enemy.width, enemy.height, ORANGEID);
}








// CHEESE LOGIC

// initializes cheese at random locations
void initCheese() {
    for (int i = 0; i < CHEESECOUNT; i++) {
        cheese[i].x = rand() % (SCREENWIDTH - player.width - cheese[i].width);
        cheese[i].y = rand() % (SCREENHEIGHT - player.height - cheese[i].height);
        
        cheese[i].width = 3;
        cheese[i].height = 3;
        cheese[i].active = 1;
        cheese[i].erased = 0;
        cheese[i].color = YELLOWID;
    }
}

// updates cheese: if player collides with cheese, cheese is erased and score is incremented
void updateCheese() {
    for (int i = 0; i < CHEESECOUNT; i++) {
        if (cheese[i].active && !cheese[i].erased) {
            if (collision(player.x, player.y, player.width, player.height, cheese[i].x, cheese[i].y, cheese[i].width, cheese[i].height)) {
                cheese[i].erased = 1;
                cheese[i].active = 0;
                drawRect4(cheese[i].x, cheese[i].y, cheese[i].width, cheese[i].height, BLACKID);

                playAnalogSound(7);
                score++;
            }
        }
    }
}

// draws cheese
void drawCheese() {
    for (int i = 0; i < CHEESECOUNT; i++) {
        if (cheese[i].active && !cheese[i].erased) {
            drawRect4(cheese[i].x, cheese[i].y, cheese[i].width, cheese[i].height, cheese[i].color);
        }
    }
}

// this function generates three cheese at random. if there are no inactive cheese it only changes location of 3 cheese already active.
void newCheese() {
    for (int i = 0; i < 3; i++) {
        int index = rand() % CHEESECOUNT;
        cheese[index].active = 1;
        cheese[index].erased = 0;
        cheese[index].x = rand() % (SCREENWIDTH - cheese[index].width);
        cheese[index].y = rand() % (SCREENHEIGHT - cheese[index].height);
    }
}






// SLOW BLOCK LOGIC (block slows player down when colliding with it)

// initializes slow block
void initSlowBlock() {
    slowBlock.x = 160;
    slowBlock.y = 80;
    slowBlock.width = 60;
    slowBlock.height = 60;
    slowBlock.color = DARKREDID;
}

// draws slow block
void drawSlowBlock() {
    drawRect4(slowBlock.x, slowBlock.y, slowBlock.width, slowBlock.height, slowBlock.color);
}