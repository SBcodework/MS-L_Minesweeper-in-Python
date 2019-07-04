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
#include <vector>
#include "common.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"

namespace VARS
{
    char dispUnknown  = ' ' ;
    char dispLeftBox  = '(' ;
    char dispRightBox = ')' ;
    char dispFlag     = '!' ;
    char dispZero     = '`' ;
}

Gridtype::Gridtype(Parameter& in_param)
{
    pParams = &in_param ;
    flags.assign( pParams->area  , 0 ) ;
    visible.assign( pParams->area, 0 ) ;
    //mineVector.assign( pParams.area, 0 ) ;  depreciated
}

Gridtype::~Gridtype()
{
    delete[] raw   ;
    raw = nullptr  ;
    delete[] mineList  ;
    mineList = nullptr ;
}

void Gridtype::initSet_grid(char* in_allocatedArray)
{
    raw = in_allocatedArray;
    return;
}

void Gridtype::set_grid(char* chptr)
{
    static int chosen = 1;
    if (chosen)
    {
        raw = chptr;
    }
    return;
}

void Gridtype::set_minelist(int* nptr)
{
    static int chosen = 1;
    if (chosen)
    {
        mineList = nptr;
    }
    return;
}

void Gridtype::disp()
{
    int length = pParams->length;
    int height = pParams->height;
    std::cout << "     ";  // 5 spaces
    for (int i=0; i<length; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << " }";
    }
    std::cout << "\n";

    char tempDispChar = '0';
    for (int i=0; i<height; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << "} ";
        for (int n=0, index=0; n<length; n++)
        {
            index = (i*length)+n;
            flags[index] ? (tempDispChar = VARS::dispFlag) :       // Display settings
                ( visible[index] ? (tempDispChar = raw[index]) : (tempDispChar = VARS::dispUnknown) );

            if ( tempDispChar == '0')  { tempDispChar = VARS::dispZero ; }
            std::cout << VARS::dispLeftBox << ' ' << tempDispChar << ' ' << VARS::dispRightBox ;

        }
        std::cout << "\n";
    }
    return;
}

void Gridtype::rawDisp()
{
    int length = pParams->length;
    int height = pParams->height;
    std::cout << "     ";  // 5 spaces
    for (int i=0; i<length; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << " }";
    }
    std::cout << "\n";
    for (int i=0; i<height; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << "} ";
        for (int n=0; n<length; n++)
        {
            std::cout << "[ " << raw[(i*length)+n] << " ]";
        }
        std::cout << "\n";
    }
    return;
}

void Gridtype::free()
{
    delete[] raw;
    raw = nullptr;
    delete[] mineList;
    mineList = nullptr;
}

int Gridtype::action( Cordtype& point )
{
    return action( point.get_index(), point.get_mode() ) ;
}


int Gridtype::action( int index, char mode )
{
    char DBstate = raw[index] ;  /// Db: once was int state
    int gameOver = 0  ;
    std::cout << "\n" ;
    switch( mode )
    {
    case 's':
        {
            gameOver = singleClick( index ) ;
            break ;
        }
    case 'm':
        {
            gameOver = middleClick( index ) ;
            break;
        }
    case 'f':
        {
            gameOver = flag( index ) ;
            std::cout << "Flags: " << flagNumber << "\n";
            break;
        }
    default:
        {
            std::cout << "Error from Gridtype.action()\n" ;
            return 2 ; // Internal Error
        }
    }

    if ( gameOver || checkWin() )
    {
        return 1 ;
    }

    return 0 ;
}

int Gridtype::singleClick( int index, int recursive )
{
    if ( (!recursive &&  flags[index]) || (visible[index]) )
    {
        return 0 ;
    }

    char state = raw[index] ;
    switch (state)
    {
    case 'X':
        {
            return 1 ; // Lost game
        }
    case '0':
        {
            visible[index] = true  ;
            flags[index]   = false ;

            int square[9] {0}     ;
            getSquare( (index % (pParams->length)), (index / (pParams->length)), pParams->length, pParams->height, square ) ;
            for ( int i = 0; i < 9; i++ )
            {
                int position = square[i] ;
                if ( (position != -1) &&  (!(visible[position])) && (position != index) )
                {
                    singleClick( square[i], 1 ) ;
                    flags[position] = false     ;
                }
            }
            return 0 ;
        }
    default:
        {
            visible[index] = true ;
            return 0 ;
        }
    }

    return 0;
}

int Gridtype::flag( int index )
{
    if ( visible[index] )
    {
        return 0 ;
    }
    bool state = flags[index]  ;
    state ? flagNumber-- : flagNumber++  ; // Change the flag counter
    flags[index] = !state ;

    return 0 ;
}

int Gridtype::middleClick( int index )
{
    if ( flags[index] || !(visible[index]) || (raw[index] == '0') )
    {
        return 0 ;
    }

    int w = pParams->length ;
    int h = pParams->height ;
    int square[9] {0} ;
    int gameOver = 0, flagNum = 0 ;

    getSquare( index % w, index / w, w, h, square) ;

    for ( int i = 0, position = 0; i < 9; i++)  // Search for a flag/mine mismatch and count flags
    {
        position = square[i] ;
        if ( position != -1 )
        {
            if ( ((raw[position] == 'X') != flags[position]) && (gameOver == 0) ) { gameOver = 1 ; }
            if ( flags[position] )                                                { flagNum++    ; }
        }
    }

    if ( ((char)('0' + flagNum)) != raw[index])   // Mismatch of number of flags: ignore
    {
        return 0 ;
    }
    if ( gameOver )
    {
        return 1 ;
    }

    for ( int i = 0, position = 0; i < 9; i++) // Single click non-flagged, non-visible squares
    {
        position = square[i] ;
        if ( (position != -1) && ( !(flags[position]) ) && ( !(visible[position])) )  /// Remember to use "position", not "index"!
        {
            singleClick( position ) ;
        }
    }
    return 0 ;
}

int Gridtype::checkWin()
{
    if ( flagNumber != pParams->mines )
    {
        return 0 ; // No win
    }

    //char state = '0' ; // For below loop, depreciated
    for ( int i = 0; i < pParams->mines; i++ ) // Optimization idea: can do a bitmask of bit vectors mines and flags, check if zero
    {
        if ( !( flags[mineList[i]] )  ) // See if any mines aren't flagged
        {
            return 0 ;
        }
    }

    hasWon = 1 ;
    return 1   ; // Win
}

void Gridtype::dispEnd( Cordtype& lastPoint )
{
    disp();
    if ( hasWon )
    {
        std::cout << "Game won! Last point: " << lastPoint.get_x() << ", " << lastPoint.get_y() << '\n' ;
        return ;
    }
    std::cout << "Game lost! Last point: " << lastPoint.get_x() << ", " << lastPoint.get_y() << '\n' ;
    return ;
}




