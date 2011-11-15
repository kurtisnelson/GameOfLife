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
#define CLAMP_R(r) ((r)<0?GAME_H-1:((r)>=GAME_H?0:(r)))
#define CLAMP_C(c) ((c)<0?GAME_W-1:((c)>=GAME_W?0:(c)))
typedef enum State{TITLE, RESET, EDIT, RUN, RUN1} State;
typedef enum View{NORMAL, NEXT} View;
typedef enum Grid{CUSTOM, PRESET1, PRESET2} Grid;
typedef enum Cell{DEAD, ALIVE, DIEING, BIRTH} Cell;
extern State state;
int main();
void titleScreen();
void moveCrosshairs(int,int,int,int);
void drawCell(int,int);
void run();
void timeStep();
int neighborCount(int,int);
void applyVerbs();
void edit();
void reset();
void drawGrid();
#endif /* MAIN_H_ */
