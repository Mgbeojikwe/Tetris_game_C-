


To run this code following the steps:

step 1:  run $bash build.bash

step 2 : run  $./a.out

A visual sample of this game is found in the url https://github.com/Mgbeojikwe/Tetris_game_C-/blob/main/tetris_c%2B%2B-2024-02-05_01.04.23.mp4 download the mp4 video by clicking on "View raw"

.............Scope of source code............. This was designed by creating two classes "Shape" and "Tetris" written in shape.h and tetris.h files respectively. The classes are connected by the "composition" interaction route in which object of "Shape" class is created and made an instance attribute in the "Tetris" class.

1.0 Shape class

This class has only one method viz: create_shape() 

1.1 create_shape() This method creates the tetromino. The tetrominos are kept in a std::vector container of char, and are randomly selected using the random_shuffle() function. The selected tetromino is kept assigned to "shape" variable, and the respective tetromino and color is returned

2.0 Tetris Class This class has 21 methods with some discussed below:

2.1 play() This method is used in housing all other methods. this method was written so that when called by the "Tetris" object, all other methods are called automatically. Thus increasing the cleaniness of the source code.

2.2 Game_over() This method check if the first row is occupied. The first row is occupied if the first row in "board_replica" has one or more label '1', and if true the game ends.

2.3 print_spiral_shape_in_left_and_right_side() this prints guard wires on the game canvas, hence separating the score board from the actual game window

2.4 print_all_obstructing_tetromino_to_canvas() This method prints obstructing tetrominos to the canvas. The tetrominos which have been separated into parts in "all_obstructing_tetromios_parts", and parts still available after clearing a filled row are printed to the canvas.

2.5 keyboard_press()

This method connect the game to the keyboard. It also prints certain texts to the canvas if "game_over" equals true and a key pressed, if only "game_over" equals true, and if "game_over" equals false.

2.6 reset_game() This method ensures that the entire game is returned to start state when ever the player wants to replay the game. This is acheived by returning "board_replica" to a null 2D-vector, clearing off "all_obstructing_tetromino" from canvas, and also generate a new tetromomno.

2.7 clear_obstructing_tetromino_from_canvas() This method returns the std:: vector containers: "all_obstructing_tetrominos_parts" and "all_obstructing_tetrominos_parts_colors" to null state.

2.9 remove_an_entire_row_from_canvas() this method helps in clearing-off completely filled rows from the canvas. It acheives this objective using the following steps: step1 => loop through the entire 2D vector "board_replica" searching for row "i" that has all its elements as 1. step 2 => If such row is found, increase the value of score by 10, run the "pop_out_affected_parts()" method. The "pop_out_affected_parts()" method removes parts whose y-axis value equals the y-axis value designated for the completely filled row step 3 => starting from the completely-filled-row "i", assign lower index rows in "board_replica" to next higher subsequent row. This is indirectly means we have removed the completely filled row from "board_replica" step 4 => Since we have pop-out affected parts as par step 2, move the lefrt over parts forward by "PIXEL_SIZE" by running the "move_left_over_parts_forward()" method step 5 => move to the next iter value i.e next row, in "board_replica" repeating step 1 to 4.

2.10 move_left_over_parts_forward()

This method requires an argument : "higher_row_index", and it moves the available parts in the std::vector container: "all_obstructing_tetrominos_part", forward by "PIXEL_SIZE" step. This objective is acheived through the steps:

step 1 => utilizing "higher_row_index" and from "board_replicaY" get the y-axis of an the incompletely filled row.
step 2 => Loop through the std::vector container  "all_obstructing_tetrominos_part". Each either value is called a "part" 
step 3 => check if y-component of "part" equals the results obtained from step1
step 4 => If step3 equals true, move the part forward by increasing its y-component by 'PIXEL_SIZE", but maintaining the value of its x-component.
step 5 => move to the next iter value and repeat step 2 to 4.

2.11 pop_out_affected_part()

This method removes tetromino parts whose y-component value equal the y-component value of the affected row. It receievs the arguement: "affected_part_Y_value" which is the y-axis value of the completely filled row that is to be removed from the canvas. This method acheives its objectives via the following steps:

step 1 => carry-out index-style for looping on the container  "all_obstructing_tetrominos_part" 

step 2 => utilizing the index obtained from step 1, get the tetromino part "i"  in  "all_obstructing_tetrominos_part"
step 3 => check if the y-component of the tetromino part from step 2 equals the arguement "affected_part_Y_value", if false append it to "parts" and also its color to "parts_color'
step 4 => update "all_obstructing_tetrominos_part" and  "all_obstructing_tetrominos_part_colors" by assigning "parts" and "parts_colors" to them respectively.

step 5 => move to the next iter value and repeat step 2 to 4

2.12 draw_tetromino_on_canvas() This method draws the moving tetromino on canvas.

2.13 tetromino_touches_another_by_the_side()

This method does collision detection, thus preventing tetromino from collapsing into one another when touched by the side. It operates after the tetromino has moved, and checks if that current position is already occupied.  It acheives its objectives via the steps:

step 1 => since a tetromino has squared parts, get the part that has lowest and largest x-axis value usign std::minmax_element()  STL algorithm
step 2 => get the (x,y)-index location of both parts using the "get_tetromino_part_row_and_column_index()"
step 3 => check if board_replica is already label at that position, if yes return "true".

2.14 tetromino_on_top_another_tetromino() This method is similar to that described in 2.13, except that it checks if any of the parts , rather than fartherst part, has its current position already occupied. It acheives its objective using the following steps: step1 => loop through the parts of the tetromino step 2 => get the (x,y)-index location of the tetromino on board replica step 3 => check if "board_replica" has a value of 1 at the indexes position obtained from step2, and return true if already occupied step 4 => move to the next part, and repeat step 2 and 3

2.15 move_tetromino()

This method  handles the movement of the tetromino. A tetromino can move left, right, down and also rotate based on the value of "direction". A tetromino is allowed to move downwards by default.

