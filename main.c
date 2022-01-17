#include "gfx/gfx.h"

#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspge.h>
#include <pspkernel.h>
#include <psputils.h>

#define VERS 1 // version
#define REVS 0 // revision

PSP_MODULE_INFO("PinPon", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

// Collision Detection
int collision(short x, short y, short width, short height, short x1, short y1, short width1,
              short height1) {
    if (x < x1 + width1 && x + width > x1 && y < y1 + height1 && height + y > y1) {
        return 1;
    }
    else {
        return 0;
    }
}

int main() {
    sceDisplaySetMode(0, 480, 272);
    pspDebugScreenInit();
    short    y     = 136;                // Y Coords of Player 1
    short    y1    = 136;                // Y Coords of Player 2
    short    score = 1, score1 = 1;      // Score of Players
    short    x_ball = 100, y_ball = 100; // Coord of The Ball
    short    directionX;                 // X Direction velocity of The Ball
    short    directionY;                 // Y Direction velocity of The Ball
    short    speed     = 2;              // Speed of The Ball
    short    speedPat  = 2;              // Speed of Pallers
    uint32_t ballColor = 0xFFFFFFFF;
    setupExitCallback();
    GFX_init();
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    struct SceCtrlData pad;
    directionX = 1;
    directionY = 1;
    while (1) {
        GFX_clear(0x0);                                   // Clear framebuffer with black color
        sceCtrlReadBufferPositive(&pad, 1);               // Read controls
        GFX_draw_rect(10, y, 10, 60, 0x0062FF);           // Create Player 1
        GFX_draw_rect(460, y1, 10, 60, 0xFF5757);         // Create Player 2
        GFX_draw_rect(x_ball, y_ball, 10, 10, ballColor); // Create Ball

        GFX_draw_rect(10, 10, score1, 5, 0x0062FF); // Create score counter for Player 1
        GFX_draw_rect(262, 10, score, 5, 0xFF5757); // Create score counter for Player 2

        // Detect Collision between Player 1 and The Ball
        if (collision(460, y1, 10, 60, x_ball, y_ball, 10, 10)) {
            directionX = -directionX;
            ballColor  = 0xFF5757;
        }

        // Detect Collision between Player 1 and The Ball
        if (collision(10, y, 10, 60, x_ball, y_ball, 10, 10)) {
            directionX = -directionX;
            ballColor  = 0x0062FF;
        }


        x_ball += directionX * speed;
        y_ball += directionY * speed;

        if (x_ball <= 0) {
            GFX_clear(0xFF5757);
            directionX = -directionX;
            score += 5;
            ballColor = 0xFFFFFFFF;
        }
        else if (x_ball >= 470) {
            GFX_clear(0x0062FF);
            directionX = -directionX;
            score1 += 5;
            ballColor = 0xFFFFFFFF;
        }

        if (y_ball <= 0) {
            directionY = -directionY;
        }
        else if (y_ball >= 262) {
            directionY = -directionY;
        }

        if (pad.Buttons & PSP_CTRL_DOWN) {
            y += speedPat;
        }
        else if (pad.Buttons & PSP_CTRL_UP) {
            y -= speedPat;
        }

        if (pad.Buttons & PSP_CTRL_CROSS) {
            y1 += speedPat;
        }
        else if (pad.Buttons & PSP_CTRL_TRIANGLE) {
            y1 -= speedPat;
        }
        if (pad.Buttons & PSP_CTRL_START) {
            score  = 0;
            score1 = 0;
            x_ball = 100;
            y_ball = 100;
            y      = 136;
            y1     = 136;
            speed  = 2;
        }
        GFX_swap_buffers();
        sceDisplayWaitVblankStart();
    }
    sceKernelExitGame();
    return 0;
}