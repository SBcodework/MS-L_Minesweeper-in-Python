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
#include "common.h"
#include "Parameter.h"
#include "Gridtype.h"
#include "Cordtype.h"
#include <string>
#include <typeinfo>
#include <algorithm>
#include <vector>
#include <bitset>
#include <random>
#include <time.h>
#include <cmath>


int extractDigitInt( int digits, std::string input )
{
    int result = 0;
    int length = input.length();
    if ( length == 0 || digits < 0 )
    {
        return -1;
    }
    /// Find the furthest index of the string representing the smallest digit (under 10) as string length permits.
    int digitsFirstGuess = digits; //(length > digits) ? length : digits;
    int digitsSecondGuess = 0;  /// Below: find the first digit.
    for ( int i = 0; (i < digitsFirstGuess) && (isdigit(input[i])) && (i < length); i++ )
    {
        digitsSecondGuess++;
    }
    if ( digitsSecondGuess == 0 )
    {
        return -1;
    }

    //float digit = digitsSecondGuess;
    for ( int i = (digitsSecondGuess - 1), magnitude = 0, inputDigit = 0, power = 0; i >= 0; i--, power++ )  /// Start backwards from the furthest index
    {
        inputDigit = (int)(input[i] - '0');
        magnitude  = (int)(std::pow(10.0, (float)power));
        result += (inputDigit * magnitude);
    }

    return result;
}

void askXYMinesLoop(Parameter& out_param)
{
    int x, y, mines, area; // Output containers
    std::string strx;
    std::string stry;
    std::string strMines;
    for (;;)
    {
        std::cout << "Grid Length? ";
        std::cin >> strx;
        std::cout << "Grid Height? ";
        std::cin >> stry;
        std::cout << "Number of mines? ";
        std::cin >> strMines;
        x = extractDigitInt(2, strx);
        y = extractDigitInt(2, stry);
        mines = extractDigitInt(4, strMines);  //4 digits

        if ( x < 1 || y < 1 || mines < 1 || x > 99 || y > 99 || mines > 9801 ||
            ( x < 4 && y < 4 ) || ( mines > ((x*y)-9) ) )
        {
            std::cout << "Invalid input! Try again.\n";
            continue;
        }

        out_param.length = x;
        out_param.height = y;
        out_param.mines = mines;
        out_param.init_dimensions();
        return;
        break;
    }
}

void dispEmpty(Parameter& in_param)
{
    int length = in_param.length;
    int height = in_param.height;
    std::cout << "     ";  // 5 spaces
    for (int i=0; i<length; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << "}";
    }
    std::cout << "\n";
    for (int i=0; i<height; i++)
    {
        std::cout << "{" << (i/10) << (i%10) << "} ";
        for (int n=0; n<length; n++)
        {
            std::cout << "[  ]";
        }
        std::cout << "\n";
    }
    return;
}

int askStartXYLoop(Parameter& out_param)
{
    int error = 0, length = 0;
    std::string line;  // "cin.get()" Is not used, as it skips the first character of input.

    for (int skip = 0; ;skip = 0)
    {
        line = "";  // Reset the line each loop
        std::cout << "Enter X cord, followed by a comma, and Y cord. Like '13,8'. \n";
        std::cin >> line;
        length = line.length();
        //std::getline(std::cin, line); Old method, above worked better
        if ( (length < 3) || ((line[1] != ',') && (line[2] != ',')) || !isdigit(line[0]) ||
            (!isdigit(line[1]) && (line[1] != ',')) || (!isdigit(line[2]) && (line[2] != ',')) ||
            ((length > 3) && (line[2] == ',') && !isdigit(line[3]) ) )
        {
            std::cout << "Invalid! Try again. Line: " << line << "\n";
            continue;
        }
        error = h_extractCoords(line, out_param.xStart, out_param.yStart, nullptr, ',');
        if ((out_param.xStart >= out_param.length) || (out_param.yStart >= out_param.height))
        {
            std::cout << "Coordinates out of range! Try again. Line: " << line << "\n";
            continue;
        }

        switch (error)
        {
        case 0:
            break;
        case 1:
            std::cout << "Error in askStartXYLoop! ";
            std::cout << "x, y: " << out_param.xStart << " " << out_param.yStart << "\n";
            return 1;
        case 2:
            std::cout << "Invalid! Try again. Line: " << line << "\n";
            skip = 1;
        }
        if (skip)
        {
            continue;
        }
     out_param.init_start();
     return 0;
    }
}

