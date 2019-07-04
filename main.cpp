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

#include <iostream>
#include <typeinfo>
#include "common.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"

#include <string>

/// Progress: working game acheived. To-do: input-saftey and changing the mineCanditates buffer to be
/// based on the runtime area of the start square rather than a constant 9 on gengrid() on "common.cpp".
/// Next, refactor.

int main ( )
{
    int     endloop  = 1 ;
    for ( ; endloop == 1 ; )
    {
        Parameter        params   ;  // .X, .Y, .mines.
        askXYMinesLoop ( params ) ;  // returns to params
        dispEmpty      ( params ) ;
        askStartXYLoop ( params ) ;  // Output is params.
                                     // Above is clean, below to be implemented; remove unneeded params from gengrid(...)
        Gridtype grid  ( params ) ;  //.raw, .flags, .shown. Grid also keeps a reference to params.
        gengrid        ( grid   ) ;  // Output is grid argument
        int dbcounter    = 0      ;  ///debug

        for ( int i = 0 ; i < params.area ; i++ )
        {
                                                       //std::cout << grid.raw[i] << ",";
            if ( grid.raw[i] == 'X' )
            {
                 dbcounter++ ;
            }
        }

        std::cout << '\n' ;
        std::cout << "dbcounter, mines: " << dbcounter << " " << params.mines << '\n' ;

        Cordtype selection ( params ) ;
        for ( int endGame = 0 ; endGame == 0 ; )
        {
            grid.disp ( ) ;
            askSelectionXYTLoop    ( selection ) ; // Function also gives type modifier to selection. Output is selection.
            endGame = grid.action  ( selection ) ;
            if ( endGame )
            {
                grid.dispEnd ( selection ) ; // Disp here inside function
                break ;
            }
        }
        grid.free ( ) ;
        selection.free_static( ) ;

        std::cout << "\nContinue? 1 if so, 0 otherwise. ";
        std::cin >> endloop;
    }
    return 0 ;  /// Free memory!
}




