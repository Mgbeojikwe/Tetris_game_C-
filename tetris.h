#include "shape.h"
#include <iostream>
#include <thread>
#include <mutex>

using std::array;
using std::transform;
using std::find;

int x{900};


static const char*  text1 = " GAME OVER";
static const char*  text2 = "press any key to restart game";


std::mutex  mut_obj;



#pragma once
class Tetris {

    private:
      Shape   tetromino;

      vector<vector<int>>   board_replica;
      vector<int>  board_replicaX;
      vector<int> board_replicaY;
      
      vector<Vector2> tetromino_XY;
      Color tetromino_color;

      char direction;
      bool game_over;
      
      
      vector<Vector2> all_obstructing_tetrominos_parts;
      vector<Color> all_obstructing_tetrominos_part_colors;
      
      
      bool a_new_tetromino_generated = false;
      
      int score;
      

      int max_X_length;
      int max_Y_length;

    public:



      Tetris()
        {

             max_X_length =   game_width/PIXEL_SIZE;
             max_Y_length = game_heigth/ PIXEL_SIZE;


       
      vector<int>  board_replica_row(max_X_length);  // number of elements in the row i.e number of column = max_X_axis


       auto fill_board_replica_axes = [](int size, vector<int> &container){

                int j{0};

                for( size_t i{0}; i<size; i++){

                      container.push_back(j);
                      j+=PIXEL_SIZE;    

                }

                container.shrink_to_fit();


        }    ;

       
       fill_board_replica_axes(max_X_length, board_replicaX);
       fill_board_replica_axes(max_Y_length, board_replicaY);

        
        vector<vector<int> >   var(max_Y_length, vector<int>(max_X_length,0));
        board_replica=var;
        board_replica.shrink_to_fit();

        auto[tetro_XY, tetro_color] = tetromino.create_shape();
        tetromino_XY = tetro_XY;
        tetromino_color= tetro_color;
        game_over = false;

         score = 0;
          
        }



            
      void  generate_new_tetromino(){
            
            /*
                  This method generates new tetromino. A tetromino is generated only when "game_over" == false
                  This is import to ensure that when game ends i.e "game_over" == true, no new tetromino is generated
                  thus ensuring that the  else if block in "keyboard_press()" method operates accurate logically
            */

        if (game_over == false){
        
             auto[tetro_XY, tetro_color] =  tetromino.create_shape();
        
            tetromino_XY = tetro_XY;
            tetromino_color = tetro_color;
        }

      }

      
      void  rotate_tetromino() {

            /*
                Here we choose a centre of rotation and followed by th steps:

                step 1: Substract the centre from the (x,y)-axis of all parts
                step 2: Rotate each resulting (x,y)  by 90 degree; this is obtained by interchanging (x,y) i.e (x,y) =  (y,x)
                step 3: add the centre to the resulting (x,y) points
            */
            Vector2  centre =  tetromino_XY[0];   // the first element in the container is made the centre of the rotating body
            vector<Vector2>  var{};

            var = tetromino_XY;

            transform(var.begin(), var.end(), var.begin(),[centre]( Vector2 part){

                  return Vector2{part.x - centre.x, part.y - centre.y};
            });
            
            transform( var.begin(), var.end(), var.begin(), []( Vector2 part){
                  return Vector2{part.y, part.x};
            });
            
            transform( var.begin(), var.end(), var.begin(), [centre]( Vector2 part){

                              return  Vector2{part.x + centre.x, part.y + centre.y};
            });



            tetromino_XY = var;


        }



     void alter_tetromino_position_L_R_or_D ( Vector2 direction2) {

      
        transform( tetromino_XY.begin(), tetromino_XY.end(), tetromino_XY.begin(), [direction2]( Vector2 part_XY){

            return Vector2{part_XY.x + direction2.x , part_XY.y + direction2.y};
        });

        
      }


