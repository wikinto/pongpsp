#include "common/callback.h"
#include "gfx/gfx.h"
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>

#define VERS    1 // version
#define REVS    0 // revision
#define printf pspDebugScreenPrintf
PSP_MODULE_INFO("PinPon", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);


int collision(int x, int y, int width, int height, int x1, int y1, int width1, int height1){
		if (x < x1 + width1 &&
        x + width > x1 &&
        y< y1 + height1 &&
        height + y > y1) {	
			return 1;
		}
		return 0;
}

int main() {
	int y = 136; // Y Coords of Player 1
	int y1 = 136; // Y Coords of Player 2
	int x_ball = 100, y_ball = 100;
	short directionX;
	short directionY;
    setupExitCallback();
    GFX_init();
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	struct SceCtrlData pad;
	directionX = 1;
	directionY = 1;
    while(1) {
        GFX_clear(0x0); //#82CAFFFF RGBA in Hex -> 0xFFFFCA82
		sceCtrlReadBufferPositive(&pad,1);
        GFX_draw_rect(10, y, 10, 60, 0x0062FF); //Player 1
		GFX_draw_rect(460, y1, 10, 60, 0xFF5757); //Player 2
		GFX_draw_rect(x_ball,y_ball, 10, 10, 0xFFFFFFFF); //Ball

		if (10 < x_ball + 10 &&
        10 + 10 > x_ball &&
        y < y_ball + 10 &&
        60 + y > y_ball) {
			directionX = -directionX;
			directionY = -directionY;
		}


		if (460 < x_ball + 10 &&
        	460 + 10 > x_ball &&
        	y1 < y_ball + 10 &&
        	60 + y1 > y_ball) {
		directionX = -directionX;
		directionY = -directionY;
		}
		x_ball += directionX;
		y_ball += directionY;

		if(x_ball <= 0){
			directionX = -directionX;
		}
		else if(x_ball >= 470){
			directionX = -directionX;
		}

		if(y_ball <= 0){
			directionY = -directionY;
		}
		else if(y_ball >= 262){
			directionY = -directionY;
		}
		
		if(pad.Buttons & PSP_CTRL_DOWN){
			y++;
		}
		else if (pad.Buttons & PSP_CTRL_UP){
			y--;
		}

		if(pad.Buttons & PSP_CTRL_CROSS){
			y1++;
		}
		else if (pad.Buttons & PSP_CTRL_TRIANGLE){
			y1--;
		}

        GFX_swap_buffers();
        sceDisplayWaitVblankStart();
    }
}