int h_extractCoords(std::string in_line, int& out_x, int& out_y, char* out_sep, char sep)
{
    int length   = in_line.length();

    if ( (length < 3) || (length > 5) )
    {
        return 2;  // Too long or too short; for legacy reasons 2 is the error code for user errors
    }

    int x = extractDigitInt( 2, in_line );
    int slicedStart = (x < 10) ? 2 : 3;

    if ( (x == -1) || (slicedStart > length) )
    {
        return 2;  // Invalid X or string is too short for a separator to fit
    }

    char separator = in_line[slicedStart - 1];

    if ( ( (separator != sep) && sep != '\0' ) || ( (sep == '\0') && isdigit(separator) ) )
    {
        return 2;  // An invalid separator is found when it is defined in the arguments, or if the separator is a digit when searching for one.
    }

    std::string slicedString = in_line.substr( slicedStart, length );
    int y = extractDigitInt( 2, slicedString );

    if ( y == -1 )
    {
        return 2;  // Invalid y
    }

    if ( out_sep != nullptr )
    {
        *out_sep = separator;
    }

    out_x = x;
    out_y = y;

    return 0;
}

void gengrid(Gridtype& out_grid)
{   // Parameters are declared below
    srand(time(0));  // Init randomizer

    Parameter* params = out_grid.pParams;
    int w = params->length;
    int h = params->height;
    int area = h*w;
    int xstart = params->xStart;
    int ystart = params->yStart;
    int istart = params->iStart; //Unneeded at the moment
    int mines = params->mines;

    char* rawGrid = new char[area];
    std::fill_n(rawGrid, area, '0');  // Fill with zeros

    int startSquare[9] {0}; // Indexes of the start square
    getSquare(xstart, ystart, w, h, startSquare);

    int clearCounter = 0;
    for (int i = 0, state = 0 ; i<9 ; i++) // Go through startSquare
    {
        state = startSquare[i]; /*
        if (state > -1 && state < area)
        {
            continue;
        }*/
        if (state != -1 && state < area) // -1 means an invalid spot
        {
            rawGrid[state] = 'c';
            clearCounter++;  // Count the number of valid mine-less spots
        }
    }

    int* mineList = new int[mines];
    std::fill_n(mineList, mines, 0);
    int mineLCounter = 0;

    int mineCandidates[mines + 9] {0};
    uniqueRand(mines+9, 0, area, mineCandidates); // mineCandidates now has a list of unique mines.

    for (int i = 0; ((i < mines+9) && (mineLCounter < mines)) ; i++)
    {
        if ((rawGrid[mineCandidates[i]] != 'c') && mineLCounter < (mines))
        {
            mineList[mineLCounter] = mineCandidates[i];
            rawGrid[mineList[mineLCounter]] = 'X';
            mineLCounter++; /// This needs to be here, not in the above for() declaration.
        }
    }

    for (int i = 0, state = 0 ; i<9 ; i++) // Go through startSquare, set back to 0's, we don't need 'c's anymore
    {
        state = startSquare[i];
        if (state != -1 && state < area) // -1 means an invalid spot
        {
            rawGrid[state] = '0';
        }
    }

    for (int i = 0, square[9] {0}, currentMine; i < mines; i++)  // Count numbers and put them in
    {
        currentMine = mineList[i];
        getSquare(currentMine % w, currentMine / w, w, h, square); ///Watch in case of bug
        for (int n = 0, state = 0; n < 9; n++)
        {
            state = square[n];
            if ( state != -1 && rawGrid[state] != 'X' && rawGrid[state] >= '0' && rawGrid[state] < '8' )
            {
                rawGrid[state]++; // '0' goes to '1' and so on
            }
        }
    }

    out_grid.set_grid(rawGrid);  // Return
    out_grid.set_minelist(mineList);

    out_grid.action( istart, 's' );
    return;
}

void getSquare(int centerX, int centerY, int w, int h, int out_square[9])
{
    int xpart = 0;
    int ypart = 0;
    int startX = centerX - 1;
    int startY = centerY - 1;

    for (int iy = 0, counter = 0; iy < 3 ; iy++)
    {
        for (int ix = 0; ix < 3; ix++, counter++)
        {
            xpart = startX + ix;
            ypart = startY + iy;
            out_square[counter] = toIndexCheck(xpart, ypart, w, h);
        }
    }
    return;
}

int toIndexCheck(int x, int y, int w, int h)
{
    if (outOfBounds(x, y, w, h))
    {
        return -1;
    }
    return ((y*w) + x );
}

int outOfBounds(int x, int y, int w, int h)
{
    if (x >= 0 && y >= 0 && x < w && y <h)
    {
        return 0;
    }
    return 1;
}

