#include <iostream>
#include <vector>
#include <raylib.h>
#include <algorithm>
#include <random>
#include <tuple>

using std::vector;
using std::string;
using std::random_shuffle;


const int PIXEL_SIZE{40}, game_width{PIXEL_SIZE*20}, game_heigth{PIXEL_SIZE*20}; // game height and width are made factors of
                                                                                    // Pixel size, so that them game remains dynamic


#pragma once
class Shape{

    private:

        
        vector<int> possible_start_pointX;
        vector<char> shapes;
        vector< Color> colors;

    public:
    

        Shape()
            {
            
               shapes= {'L','T','B','I','N'};
               colors = {Color{200,122,255,250},Color{0,82,172,250}, Color{0,117,44,250}, Color{255,203,0,250}} ;

                possible_start_pointX = {PIXEL_SIZE,  7*PIXEL_SIZE, 15*PIXEL_SIZE};
                possible_start_pointX.shrink_to_fit();
                
        }


    std::tuple<vector<Vector2>, Color>    create_shape(){

            vector<Vector2>  tetromino_position{};


            if ( shapes.size() == 0)  // if all shapes have been selected

              shapes = {'L','T','B','I','N'};

            if (colors.size() == 0)
                     colors = {Color{200,122,255,250},Color{0,82,172,250}, Color{0,117,44,250}, Color{255,203,0,250}};

            random_shuffle(shapes.begin(), shapes.end());
            auto shape =shapes[0];
            shapes.erase( shapes.begin()+0);    // ensuring each block appears once


            random_shuffle(colors.begin(), colors.end());
            auto color = colors[0];
            colors.erase( colors.begin()+0);   // ensuring a single color is chosen once
            
            random_shuffle(possible_start_pointX.begin(), possible_start_pointX.end());
                        
            auto x_i= possible_start_pointX.begin();
            float x= *x_i;
            float y=0;

            switch (shape){

                case 'L':
                    tetromino_position = {Vector2{x,y}, Vector2{x,(y+PIXEL_SIZE)}, Vector2{(x+PIXEL_SIZE),(y+PIXEL_SIZE)}, Vector2{(x+2*PIXEL_SIZE),(y+PIXEL_SIZE)}};
                    break;

                case 'T':

                    tetromino_position={Vector2{x,(y+PIXEL_SIZE)}, Vector2{(x+PIXEL_SIZE),y}, Vector2{(x+PIXEL_SIZE),(y+PIXEL_SIZE)}, Vector2{(x+2*PIXEL_SIZE),(y+PIXEL_SIZE)}};
                    break;

                case 'B':
                    tetromino_position = {Vector2{x,y}, Vector2{(x+PIXEL_SIZE),y}, Vector2{x,(y+PIXEL_SIZE)}, Vector2{(x+PIXEL_SIZE),(y+PIXEL_SIZE)}};
                    break;

                case 'I':
                    tetromino_position = {Vector2{x,y}, Vector2{(x+PIXEL_SIZE),y}, Vector2{(x+2*PIXEL_SIZE),y}, Vector2{(x+3*PIXEL_SIZE),y}};
                    break;

                 case 'N':
                       tetromino_position = {Vector2{x,y}, Vector2{x,(y+PIXEL_SIZE)}, Vector2{(x+PIXEL_SIZE),(y+PIXEL_SIZE)}, Vector2{(x+PIXEL_SIZE),(y+2*PIXEL_SIZE)}};
                        break;

           
            }

            return {tetromino_position,color};




    }


~Shape(){

}


};