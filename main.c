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
View vMode = NORMAL;
Grid grid = CUSTOM;
static Cell gameArr[GAME_W * GAME_H];
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
      if (state == EDIT)
        edit();
      if (state == RUN || state == RUN1)
        run();
    }
  return (0);
}

void
edit()
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
          if (GET_CELL(r,c) == ALIVE || GET_CELL(r,c) == BIRTH)
            GET_CELL(r,c) = DEAD;
          else if (GET_CELL(r,c) == DEAD || GET_CELL(r,c) == DIEING)
            GET_CELL(r,c) = ALIVE;
          moveCrosshairs(r, c, r, c);
          flipPage();
          moveCrosshairs(r, c, r, c);
          grid = CUSTOM;
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
      if (KEY_DOWN_NOW(KEY_L))
        {
          KEY_RELEASED_WAIT(KEY_L);
          if (vMode == NORMAL)
            vMode = NEXT;
          else
            vMode = NORMAL;
          drawGrid();
          flipPage();
          drawGrid();
        }
      //Swap in an example game.
      if (KEY_DOWN_NOW(KEY_R))
        {
          KEY_RELEASED_WAIT(KEY_R);
          if (grid != PRESET1)
            {
              grid = PRESET1;
              for (int i = 0; i < GAME_W * GAME_H; i++)
                gameArr[i] = DEAD;
              gameArr[0] = ALIVE;
              gameArr[1] = ALIVE;
              gameArr[2] = ALIVE;
              drawGrid();
              flipPage();
              drawGrid();
            }
          else if (grid != PRESET2)
            {
              grid = PRESET2;
              for (int i = 0; i < GAME_W * GAME_H; i++)
                gameArr[i] = DEAD;
              gameArr[1] = ALIVE;
              gameArr[GAME_W+2] = ALIVE;
              gameArr[2*GAME_W] = ALIVE;
              gameArr[2*GAME_W+1] = ALIVE;
              gameArr[2*GAME_W+2] = ALIVE;
              drawGrid();
              flipPage();
              drawGrid();
            }
        }
      //Run the simulation
      if (KEY_DOWN_NOW(KEY_START))
        {
          KEY_RELEASED_WAIT(KEY_START);
          state = RUN;
          return;
        }
      if (KEY_DOWN_NOW(KEY_B))
        {
          KEY_RELEASED_WAIT(KEY_B);
          state = RUN1;
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
          state = EDIT;
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
run()
{
  volatile u32 slow = 0;
  int count = 0;
  while (1)
    {
      if(vMode == NEXT && count%2)
        applyVerbs();
      else
        timeStep();
      drawGrid();
      flipPage();
      if(state == RUN1)
        {
          state = EDIT;
          return;
        }
      while (slow < 50000)
        {
          if (KEY_DOWN_NOW(KEY_START))
            {
              state = EDIT;
              return;
            }
          if (KEY_DOWN_NOW(KEY_SELECT))
            {
              KEY_RELEASED_WAIT(KEY_SELECT);
              state = RESET;
              return;
            }
          if (KEY_DOWN_NOW(KEY_L))
            {
              KEY_RELEASED_WAIT(KEY_L);
              if (vMode == NORMAL)
                vMode = NEXT;
              else
                vMode = NORMAL;
            }
          slow++;
        }
      slow = 0;
      count++;
    }
}

void
timeStep()
{
  Cell newGameArr[GAME_W * GAME_H];
  for (int i = 0; i < GAME_W * GAME_H; i++)
    {
      int c = neighborCount(i / GAME_W, i % GAME_W);
      if (c < 2 && gameArr[i] == ALIVE)
        newGameArr[i] = (vMode==NORMAL?DEAD:DIEING);
      else if (c > 3 && gameArr[i] == ALIVE)
        newGameArr[i] = (vMode==NORMAL?DEAD:DIEING);
      else if (c == 3 && gameArr[i] == DEAD)
        newGameArr[i] = (vMode==NORMAL?ALIVE:BIRTH);
      else
        newGameArr[i] = gameArr[i];
    }
  for (int i = 0; i < GAME_W * GAME_H; i++)
    {
      gameArr[i] = newGameArr[i];
    }
}

//Change Cells to ALIVE or DEAD if dieing or birthing
void
applyVerbs()
{
  for(int i = 0; i<GAME_W*GAME_H; i++)
          {
            if(gameArr[i] == BIRTH)
              gameArr[i] = ALIVE;
            else if(gameArr[i] == DIEING)
              gameArr[i] = DEAD;
          }
}

int
neighborCount(int r, int c)
{
  int count = 0;
  //Left
  if (gameArr[CLAMP_C(c - 1) + (r * GAME_W)] == ALIVE)
    count++;
  //Right
  if (gameArr[CLAMP_C(c + 1) + (r * GAME_W)] == ALIVE)
    count++;
  //Up
  if (gameArr[c + (CLAMP_R(r - 1) * GAME_W)] == ALIVE)
    count++;
  //Down
  if (gameArr[c + (CLAMP_R(r + 1) * GAME_W)] == ALIVE)
    count++;
  //UL
  if(gameArr[CLAMP_C(c-1) + (CLAMP_R(r-1) * GAME_W)] == ALIVE)
    count++;
  //UR
  if(gameArr[CLAMP_C(c+1) + (CLAMP_R(r-1) * GAME_W)] == ALIVE)
      count++;
  //DL
  if(gameArr[CLAMP_C(c-1) + (CLAMP_R(r+1) * GAME_W)] == ALIVE)
      count++;
  //DR
  if(gameArr[CLAMP_C(c+1) + (CLAMP_R(r+1) * GAME_W)] == ALIVE)
      count++;
  return (count);
}

void
drawCell(int r, int c)
{
  Cell cell = GET_CELL(r,c);
  if (cell == ALIVE)
    drawImage4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, alive);
  if (cell == DEAD)
    drawRect4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, 0);
  if (vMode == NEXT)
    {
      if (cell == BIRTH)
        drawImage4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, birth);
      if (cell == DIEING)
        drawImage4(r * CELL_H, c * CELL_W, CELL_W, CELL_H, dieing);
    }
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
