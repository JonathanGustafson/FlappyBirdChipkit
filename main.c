/*****************************************************
This file contains all the game logic.
-----------------------------------------------------
This file is written by
Erik Gustavsson and Jonathan Gustafson
*****************************************************/
#include <stdint.h>
#include <stdio.h>
#include <pic32mx.h>
#include "header.h"

//height and width of the display
#define SCREEN_HEIGHT 32
#define SCREEN_WIDTH 128
//height and width of the bird
#define BIRD_HEIGHT 3
#define BIRD_WIDTH 3
//width of the pipe
#define PIPE_WIDTH 5

int birdX = SCREEN_WIDTH/3; //birds x coordinates
int birdY = SCREEN_HEIGHT/4; //birds y coordinates

int pipe1X = SCREEN_WIDTH;  //pipe1s x-coordinates
int pipe2X = SCREEN_WIDTH + (SCREEN_WIDTH/2);  //pipe2s x-coordinates
int pipeMinDist = 10; //Minimum distance between top and bot pipe

int randomPipe1_bot, randomPipe2_bot; //random ints for the length of the pipe bottoms
int randomPipe1_top, randomPipe2_top; //random ints for the length of the pipe tops

int gamePlaying = 0; //works as a boolean value that indicates if the game is playing or not
int jumpedrecently; //Used to avoid double jumps
int score;
int collided = 0;

uint8_t display[128*4] = {0}; //Display

/*
Random from https://stackoverflow.com/questions/26237419/faster-than-rand
using the follwing licence  CC BY-SA 3.0 https://stackoverflow.com/help/licensing
Written by "Asis" Oct 7 '14 , edited by "ElementW" Oct 9 '16
*/

static unsigned int g_seed;

// Used to seed the generator.
inline void fast_srand(int seed) {
    g_seed = seed;
}

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void) {
    g_seed = (214013*g_seed+2531011);
    return (g_seed>>16)&0x7FFF;
}
/*
  End of random
*/

int collision(){
  int i,j;
    /*** checks if the bird hit the ground   ***/
    if (birdY >= 32){
      return 1;
    }

    /*** if bird is at the same x-coordinate as a pipe, check if the bird is in the gap ***/
    if(birdX >= pipe1X - PIPE_WIDTH && birdX <= pipe1X){
      if(birdY < randomPipe1_top || birdY > (32-randomPipe1_bot))
        return 1;
    }
    /*** same as above but for the 2nd pipe ***/
    if(birdX >= pipe2X - PIPE_WIDTH && birdX <= pipe2X){
      if(birdY < randomPipe2_top || birdY > (32-randomPipe2_bot))
          return 1;
    }
    return 0;
}

void user_isr( void )
{
  //Check timer flag
  if(IFS(0) & 0x100){
      IFS(0) &= 0xfffffeff; //Reset flag

    /************GAMEPAUSE*************/
    if(!gamePlaying)
      return;
    /************Bird Y-movement*************/
    birdY += 1;
    jumpedrecently = 0;

    /************Pipe X-movement*************/
    pipe1X -= 1;
    pipe2X -= 1;

    /*************Pipe-spawning**************/

    if(pipe1X < 0){
      pipe1X = SCREEN_WIDTH;
      randomPipe1_bot = 4 + (fast_rand() % 11);
      randomPipe1_top = 4 + (fast_rand() % 11);

      //makes sure that the distance for the gap is not larger than the minimum distance
      while(randomPipe1_bot + randomPipe1_top > 32-pipeMinDist && pipeMinDist >= 0){
        randomPipe1_top = randomPipe1_top -1;
      }
    }
    if(pipe2X < 0){
      pipe2X = SCREEN_WIDTH;
      randomPipe2_bot = 4 + (fast_rand() % 11);
      randomPipe2_top = 4 + (fast_rand() % 11);
    }
    //makes sure that the distance for the gap is not larger than the minimum distance
    while(randomPipe2_bot + randomPipe2_top > 32-pipeMinDist && pipeMinDist >= 0){
      randomPipe2_top = randomPipe2_top -1;
    }

    /***************Colisions****************/
      collided = collision(); //check for collisions;

    /*************Score-counting*************/
    if(pipe2X == birdX || pipe1X == birdX){
      score++;

      //makes the minimumdistance shorter as the game progress.
      if((score % 1) == 0 && pipeMinDist > 5)
        pipeMinDist --;

      /*** LED for score-tracking  ***/
      if ((PORT_EGEN &0xff) == 0xff ){
        PORT_EGEN &= ~0xff;
      }
      PORT_EGEN += 1;
    }
  }
  return;
}

