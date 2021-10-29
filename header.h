/***************************************
FlappyBird header file

Written by Jonathan Gustafson
and Erik Gustavsson
***************************************/

// btn 4 jump
// btn 3 next
// def global stuff
 uint8_t spi_send_recv(uint8_t data);
 extern char textbuffer[4][16];
 extern const uint8_t const font[128*8];
 #define PORT_EGEN *(volatile unsigned*) 0xbf886110

 // from lab material (other authors)
 // displayfunc.c
 uint8_t spi_send_recv(uint8_t data);
 void quicksleep(int);
 void display_init(void); // initialise screen
 void display_string(int, char *s); // display string on row 0-3, str
 void display_image(int x, const uint8_t *data);
 void display_update(void); // necessary to update text strings
 char * itoaconv(int);

 //egna
 void clear_display();
//void render_display(uint8_t *array)

 // tmp btn test , led
 //config.c
 void user_isr( void );// empty funciton since Vectors.S needs them
 int getsw( void ); //
 int getbtns( void ); //

 void config(void); // run system, displayinint, constantly run run, most from lab

 //game functions
 int colision(void);
 void user_isr( void );// empty funciton since Vectors.S needs them
 void add_pixel(int, int);
 void menu(void);  // menue, display and get run play with btn 4
 void play(void);      // game
 void gameOver(void);
 void draw_floor(void); // draw the floor for the game 

 void play(void);      // game
 int main(void); // running program/ menue, play gameover
