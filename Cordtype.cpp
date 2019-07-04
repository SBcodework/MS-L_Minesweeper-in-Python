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
#include <typeinfo>

Cordtype::Cordtype(Parameter& Params)
{
    setter(Params);
}

Cordtype::~Cordtype()
{

}

Parameter* Cordtype::pParams = nullptr; /// Remember: "static" can not be here.

void Cordtype::setter(int in_x, int in_y, char in_mode)
{
    x = in_x;
    y = in_y;
    index = (in_y*(pParams->length)) + in_x;
    if (in_mode == '\0')
    {
        return;
    }
    mode = in_mode;
    return;
}

int Cordtype::setter(int in_index, int in_mode)
{
    index = in_index;
    if (pParams == nullptr)
    {
        std::cout << "Error: Cordtype::setter(int in_index)'s pParams is null!\n";
        return 1;
    }
    x = in_index % (pParams->length);
    y = in_index % (pParams->height);
    if (in_mode == '\0')
    {
        return 0;
    }
    mode = in_mode;
    return 0;
}

int Cordtype::setter(Parameter& Params)
{
    if (typeid(Params) != typeid(Parameter) )
    {
        std::cout << "Error: Cordtype::setter(Parameter& Params)'s input is invalid!\n";
        return 1;
    }
    pParams = &Params;
    return 0;
}

int Cordtype::get_x()
{
    return x;
}

int Cordtype::get_y()
{
    return y;
}

int Cordtype::get_index()
{
    return index;
}

char Cordtype::get_mode()
{
    return mode;
}

void Cordtype::free_static()
{
    if (pParams == nullptr)
    {
        return;
    }
    delete pParams;
    pParams = nullptr;
    return;
}
