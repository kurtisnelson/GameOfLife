/*
 * video.c
 *
 *  Created on: Nov 7, 2011
 *      Author: kurt
 */
#include "video.h"
#include "dma.h"
#include "gba.h"

u16 *palette = (u16 *) 0x05000000;
u16 *backBuffer = (u16 *) 0x06000000;
u16 *videoBuffer = (u16 *) 0x0600a000;

void
vMode3()
{
  REG_DISPCNT = DCNT_MODE3 | DCNT_BG2;
}

void
vMode4()
{
  REG_DISPCNT = DCNT_MODE4 | DCNT_BG2 | DCNT_PAGE;
}

void
setPalette(u8 addr, COLOR color)
{
  palette[addr] = color;
}

void
flipPage()
{
  u16 *temp = videoBuffer;
  videoBuffer = backBuffer;
  backBuffer = temp;
  waitForVBlank();
  REG_DISPCNT ^= DCNT_PAGE;
}

void
setPixel4(u32 r, u32 c, u8 color)
{
  u16 *dst = &backBuffer[(r * SCREEN_WIDTH + c) / 2];
  if (c & 1)
    *dst = (*dst & 0xFF) | (color << 8); // odd pixel
  else
    *dst = (*dst & ~0xFF) | color; // even pixel
}

void
setPixel3(u32 r, u32 c, COLOR color)
{
  videoBuffer[(r * SCREEN_WIDTH) + c] = color;
}

void
waitForVBlank()
{
  while (REG_VCOUNT >= 160)
    ; // wait till VDraw
  while (REG_VCOUNT < 160)
    ; // wait till VBlank
}

void
drawImage3(int r, int c, int width, int height, const u16* image)
{
  if (width == SCREEN_WIDTH)
    {
      REG_DMA3SAD = (u32) image;
      REG_DMA3DAD = (u32) videoBuffer;
      REG_DMA3CNT = 0;
      REG_DMA3CNT |= SCREEN_WIDTH * height | DMA_ENABLE | DMA_16;
    }
  else
    {
      int row = 0;
      while (row < height)
        {
          REG_DMA3SAD = (u32) image;
          REG_DMA3DAD = (u32) (videoBuffer + ((r + row) * SCREEN_WIDTH) + c);
          REG_DMA3CNT = 0;
          REG_DMA3CNT |= width | DMA_ENABLE | DMA_16;
          image += width;
          row++;
        }
    }
}

void
drawImage4(int r, int c, int width, int height, const u16* image)
{
  if (width == SCREEN_WIDTH)
    {
      REG_DMA3SAD = (u32) image;
      REG_DMA3DAD = (u32) backBuffer;
      REG_DMA3CNT = (SCREEN_WIDTH * height) / 2 | DMA_ENABLE | DMA_16;
    }
  else
    {
      int row = 0;
      while (row < height)
        {
          REG_DMA3SAD = (u32) image;
          REG_DMA3DAD = (u32) (backBuffer + ((r + row) * SCREEN_WIDTH/2) + c/2);
          REG_DMA3CNT = 0;
          REG_DMA3CNT |= width / 2 | DMA_ENABLE | DMA_16;
          image += width/2;
          row++;
        }
    }
}

void
drawRect4(u32 r, u32 c, u32 width, u32 height, u8 palette)
{
  u16 val = palette + (palette << 8);
  for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width/2; j++)
        {
          backBuffer[((c/2)+j) + (SCREEN_WIDTH/2)*(i+r)] = val;
        }
    }
}

void
drawRect3(u32 r, u32 c, u32 width, u32 height, COLOR color)
{
  if (width == SCREEN_WIDTH && height == SCREEN_HEIGHT)
    {
      REG_DMA3SAD = (u32) &color;
      REG_DMA3DAD = (u32) videoBuffer;
      REG_DMA3CNT = 0;
      REG_DMA3CNT |= SCREEN_WIDTH * SCREEN_HEIGHT | DMA_ENABLE | DMA_16
          | DMA_SRC_FIXED;
    }
  else
    {
      int row = 0;
      while (row < height)
        {
          REG_DMA3SAD = (u32) &color;
          REG_DMA3DAD = (u32) (videoBuffer + ((r + row) * SCREEN_WIDTH) + c);
          REG_DMA3CNT = 0;
          REG_DMA3CNT |= width | DMA_ENABLE | DMA_16 | DMA_SRC_FIXED;
          row++;
        }
    }
}

void
drawX4(int r, int c, u8 palette)
{
  setPixel4(r-2,c-2,palette);
  setPixel4(r+2,c-2,palette);
  setPixel4(r-2,c+2,palette);
  setPixel4(r+2,c+2,palette);
  setPixel4(r-1,c-1,palette);
  setPixel4(r+1,c-1,palette);
  setPixel4(r-1,c+1,palette);
  setPixel4(r+1,c+1,palette);
}

void
autoDraw3(const u16 * screen)
{
  REG_DMA3SAD = (u32) screen;
  REG_DMA3DAD = (u32) videoBuffer;
  REG_DMA3CNT = 0;
  REG_DMA3CNT |= SCREEN_WIDTH * SCREEN_HEIGHT | DMA_ENABLE | DMA_16;
}
