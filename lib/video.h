/*
 * video.h
 *
 *  Created on: Nov 7, 2011
 *      Author: kurt
 */

#ifndef VIDEO_H_
#define VIDEO_H_
#include "gba.h"
#include "color.h"
#define REG_VCOUNT (*(REG *)0x04000006)
#define REG_DISPCNT (*(REG *)0x04000000)
#define DCNT_MODE3 0x0003
#define DCNT_MODE4 0x0004
#define DCNT_BG2 0x0400
#define DCNT_PAGE 0x10
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 160
extern u16 *palette;
extern u16 *videoBuffer;
void setPixel3(u32, u32, COLOR);
void setPixel4(u32, u32, u8);
COLOR color(u8, u8, u8);
void drawRect3(u32, u32, u32, u32, COLOR);
void drawRect4(u32, u32, u32, u32, u8);
void waitForVBlank();
void flipPage();
void drawImage3(int, int, int, int, const u16*);
void drawImage4(int, int, int, int, const u16*);
void autoDraw3(const u16*);
void drawX4(int, int, u8);
void vMode3();
void vMode4();
#endif /* VIDEO_H_ */
