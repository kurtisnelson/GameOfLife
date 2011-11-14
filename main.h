/*
 * main.h
 *
 *  Created on: Nov 13, 2011
 *      Author: kurt
 */

#ifndef MAIN_H_
#define MAIN_H_
#define GAME_W 30
#define GAME_H 20
#define GET_CELL(r,c) gameArr[(c) + (r) * GAME_W]
typedef enum State{TITLE, RESET, NORMAL, NEXT, SIM, SIM_NEXT} State;
typedef enum Cell{ALIVE, DEAD, DIEING, BIRTH} Cell;
extern State state;
int main();
void titleScreen();
void moveCrosshairs(int,int,int,int);
void drawCell(int,int);
void normal();
void reset();
void drawGrid();
#endif /* MAIN_H_ */
