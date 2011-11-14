/*
 * main.c
 *
 *  Created on: Nov 13, 2011
 *      Author: kurt
 */
#include "main.h"
#include "lib/gba.h"
#include "lib/video.h"
#include "lib/input.h"
#include "images.h"
State state = RESET;
Cell gameArr[GAME_W * GAME_H];
int
main()
{
  vMode4();
  reset();
  while (1)
    {
      if (state == RESET)
        reset();
      if (state == TITLE)
        titleScreen();
      if (state == NORMAL)
        normal();
    }
  return (0);
}

void
normal()
{
  int r = GAME_H / 2;
  int c = GAME_W / 2;
  drawGrid();
  flipPage();
  drawGrid();
  moveCrosshairs(r, c, r, c);
  flipPage();
  while (1)
    {
      if (KEY_DOWN_NOW(KEY_A))
        {
          KEY_RELEASED_WAIT(KEY_A);
          if (GET_CELL(r,c) == ALIVE)
            GET_CELL(r,c) = DEAD;
          else if (GET_CELL(r,c) == DEAD)
            GET_CELL(r,c) = ALIVE;
          moveCrosshairs(r, c, r, c);
          flipPage();
          moveCrosshairs(r, c, r, c);
        }
      if (KEY_DOWN_NOW(KEY_LEFT))
        {
          KEY_RELEASED_WAIT(KEY_LEFT);
          int newC = c - 1;
          if (c <= 0)
            newC = GAME_W - 1;
          moveCrosshairs(r, c, r, newC);
          flipPage();
          moveCrosshairs(r, c, r, newC);
          c = newC;
        }
      if (KEY_DOWN_NOW(KEY_RIGHT))
        {
          KEY_RELEASED_WAIT(KEY_RIGHT);
          int newC = c + 1;
          if (c >= GAME_W - 1)
            newC = 0;
          moveCrosshairs(r, c, r, newC);
          flipPage();
          moveCrosshairs(r, c, r, newC);
          c = newC;
        }
      if (KEY_DOWN_NOW(KEY_DOWN))
        {
          KEY_RELEASED_WAIT(KEY_DOWN);
          int newR = r + 1;
          if (r >= GAME_H - 1)
            newR = 0;
          moveCrosshairs(r, c, newR, c);
          flipPage();
          moveCrosshairs(r, c, newR, c);
          r = newR;
        }
      if (KEY_DOWN_NOW(KEY_UP))
        {
          KEY_RELEASED_WAIT(KEY_UP);
          int newR = r - 1;
          if (r <= 0)
            newR = GAME_H - 1;
          moveCrosshairs(r, c, newR, c);
          flipPage();
          moveCrosshairs(r, c, newR, c);
          r = newR;
        }
      if (KEY_DOWN_NOW(KEY_SELECT))
        {
          KEY_RELEASED_WAIT(KEY_SELECT);
          state = RESET;
          return;
        }
    }
}

void
moveCrosshairs(int or, int oc, int r, int c)
{
  drawCell(or, oc);
  drawX4(r * CELL_H + 4, c * CELL_W + 4, 35);
}
void
titleScreen()
{
  drawImage4(0, 0, START_WIDTH, START_HEIGHT, start);
  flipPage();
  while (1)
    {
      if (KEY_DOWN_NOW(KEY_START))
        {
          state = NORMAL;
          return;
        }
      if (KEY_DOWN_NOW(KEY_SELECT))
        {
          KEY_RELEASED_WAIT(KEY_SELECT);
          state = RESET;
          return;
        }
    }
}

void
drawGrid()
{
  for (int c = 0; c < GAME_W; c++)
    {
      for (int r = 0; r < GAME_H; r++)
        {
          drawCell(r, c);
        }
    }
}

void
drawCell(int r, int c)
{
  Cell cell = GET_CELL(r,c);
  if (cell == ALIVE)
    drawImage4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, alive);
  if (cell == DEAD)
    drawRect4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, 0);
  if (cell == BIRTH)
    drawImage4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, birth);
  if (cell == DIEING)
    drawImage4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, dieing);
}

void
reset()
{
  for (int i = 0; i < IMAGES_PALETTE_SIZE; i++)
    {
      palette[i] = images_palette[i];
    }
  for (int i = 0; i < GAME_W * GAME_H; i++)
    gameArr[i] = DEAD;
  drawRect4(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  flipPage();
  drawRect4(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  state = TITLE;
}