      std::tuple<int, int>  get_tetromino_part_row_and_column_indexes( Vector2 tetromino_part){

            int row_index{}, column_index{};

                      int partX = tetromino_part.x;
                      int partY = tetromino_part.y;

                      auto get_axes_value_index_in_board_replica = [](int axes_value, vector<int> board_replica_axes){

                                    int index{};

                           auto it = std::find(board_replica_axes.begin(), board_replica_axes.end(), axes_value);
                      
                           if ( it != board_replica_axes.end())   // i.e if found
                                        index =  it - board_replica_axes.begin();

                                        return index;
                      };

                 
                  column_index = get_axes_value_index_in_board_replica(partX, board_replicaX);
                  row_index = get_axes_value_index_in_board_replica(partY, board_replicaY);

                      return {row_index, column_index};
      
      }

      void  label_board_replica ( ){
            /*
            This method labels the tetromino on the board replica. Board replica is only labeled 
            when the tetromino reaches the bottom or is ontop another tetromino.
            We start by getting the indexex of the (x,y)-component of each tetromino part, then label board replica at that index
            */

            for(auto part_XY: tetromino_XY){

              
              auto [row_index, column_index]= get_tetromino_part_row_and_column_indexes(part_XY);
              
              board_replica[row_index][column_index] = 1;
            

            }
      
      
      }

       
       std::tuple<int,int> place_boundary_condition( ){


                  // get the tipY, leastX  and fartherstX of the tetromino
               auto tipY =  std::max_element( tetromino_XY.begin(), tetromino_XY.end(), [] ( Vector2 part1 , Vector2 part2) { return part1.y < part2.y;});
              auto largest_partX = std::max_element ( tetromino_XY. begin(), tetromino_XY.end(),[]( Vector2 part1,Vector2 part2){ return part1.x < part2.x;});
               auto least_partX  = std::min_element ( tetromino_XY.begin(), tetromino_XY.end(), [](Vector2  part1, Vector2 part2){ return part1.x < part2.x ; });
           
           switch (direction) {

                case 'D':
                      return {(*tipY).y,0};
                      break;
                case 'L':
                      
                      return {(*least_partX).x,0};
                      break;
                case  'R':
                      
                      return {(*largest_partX).x,0};
                      break;
                 
                 case 'U':
                        return {(*least_partX).x, (*largest_partX).x};
           }

          
        }

            void create_container_of_obstructing_part(){
                  /*
                  This method appends all obstructing parts. Such container is salient in enabling us
                  print obstructing tetrominos to the canvas, and also row completely filled row from canvas
                  */
                  
                  for (auto part: tetromino_XY){

                        all_obstructing_tetrominos_parts.push_back(part);
                  }
                  
                  // since the container is updated by four parts that have same color, we also 
                  // need to update the color container four times with same color.
                  for ( size_t i{0}; i<4 ; i++)
                                    all_obstructing_tetrominos_part_colors.push_back(tetromino_color);

                  
            }



      void   move_tetromino()  {
          
          
          Vector2  direction2D{};
          int foremost_partY{}, smallest_partX{}, far_right_partX{};

          auto temporary_XY = tetromino_XY;   


            if (direction == 'D'){

              direction2D = {0, PIXEL_SIZE};
              
              alter_tetromino_position_L_R_or_D(direction2D);

              auto[foremost_partY, var] = place_boundary_condition();
              
              if ( (foremost_partY  > (game_heigth- 2*PIXEL_SIZE ) ) || tetromino_on_top_another_tetromino() == true  ) {    // it tetromino reaches game bottom
              
                        tetromino_XY = temporary_XY;
                        label_board_replica();        // label board replica
                        
                        // the resulting tetromino is now an "obstructing tetromino", and needs to
                        // be kept in "all_obstructing_tetrominos_parts". This is done prior generatign a new
                        //tetromino to enable us utilize the obstrucing tetromino. The tetromino color is appended
                        // four times to "all_obstructing_tetrominos_part_colors" since the parts in a tetromino
                        // have same color
                        
                        create_container_of_obstructing_part();
                              
                        //now we generate a new tetromino
                        generate_new_tetromino();     // generate new tetromino
                        a_new_tetromino_generated = true;
              }

            
              else 
                        a_new_tetromino_generated = false;
            }
            

            else if (direction == 'L'){

            
                 direction2D = {-PIXEL_SIZE,0};

                 alter_tetromino_position_L_R_or_D(direction2D);
                 
                auto[smallest_partX, var] = place_boundary_condition();
            
                  // if (smallest_partX < (-PIXEL_SIZE)  ||  tetromino_touches_another_by_the_side( )== true)   // placing boundary condition at the left
            
                  if (smallest_partX  < 0  ||  tetromino_touches_another_by_the_side( )== true)   // placing boundary condition at the left

                        tetromino_XY =  temporary_XY;

            
            }

            else if (direction == 'R') {

              direction2D = {PIXEL_SIZE,0};

                 alter_tetromino_position_L_R_or_D(direction2D);

                 auto[far_right_partX, var] = place_boundary_condition();

                if (far_right_partX > (game_width - PIXEL_SIZE)  ||  tetromino_touches_another_by_the_side( )== true)  // placing boundary condition at the right side of game board
             
                       tetromino_XY = temporary_XY; 
              
            }
            
            else if (direction == 'U') {
            
                  rotate_tetromino();

                  // AFter rotating we need to palce boundary condtion to ensure tetromino
                  // doesn't leave the canvas
                  auto[far_leftX_after_rotation, far_right_partX_rotation] = place_boundary_condition();
                  
                  if ( far_leftX_after_rotation < 0  || far_right_partX_rotation > (game_heigth -PIXEL_SIZE))

                           tetromino_XY =   temporary_XY;
            
            }

          direction='D';  // returning to default value so that the tetromino always moves downwards
          
          }



