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

#ifndef GRIDTYPE_H
#define GRIDTYPE_H
#include <vector>
#include "Cordtype.h"
#include "Parameter.h"

class Gridtype /// "get state" functions to be implemented
{
    public:
        Parameter* pParams = nullptr;

        Gridtype(Parameter& in_param);
        virtual ~Gridtype();

        void initSet_grid(char* in_allocatedArray); // Sets .raw pointer to argument.
                                         /// Argument must be dynamically allocated (new keyword, etc).

        char get_state(Cordtype in_cords); // Returns the state at the coordinate.
        char get_state(int in_x, int in_y); // See above. Converts arguments to a temporary Cordtype object.
                                           // Then calls above function.
        char get_state(int in_index); // See above.

        void set_grid(char* chptr);

        void set_minelist(int* nptr);

        void disp();
        void dispEnd( Cordtype& lastPoint );
        void rawDisp();

        void free();

        int action( Cordtype& point );
        int action( int index, char mode );

        int singleClick( int index, int recursive = 0 );
        int flag( int index );
        int middleClick( int index );

        int checkWin();  /// 0, no win. 1, win.

        int flagNumber = 0 ;
        int hasWon     = 0 ;
        char* raw                  = nullptr ; // The grid itself.
        int*  mineList             = nullptr ;
        std::vector<bool> flags   ;
        std::vector<bool> visible ;
        //std::vector<bool> mineVector ; depreciated
};

#endif // GRIDTYPE_H