int getsw( void ){
	return (int) ((PORTD & 0xf00) >> 8); //Get value of switches
}

int getbtns( void ) {
	return (int) ((PORTD & 0xe0) >> 5); //Get value of btns
}

//makes the bird move up the screen
void bird_jump(){
  if (jumpedrecently == 0)
  birdY -= 4;
  jumpedrecently = 1;
}

void add_pixel(int x, int y){
  //check if the coordinates  fits in screen range.
  if(x <= 0 || x >= SCREEN_WIDTH || y <= 0 || y >= SCREEN_HEIGHT)
    return;

  //(y/8)*128 chooses the page 0-3
  // + x chooses the x coordinate
  // the shift amount chooses the y coordinate in the page
  display[y/8 * 128 + x] |= (1 << (y-(y/8)*8));
}

void draw_bird(){
  int i,j;
  for(i = -1; i < BIRD_WIDTH - 1 ; i++)
    for(j = -1; j < BIRD_HEIGHT - 1 ; j++)
      add_pixel((birdX + i), (birdY + j));
}

void draw_pipe1(){
  int i,j;
  for(i = 0; i < PIPE_WIDTH ; i++)
    for(j = 32; j > (32-randomPipe1_bot); j--)
      add_pixel(pipe1X - i, j); //Ritar från höger till vänster

    for(i = 0; i < PIPE_WIDTH ; i++)
      for(j = 0; j < randomPipe1_top; j++)
        add_pixel(pipe1X - i, j);
}

void draw_pipe2(){
  int i,j;
  for(i = 0; i < PIPE_WIDTH ; i++)
    for(j = 32; j > (32-randomPipe2_bot); j--)
      add_pixel(pipe2X - i, j);

    for(i = 0; i < PIPE_WIDTH ; i++)
      for(j = 0; j < randomPipe2_top; j++)
        add_pixel(pipe2X - i, j);
}

void draw_floor(){
  int i;
  for (i = 0; i <=SCREEN_WIDTH ; i++){
    add_pixel(i, SCREEN_HEIGHT - 1);
  }
}


void menu(){
  //Show start screen
  display_string(0, "--FLAPPY BIRD--");
	display_string(2, "btn3 to start!");
  display_string(3, "btn4 to jump");
	display_update();

  //infinite loop that waits for the player to start
  //the game with btn3
  while(1){
  if(getbtns() & 0x2)
    return;
  }
}

void play(){

  /******** RESET DISPLAY ********/
  clear_display(display);
  render_display(display);

  /******** RESET GAME VALUES ********/
  timerstart();
  jumpedrecently = 0;

  birdX = SCREEN_WIDTH/3;
  birdY = SCREEN_HEIGHT/4;

  pipe1X = SCREEN_WIDTH;
  pipe2X = SCREEN_WIDTH + (SCREEN_WIDTH/2);
  pipeMinDist = 10;

  score = 0;
  collided = 0;
  PORT_EGEN = 0;

  randomPipe2_bot = 4 + (fast_rand() % 11);
  randomPipe1_bot = 4 + (fast_rand() % 11);

  randomPipe2_top = 4 + (fast_rand() % 11);
  randomPipe1_top = 4 + (fast_rand() % 11);

  delay(20);

  gamePlaying = 1;

/******** MAIN GAME LOOP ********/
  while(1){
    clear_display(display);
    draw_bird();
    draw_pipe1();
    draw_pipe2();
    draw_floor();
    render_display(display);
    if(getbtns() & 0x4)
      bird_jump();
    if (collided == 1){
      gamePlaying = 0;
      break;
    }
  }
}

void gameOver(){

  //Show gameover screen
  clear_display(display);
	display_string(0, "----GAMEOVER----");
  display_string(1, "btn2 to go back");
  display_string(2, "Your score was:");
  display_string(3, itoaconv(score));
	display_update();

  //wait for the player to go back to the menu with btn3
  while(1){
    if(getbtns() & 0x1)
      return;
  }
}

int main(){
  config();

  //infinite loop to run the program
  while(1){
    menu(); //Menu
    play(); //Start the game
    gameOver(); //Gameover, go to menu
  }

  return 0;
}
