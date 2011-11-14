/*
 * Exported with BrandonTools v0.9
 * Invocation command was BrandonTools -mode4 images alive.png birth.png dieing.png start.png 
 * 
 * Image Information
 * -----------------
 * alive.png 8@8
 * birth.png 8@8
 * dieing.png 8@8
 * start.png 240@160
 * 
 * Quote/Fortune of the Day!
 * -------------------------
 * A conclusion is the place where you got tired of thinking.  ~Attributed to Arthur McBride Bloch
 * 
 * All bug reports / feature requests are to be sent to Brandon (brandon.whitehead@gatech.edu)
 */

#ifndef IMAGES_BITMAP_H
#define IMAGES_BITMAP_H
#define CELL_H 8
#define CELL_W 8
#define IMAGES_PALETTE_SIZE 36

extern const unsigned short images_palette[36];

extern const unsigned short alive[32];
#define ALIVE_WIDTH 8
#define ALIVE_HEIGHT 8

extern const unsigned short birth[32];
#define BIRTH_WIDTH 8
#define BIRTH_HEIGHT 8

extern const unsigned short dieing[32];
#define DIEING_WIDTH 8
#define DIEING_HEIGHT 8

extern const unsigned short start[19200];
#define START_WIDTH 240
#define START_HEIGHT 160

#endif