// Numlimit is exclusive. Output must be of size "size".
int uniqueRand(int size, int start, int numlimit, int* output)
{
    /// init srand
    std::vector<bool> bvector(numlimit, 0);

    int currentStart = 0;
    int currentEnd = 0; // Inclusive
    int currentMiddle = 0; // Exclusive; the inclusive start of the second sequence.
    int currentSize = 0;

    for (int counter = 0; counter < size; counter++)
    {
        currentStart = start;
        currentEnd = numlimit;
        currentMiddle = (numlimit / 2); // Seperator at right. -1 used to be here
        currentSize = numlimit - start; /// changed from numlimit

        for (int state = 0, choice = 0 ; ; )
        {
            if (currentStart == (currentEnd-1))
            {
                if (bvector[currentStart] == 1)
                {
                    std::cout << "Duplicate error!\n";
                }
                output[counter] = currentStart; // We have found our number
                bvector[currentStart] = true;
                break;
            }

            state = allCheck(currentStart, currentMiddle, currentEnd, bvector);

            switch (state)
            {
                case 0: // Choose a random element. 0 = left, 1 = right.
                choice = (rand() % 2);
                break;

                case 1: // Choose left
                choice = 0;
                break;

                case 2: // Choose right
                choice = 1;
                break;

                case 3:
                std::cout << "Array full! Size: " << currentSize << " Start: " << currentStart << " Middle: " << currentMiddle << " End: " << currentEnd << "\n";
                for ( int n=0; n<size; n++)
                {
                    std::cout << output[n];
                }
                std::cout << "\n";
                return 1;

            }

            switch (choice)
            {
                case 0:
                currentEnd = currentMiddle;
                currentSize = currentEnd - currentStart; // removed +1
                currentMiddle = currentStart + (currentSize / 2); // removed -1, added currentstart
                break;

                case 1:
                currentStart = currentMiddle; //removed +1
                currentSize = currentEnd - currentStart; //removed +1
                currentMiddle = currentStart + (currentSize / 2); //removed -1
                break;
            }
        }
    }
    return 0;
}
// OLD documentation:
// Include start and end. Middle refers to the seperatation RIGHT of the index. It is added one locally, same with end.
// Return 0 if neither left or right is all 1's. 1 if only the left one has a zero in it, 2 if it's right, and 3 for both.
// New documentation: Include start, in_middle includes the start of the second sequence, in_end is exclusive.
int allCheck(int start, int middle, int end, std::vector<bool>& in_vector)
{
    int allLeft = 1; // 1 meaning all ones are in the bit array.
    int allRight = 1;
    ///int middle = in_middle + 1; // LEFT of index; testing
    //int end = in_end + 1; // Exclusive
    //int middle = (end - start) / 2;
    for (int currentStart = start, currentEnd = middle, *flag = &allLeft, t = 0;
         t < 2;
         t++, currentStart = middle, currentEnd = end, flag = &allRight) // Two times, go through left and right

    {
        for (int i = currentStart , size = currentEnd - currentStart; i < currentEnd; i++ ) // Commented out portion was unused
        {
            if (in_vector[i] == false) // Remember to init the vector with zeroes, else seg fault here
            {
                *flag = 0;  // There's a zero in the bit array.
                break;
            }
        }
    }

    switch (allLeft + allRight)
    {
        case 0: // Neither are ones
            return 0;

        case 1:
            if (allLeft) // Right only has no ones
                {
                    return 2;
                }
                return 1; // Left has no ones

        case 2:
            std::cout << "Sub-array full! Dump: \n";
            for ( int n=0; n<(end-start); n++)
            {
                std::cout << in_vector[n];
            }
            std::cout << "\n";
            return 3; // Both have ones
    }
    std::cout << "Error in allCheck(...)!\n";  // Shouldn't reach here, added to avoid compiler warning
    return 0;
}

void askSelectionXYTLoop( Cordtype& out_cord )
{
    // Gather input (h_extract_cords)
    // Output trimmed input to out_cord

    // Find the seperator, then run h_extractcords
    std::string line = "\0" ;
    int x = 0, y = 0 ;
    char sep = '1';


    for (int error = 1, loopError = 1; loopError ;)
    {
        std::cout << "\nSelect a point and an action. 's' to uncover, 'm' to middle click, and 'f' to flag. Like '13f7'. \n";
        std::cin >> line;

        error = h_extractCoords( line, x, y, &sep ) ;
        if ( error || (x < 0) || (y < 0) || (x > (out_cord.pParams->length)) || (y > (out_cord.pParams->height) ) )
        {
            std::cout << "Invalid input! Try again.\n";
            continue;
        }

        switch (sep)
        {
        case 's':

        case 'm':

        case 'f':
            loopError = 0;
            break;
        default:
            std::cout << "AskXYT Error, sep is invalid! Sep: " << sep << ". Try again.\n";
        }
    }

    out_cord.setter(x, y, sep) ;

    return ;
}








