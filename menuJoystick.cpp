/*
 * file : menuJoystick.cpp
 * Copyright (c) pfeuh <ze.pfeuh@gmail>
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "menuJoystick.h"

#define MENU_JOYSTICK_ALL_BITS_SLEEPING_VALUE 0xff
#define MENU_JOYSTICK_WEIGHT_UNUSED_BITS_FOR_4 0xf0
#define MENU_JOYSTICK_WEIGHT_UNUSED_BITS_FOR_5 0xe0

#define MENU_JOYSTICK_WEIGHT_UP 0x01
#define MENU_JOYSTICK_WEIGHT_DOWN 0x02
#define MENU_JOYSTICK_WEIGHT_LEFT 0x04
#define MENU_JOYSTICK_WEIGHT_RIGHT 0x08
#define MENU_JOYSTICK_WEIGHT_FIRE 0x10

/*******************/
/* Private methods */
/*******************/

void MENU_JOYSTICK::bufferAdd(byte c)
{
    buffer[bufferInPtr++] = c;
    bufferInPtr %= MENU_JOYSTICK_BUFFER_SIZE;
}

void MENU_JOYSTICK::sendDirection(byte direction)
{
    if(dataHandler)
        dataHandler(direction);
    else
        bufferAdd(direction);
}

/******************/
/* Public methods */
/******************/

MENU_JOYSTICK::MENU_JOYSTICK(int _pinUp, int _pinDown, int _pinLeft, int _pinRight, int _pinFire)
{
    pinUp = _pinUp;
    pinDown = _pinDown;
    pinLeft = _pinLeft;
    pinRight = _pinRight;
    pinFire = _pinFire;    
}

MENU_JOYSTICK::MENU_JOYSTICK(int _pinUp, int _pinDown, int _pinLeft, int _pinRight)
{
    pinUp = _pinUp;
    pinDown = _pinDown;
    pinLeft = _pinLeft;
    pinRight = _pinRight;
    pinFire = 0;    
}

void MENU_JOYSTICK::begin()
{
    pinMode(pinUp, INPUT_PULLUP);
    pinMode(pinDown, INPUT_PULLUP);
    pinMode(pinLeft, INPUT_PULLUP);
    pinMode(pinRight, INPUT_PULLUP);
    pinMode(pinFire, INPUT_PULLUP);
    oldBits = MENU_JOYSTICK_ALL_BITS_SLEEPING_VALUE;
    bufferInPtr = 0;
    bufferOutPtr = 0;
    dataHandler = NULL;
}

void MENU_JOYSTICK::bind(void(*data_callback)(byte direction))
{
    dataHandler   = data_callback;
}

void MENU_JOYSTICK::sequencer()
{
    byte newBits;

    // let's concatenate bits
    newBits = digitalRead(pinUp) + 
        (digitalRead(pinDown) << 1)  +
        (digitalRead(pinLeft) << 2)  +
        (digitalRead(pinRight) << 3);
    if(pinFire)
        newBits += (digitalRead(pinFire) << 4) + MENU_JOYSTICK_WEIGHT_UNUSED_BITS_FOR_5; 
    else
        newBits += MENU_JOYSTICK_WEIGHT_UNUSED_BITS_FOR_4;

    // is there at least 1 bit active?
    if(newBits != MENU_JOYSTICK_ALL_BITS_SLEEPING_VALUE)
    {
        // is there a falling edge?
        if (oldBits == MENU_JOYSTICK_ALL_BITS_SLEEPING_VALUE)
        {
            // yes, let's send the first founded direction bit
            if (!(newBits & MENU_JOYSTICK_WEIGHT_UP))
                sendDirection(MENU_JOYSTICK_CHAR_UP);
            else if (!(newBits & MENU_JOYSTICK_WEIGHT_DOWN))
                sendDirection(MENU_JOYSTICK_CHAR_DOWN);
            else if (!(newBits & MENU_JOYSTICK_WEIGHT_LEFT))
                sendDirection(MENU_JOYSTICK_CHAR_LEFT);
            else if (!(newBits & MENU_JOYSTICK_WEIGHT_RIGHT))
                sendDirection(MENU_JOYSTICK_CHAR_RIGHT);
            else if (!(newBits & MENU_JOYSTICK_WEIGHT_FIRE))
                sendDirection(MENU_JOYSTICK_CHAR_FIRE);
        }
    }
        
    oldBits = newBits;
}

byte MENU_JOYSTICK::available()
{
    return bufferInPtr - bufferOutPtr;
}

byte MENU_JOYSTICK::read()
{
    byte c = buffer[bufferOutPtr++];
    bufferOutPtr %= MENU_JOYSTICK_BUFFER_SIZE;
    return c;
}

