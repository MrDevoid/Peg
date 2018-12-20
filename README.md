# Peg
A c++ implementation of the "Peg solitary" game. The goal of the game is to erase all the pieces except one which must end in one of the marked boxes


It only works on Windows because it includes a system library to show colors.

To start the game it will be necessary to load the board on which you will play, for this reason it is necessary to place the .txt files, in which the boards are described, on the same route as the executable file, or, put the file path when you are loading it

Here are the structure of the board files, in case of you want to create a new one:

    <Rows of the matrix>  <Columns of the matrix>
                <Matrix>
    <Row of the goal> <Column of the goal>
    <Number of moves>
    
For example:

    5 5
    0   0 0  0 0
    0   0 2  0 2
    0  10 0 10 2
    10  0 0 10 2
    10 10 2  0 0
    1 4
    6
Notes: The row and column goal start on 0. The meaning of the numbers in the matrix are:
>> - 0: invalid box 
>> - 2: empty box 
>> - 10: box with a piece 