  bool      tetromino_on_top_another_tetromino() {

    /*
          This method checks if the tetromino is on top another,and is acheived by checking if the current
          position of the tetromino ,in the board replica, has already been occupied. If true, then the 
          tetromino is returned to it previous (penultimate) position with the help of the "move_tetromino()" method.

          N:B: This collision detection method is utilized only when "direction == 'D' ", thus is perfect, in placing tetro
          minos on-top one another. Since we utilized it only for when "direction ==  'D' ", thus not suitable for the 
          detection of side-collision.
    
    */

            for( auto part: tetromino_XY){

                  auto[row_index, column_index]  =  get_tetromino_part_row_and_column_indexes(part);
            
                  if ( board_replica[row_index][column_index] == 1)
                            return true;
                  
                  else 
                        continue;    // check other parts of the tetromino

            }

            return false ;            // if current position of all tetromino's parts aren't filled
                                          // return false

        }



       bool  tetromino_touches_another_by_the_side( ){

              /*
              
              This method checks if one of the parts touches an obstructing's tetromino part.
                This property is acheived by checking if the current position, resulting from left/right movement, of a given part is already occupied.
                if true, then , with help of the "move_tetromino()" method, the entire teromino is returned to its
                previous position. 

                Thus this method is called after movement of the tetromino.
              
              */           
                    // direction is netiher 'L'  nor 'R'

                      vector<int>  partsX{};

                for( auto part: tetromino_XY)
                       partsX.push_back( part.x);
                
                auto[min_X,max_X] = std::minmax_element(partsX.begin(), partsX.end());  //get the two tetromino parts that are
                                                                                    // leftmost and rightmost on the canvas

                if (direction == 'L'){
                  
                  //on getting this minimum, we have to find its y-axis.

                  for( auto part: tetromino_XY){

                      if ( part.x ==  *min_X){
                           
                             auto [row_index, column_index] = get_tetromino_part_row_and_column_indexes(part);
                             
                             if (board_replica[row_index][column_index]==1)
                                    return true;
                      
                      }
                      else  
                          continue; 

                  }

                  return false;  // return false if tetromino is not touching another by the left
                    
                }
              

              else if (direction == 'R'){   


                for (auto part: tetromino_XY){
                    
                    if (part.x == *max_X){
                           auto [row_index, column_index] = get_tetromino_part_row_and_column_indexes(part);
                           
                           if (board_replica[row_index][column_index] == 1)
                                    return true;
                            else 
                                continue;
                    }
                }

              }

        }
        
        

     void   draw_tetromino_on_cavas() {

      for (auto part: tetromino_XY){

             int x = part.x;
             int y = part.y;

            DrawRectangle(x+(PIXEL_SIZE), y+(PIXEL_SIZE), (PIXEL_SIZE-1),(PIXEL_SIZE-1), tetromino_color);
            
      }

      
         }


