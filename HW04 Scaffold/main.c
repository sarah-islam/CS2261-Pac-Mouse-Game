#include "gba.h"
#include "print.h"
#include "mode4.h"
#include <stdio.h>
#include "game.h"
#include "startscreen.h"
#include "analogSound.h"

void initialize();

// state function prototypes
void start();
void game();
void pause();
void lose();
void win();
void goToStart();
void goToGame();
void goToPause();
void goToLose();
void goToWin();

// button vars
unsigned short buttons;
unsigned short oldButtons;

// character buffer
char buffer[50];

// state enum
enum STATE {
    START,
    GAME,
    PAUSE,
    LOSE,
    WIN
} state;

int main() {

    initialize();

    while (1) {
        oldButtons = buttons;
        buttons = REG_BUTTONS;

        // state machine
        switch (state) {
            case START:
                start();
                break;
            case GAME:
                game();
                break;
            case PAUSE:
                pause();
                break;
            case LOSE:
                lose();
                break;
            case WIN:
                win();
                break;
        }
    }
    return 0;

}

void initialize() {
    mgba_open();

    REG_DISPCTL = MODE(4) | BG2_ENABLE | DISP_BACKBUFFER;
    
    oldButtons = 0;
    buttons = REG_BUTTONS;

    goToStart();

}

void goToStart() {
    DMANow(3, startscreenPal, BG_PALETTE, 256);
    drawFullscreenImage4(startscreenBitmap);

    waitForVBlank();
    flipPages();

    state = START;
}

void start() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {        
        goToGame();
        initGame();
    }
}

void goToGame() {
    state = GAME;
}

void game() {
    updateGame();
    drawGame();

    // draw lives and score
    drawString4(180, 145, "lives: ", TURQUOISEID);
    sprintf(buffer, "%d", lives);
    drawString4(220, 145, buffer, TURQUOISEID);
    drawString4(180, 5, "score: ", TURQUOISEID);
    sprintf(buffer, "%d", score);
    drawString4(220, 5, buffer, TURQUOISEID);

    waitForVBlank();
    flipPages();

    if (score == 21) {
        goToWin();
    }
    if (lives == 0) {
        goToLose();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToPause();
    }
}

void goToPause() {
    fillScreen4(DARKREDID);
    drawString4(10, 65, "break so soon? press start to resume", LAVPINKID);
    drawString4(10, 75, "or press select to restart game!", LAVPINKID); 

    waitForVBlank();
    flipPages();

    state = PAUSE;
}

void pause() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToGame();
    } else if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToStart();
    }
}

void goToLose() {
    fillScreen4(DARKREDID);
    drawString4(10, 65, "UH OH, you lost! D:", LAVPINKID);
    drawString4(10, 75, "press start to try again!", LAVPINKID); 

    waitForVBlank();
    flipPages();

    state = LOSE;
}

void lose() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToWin() {
    fillScreen4(DARKREDID);
    drawString4(10, 65, "HOORAY YOU WON! :D", LAVPINKID);
    drawString4(10, 75, "press start to replay!", LAVPINKID); 

    waitForVBlank();
    flipPages();

    state = WIN;
}

void win() {
    waitForVBlank();

    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}