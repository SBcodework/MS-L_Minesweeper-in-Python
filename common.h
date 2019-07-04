/**
BSD 2-Clause License

Copyright (c) 2019, SBcodework
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**/

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED
#include "Parameter.h"
#include "Gridtype.h"
#include <vector>

//typedef std::__cxx11::string str;
typedef std::string str;

int extractDigitInt( int digits, std::string input );

void askXYMinesLoop(Parameter& out_param);  // Gets a valid inquiry into the length, width, and mines. Outputs to Parameter object.

void dispEmpty(Parameter& in_param);   // Displays an empty grid.

int askStartXYLoop(Parameter& out_param);  // Gets a valid inquiry into the start coordinates. Outputs to Parameter object.

int h_extractCoords(std::string in_line, int& out_x, int& out_y, char* out_sep = nullptr, char sep = '\0' );     // Helper function to extract coords from a string like "31#5", where '#' is the separator.
                                                                          // Returns 1 if error, 0 if not.
                                                                          /// If sep is null, the function scans for a separator. Outsep, when not a nullptr,
                                                                          /// outputs the non-digit separator that is found.

void gengrid(Gridtype& out_grid);   // Generates a grid using a  Gridtype object, which is the output. Parameter modifying: ?
                                                      // The MAX SIZE of a grid in either x or y dimension shall be 99 (two digits) in each.

void getSquare(int centerX, int centerY, int w, int h, int out_square[9]);  // Fills out_square with valid indexes of a center point. If a point is invalid, it is set
                                                                            // to -1.

int toIndexCheck(int x, int y, int w, int h);  // Checks if the point is out of bounds. If so, it returns -1. Else, it converts the arguments to the index.
                                              // Example input/output:

int outOfBounds(int x, int y, int w, int h);  // Out-of-bounds X and Y checking.
                                             /// Remember: Stick to X and Y indexing when using this. If not, there is a risk of
                                             /** accessing the element "E" in the example:

                                             Width: 3 , Height: 3
                                             X: 0    1    2     Y:
                                             [ '0', '0', '0',     0
                                               '0', '0', 'E',     1
                                               'O', '0', '0' ]    2

                                             when you intended to check, in terms of X and Y, if a coordinate of (3,1) is invalid,
                                             leading the function to return 'E' when is should have returned -1.
                                             This might happen when converting an index to X and Y, which looses precision (an entire dimension), leading to the above.
                                             If you convert (3,1) to an Index, you get 5, and by converting 5 to X and Y you get (2,1).
                                             **/

int uniqueRand(int size, int start, int numlimit, int* output); /// Returns to output[size] an array of unique ints of size "size".
                                                                /// Start is inclusive, numlimit is exclusive.

int allCheck(int start, int in_middle, int in_end, std::vector<bool>& in_vector);

//void disp(Gridtype in_grid);   //UNEEDED Displays the grid. A Parameter object is not needed because it can be obtained by the grid.

void askSelectionXYTLoop(Cordtype& out_cord);    // Gets a valid inquiry into the X cord, Y cord, and the type of action.
                                           // Outputs to Cordtype, including the type of action. The grid is used for verification.

//int endGame(Gridtype in_grid, Cordtype in_cord);    // Enters procedures to end the game. Asks the player if they want to play a new game,
                               // outputs 1 if so, 0 otherwise.

#endif // COMMON_H_INCLUDED
// Grid: param reference, .raw