         void   popout_parts_of_completely_filled_rows( vector<int> y_values_of_filled_rows){


                  for (size_t index{0}; index < all_obstructing_tetrominos_parts.size(); index ++){

                       Vector2  part=  all_obstructing_tetrominos_parts[index];

                       int y = part.y;

                       auto it = find(y_values_of_filled_rows.begin(), y_values_of_filled_rows.end(), y);

                        if (it != y_values_of_filled_rows.end()) {  // if y-axis value of part is found in container 

                                    all_obstructing_tetrominos_parts.erase( index + all_obstructing_tetrominos_parts.begin());
                                    all_obstructing_tetrominos_part_colors.erase(index + all_obstructing_tetrominos_part_colors.begin());
                        }

            }
         }



         void    move_left_over_parts_forward( int y_value_of_the_last_completely_filled_row){
           

            /*
                  This method moves left-over parts above the completely-filled row(s) forward. This selective movement is important
                  to ensure that only parts higher than the cleared row are moved; parts below that remove maintain their positions
            */

            for (size_t index{0}; index < all_obstructing_tetrominos_parts.size(); index ++){

                 Vector2 part = all_obstructing_tetrominos_parts[index];
            
                 int y  = part.y;

                 if (y < y_value_of_the_last_completely_filled_row)
                          all_obstructing_tetrominos_parts[index] = Vector2{part.x, part.y + PIXEL_SIZE};

                 else 
                       continue;
                        
            }

            }



    void remove_a_completely_filled_row_from_canvas(){
      
      /*
      This method clears a completely filled row from the canvas via the following steps:

            1) loop through "board_replica" from bottom up to find rows whose elements are all 1.
            2) For each row found, 
                  2a) we obtain its row-index and the value of its index-wise y-component 
                        in "board_replicaY".
                  2b) we search for that y-component in "all_obstructing_tetromino_parts", if found we delete
                        affected part from "all_obstructing_tetromino_parts"container
                  
            3) we replace the affected row with a higher placed row whose elements are not all 1.
            4) We all parts in the resulting "all_obstructing_tetromino_parts " forward by "PIXEL_SIZE"
            5)we then print all parts in the resulting "all_obstructing_tetromino_parts" to the canvas

      */

            vector<int> y_values_of_affected_rows{};

             size_t last_index = board_replica.size()-1;

         
            for (size_t index{last_index}; index >0  ; index -- ){
                     
                     auto row = board_replica[index];
                      
                      bool  row_is_filled_completely = std::all_of ( row.begin(), row.end(),[](int x){
                              return x ==1;
                        });

                        vector<int> row_value = board_replica[index];

                        if (row_is_filled_completely == true){

                                    score+=10;                          //increase score by 10


                                    int affected_y_axis_value = board_replicaY[index];  // getting the y-value of the affected row
                                    y_values_of_affected_rows.push_back(affected_y_axis_value);

                                    // move all the higher rows downwards
                                    for (size_t j{index}; j > 0 ; j --){

                                                board_replica[j] = board_replica[j-1];  //assigning the row at lower index (i.e higher placed row)
                                                                                          // to that of higher index (i.e lower placed row)                                             
                                    }

                        }


           }


           if (y_values_of_affected_rows.size() >= 1) {    // if there exit completely filled rows; doing this optimize the spped of the code
           
                   auto it =  std::max(y_values_of_affected_rows.begin(), y_values_of_affected_rows.end());
           
                  int y_value_of_last_completely_filled_row = *it;


                  // remove the parts that occupying the completely filled rows

                  popout_parts_of_completely_filled_rows(y_values_of_affected_rows);
                  move_left_over_parts_forward(y_value_of_last_completely_filled_row);
           
                  
                  
           }


    }


      
    void  clear_obstructing_tetrominos_from_canvas(){
      /*
            This method clears "all_obstructing_tetrominos_parts" and "all_obstructing_tetrominos_part_colors" 
            hence implicitly clears from canvas, since there will longer be any obstructing tetromino to print 
            on canvas
      */

                  all_obstructing_tetrominos_parts.clear();
                  all_obstructing_tetrominos_part_colors.clear();
      }

