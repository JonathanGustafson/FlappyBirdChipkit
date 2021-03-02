/*****************************************************
Lite test för hur vi skulle använda github och
hur strukturen för spellogiken kan se ut
*****************************************************/
<#include <stdio.h>

void play();
void gameOver();
void menu();

int main(){

  while(1){

    //Meny
    menu();

    //Spela spelet
    play();

    //Gameover, gå till menyn
    gameOver();

  }

  return 0;
}
