#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>
/*
#badge    <s> 96x68
#badge    <s> das farbige hat 98x70 
*/

#define RESX 96
#define RESY 68
#define RESY_B 9

#define LCD_INVERTED (1<<0)
#define LCD_MIRRORX  (1<<1)
#define LCD_MIRRORY  (1<<2)

/* Display buffer */
extern uint8_t lcdBuffer[RESX*RESY_B];

void lcdInit(void);
void lcdFill(char f);
void lcdDisplay(void);
void lcdInvert(void);
void lcdToggleFlag(int flag);
void lcdSetPixel(char x, char y, bool f);
bool lcdGetPixel(char x, char y);
void lcdShift(int x, int y, bool wrap);
void lcdSetContrast(int c);
void lcdSetInvert(int c);
#endif
