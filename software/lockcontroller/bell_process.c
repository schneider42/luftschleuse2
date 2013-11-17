/*
 *  This file is part of the luftschleuse2 project.
 *
 *  See https://github.com/muccc/luftschleuse2 for more information.
 *
 *  Copyright (C) 2013 Tobias Schneider <schneider@muc.ccc.de> 
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "bell_process.h"
#include "../common/pinutils.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define MIN_PRESS   50
#define MAX_PRESS   2500
#define MIN_BREAK   50
#define MAX_BREAK   2500
#define MAX_BEEPS   20


static char *code = ".-..-";
static int16_t times[MAX_BEEPS + 1];
static uint8_t step;
static uint8_t debounce;
static bool pressed;
static int16_t press_timer;
static int16_t break_timer;
static bool accepted;
static uint16_t c = 0;

void bell_init(void)
{
    DDR_CONFIG_IN(BUTTON_BELL);
    PIN_SET(BUTTON_BELL);
    step = 0;
    debounce = 0;
    pressed = false;
    press_timer = -1;
    break_timer = -1;
    accepted = false;
    c = 0;
}

static inline bool isPressed(void)
{
    if( !PIN_HIGH(BUTTON_BELL) ){
        return true;

    }
    return false;
}

void bell_tick(void)
{

    if( c && --c==0 ){
        accepted = false;
    }

    if( debounce == 0 && !pressed && isPressed() ){
        debounce = 50;
        pressed = true;
        press_timer = 0;
    }

    if( debounce == 0 && pressed && !isPressed() ){
        debounce = 50;
        pressed = false;
        break_timer = 0;
    }

    if( debounce ){
        debounce--;
    }

    if( pressed && (press_timer >= 0) ){
        press_timer++;
    }

    if( !pressed && (break_timer >= 0) ){
        break_timer++;
    }

 
    if( !pressed && press_timer > 0 ){
      if ( press_timer > MIN_PRESS && press_timer < MAX_PRESS && step < MAX_BEEPS){
	times[step + 1] = 0;
	times[step++] = press_timer;
        //printf("press %d", press_timer);
      } else{
	step = 0;
        c = 2000;
        times[0] = 0;
      }
	press_timer = -1;
    }

    if( break_timer > MAX_BREAK ){
        break_timer = -1;
        step = 0;
        times[0] = 0;
    }

}

bool bell_isAccepted(void)
{
  int beeps = 0;
  int boundary;
  double best_badness = 10000000.0;
  int16_t best_boundary = 0;
  double badness;
  int beep;
  bool matches = true;

  while(times[beeps++])
    ;

  --beeps;

  if(beeps != strlen(code))
    return(false);


  for(boundary = 0; boundary <= beeps; boundary++){
    
    int nshort = 0;
    int nlong = 0 ;
    int32_t sshort = 0;
    int32_t ssshort = 0;
    int32_t slong = 0;
    int32_t sslong = 0;

    for(beep = 0; beep < beeps; beep++){
      if(times[beep] <= times[boundary]){
	++nshort;
	sshort += times[beep];
	ssshort = (int32_t)times[beep] * times[beep];
      }
      else{
	++nlong;
	slong += times[beep];
	sslong += (int32_t)times[beep] * times[beep];
      }
    }
    
    if(nshort){
      //printf("sqrt(%f) ", (double) ssshort * nshort / (double) sshort / (double) sshort - 1.0);
      badness = sqrt((double) ssshort * nshort / (double) sshort / (double) sshort - 1.0);
    }
    else{
      badness = 0.0;
    }
    if(nlong){
      //printf("sqrt(%f) ", (double) sslong * nlong / (double) slong / (double) slong - 1.0);
      badness += sqrt((double) sslong * nlong / (double) slong / (double) slong - 1.0);
    }
    //printf("badness: %f ", badness); 
    if(badness < best_badness){
      best_badness = badness;
      best_boundary = times[boundary];
    }
  }
  //printf("best_boundary: %d ", best_boundary); 
  for(beep=0; beep < beeps; beep++){
    //printf("beep: %d ", times[beep]);
    if(times[beep] <= best_boundary){
      printf("kurz ");
      if(code[beep] != '.')
	matches = false;
    }
    else{
      printf("lang ");
      if(code[beep] != '-')
	matches = false;
    } 
  }
  
  if(matches)
    c = 2000;
  printf("match: %d", matches); 
  return matches;

}

void bell_process(void)
{
}

