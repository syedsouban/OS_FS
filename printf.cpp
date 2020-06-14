/*
 * printf.cpp
 *
 *  Created on: 24-May-2020
 *      Author: saubaan
 */
#include "types.h"
#include "printf.h"
#define HORIZONTAL 80
#define VERTICAL 25
void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint16_t x=0,y=0;
    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }
        if(x >= 80)
        {
            x = 0;
            y++;
        }
        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}




