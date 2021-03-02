/*****************************************************
Lite test för hur vi skulle använda github och
hur strukturen för spellogiken kan se ut
-----------------------------------------------------
Struktur skriven av Jonathan Gustafson
*****************************************************/
<#include <stdio.h>

void menu();
void play();
void gameOver();

void menu(){
  while(1){

  //Show start screen

  //If(start_btn) -> return;

  }
}

void play(){

  //Draw the tubes and the bird

  //Countdown 3, 2, 1, GO

  while(1){
    //Move tubes to the left, keep spawning new tubes with random lengths

    //if(jump_btn) -> jump();

    //if (bird_cordinates == tube_cordinates) -> return;
  }
}

void gameOver(){
  while(1){

  //Show gameover screen

  //If(start_btn) -> return;

  }
}

int main(){

  //infinite loop to run the program
  while(1){

    //Menu
    menu();

    //Start the game
    play();

    //Gameover, go to menu
    gameOver();

  }

  return 0;
}