   void reset_game(){
            

            for (int row{0}; row < max_Y_length; row++){
                        for( int column{0}; column < max_X_length; column++){
                                    
                                   if (board_replica[row][column]== 1) { 
                                          board_replica[row][column]= 0;
                  
                        }
                                    else 
                                          continue;
                        }     
            }
            
            clear_obstructing_tetrominos_from_canvas();

            generate_new_tetromino();
            score=0;
    }


   void keyboard_press(){

      /*
            This method connects the keyboard to the game. it also prints out text if game ends.
            The logic in printing out text is to first check if "game_over" == true and a key is press; if false
            checks if "game_over" is true only, if false, then runs game normally
      */

                 int keypress = GetKeyPressed();


            if ( game_over ==  true &&  keypress !=0 ){
                  
                         game_over = false;
                         reset_game();
                  }

               else  if (game_over == true){
                       
                        
                        print_spiral_shape_in_the_left_and_right_side();
                        std::thread thd{& Tetris::print_all_obstructing_tetromino_to_canvas, this};
                        thd.join();

                        DrawText(text1,game_width +(3*PIXEL_SIZE), game_heigth/2,30,RED);
                        DrawText(text2, 2*PIXEL_SIZE,(3*PIXEL_SIZE),30, WHITE);

                 }

                  
            else{
                  switch (keypress){

                        case KEY_LEFT:
                              direction = 'L';
                              break;
                        
                        case KEY_RIGHT:
                                    direction = 'R';
                                    break;
                        case KEY_UP:
                              direction = 'U';
                              break;
                        
                        case KEY_DOWN:
                             direction = 'D';
                             break;
                  }
                  
            
            }

    }


    void print_all_obstructing_tetromino_to_canvas() {

                  std::unique_lock  uni_obj{mut_obj};

                        for(size_t i{0}; i < all_obstructing_tetrominos_parts.size(); i++){

                                   Vector2 part = all_obstructing_tetrominos_parts[i];
                        
                                   int x = part.x;
                                   int y = part.y;

                                   if ( y > game_heigth -2*PIXEL_SIZE){
                                                all_obstructing_tetrominos_parts.erase(i + all_obstructing_tetrominos_parts.begin());
                                                all_obstructing_tetrominos_part_colors.erase(i+ all_obstructing_tetrominos_part_colors.begin());
                                   }

                                   else{
                                   Color color =all_obstructing_tetrominos_part_colors[i];

                                  DrawRectangle(x+(PIXEL_SIZE-1), y+(PIXEL_SIZE-1), (PIXEL_SIZE-1), (PIXEL_SIZE-1), color);
                        }
                        
                        }

    }

    void print_spiral_shape_in_the_left_and_right_side(){

            for (auto y: board_replicaY){
                  DrawCircleLines(0,y,PIXEL_SIZE,Color{200,122,255,250});
                  DrawCircleLines(game_width+(2*PIXEL_SIZE),y,PIXEL_SIZE,Color{200,122,255,250});
            }
            
    }

   void    Game_over (){
       
            /*
            We confirm that a game is over, if the first row is "board_replica" has one/more 1.
            We acheived by search for the presence of 1 in the first row
            */
      
                     vector<int> first_row =   board_replica[0];

                  bool top_row_is_filled =   std::any_of( first_row.begin(), first_row.end(),[]( int element){

                        return element == 1;
                     });

                     if (top_row_is_filled == true){
                                   game_over = true;
                              
                     }
                     else 
                                   game_over = false;                          
                                 
      }

  
  int get_score(){
      return score;
  }
  
 
  
  void play(){

      /*
                  Here we parallized, via multithreading ,the "print_all_obstructing_tetromino_to_canvas()" method
                  because of the container size of "all_obstructing_tetrominos_parts".
      */
     
          if ( game_over == false)   { 
            move_tetromino();
          
           draw_tetromino_on_cavas();
           keyboard_press();

          std::thread thd{& Tetris::print_all_obstructing_tetromino_to_canvas, this};
           thd.join();
           remove_a_completely_filled_row_from_canvas();
           print_spiral_shape_in_the_left_and_right_side();
           Game_over();

          }
          
          else{

            keyboard_press();

          }

  }
   

    ~Tetris(){

    }

}; 