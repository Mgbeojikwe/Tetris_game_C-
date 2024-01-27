#include <raylib.h>
#include "tetris.h"
#include <random>



int main(){

    srand(time(NULL));
    Color  background_color{0,0,0};

    int scoreX = game_width+(3*PIXEL_SIZE);
    int scoreY = PIXEL_SIZE;


InitWindow(game_width+(10*PIXEL_SIZE), game_heigth," tetris");

SetTargetFPS(7);


Tetris obj;

while( WindowShouldClose() == false){

    BeginDrawing();

    ClearBackground(background_color);
     
     obj.play();

     char buffer[10];
     sprintf(buffer,"score = %d", obj.get_score());
    
     DrawText(buffer, scoreX,scoreY,30, WHITE);
                              
    
    EndDrawing();
    
}

CloseWindow();

delete(text1);
delete(text2);     

return 0;

}