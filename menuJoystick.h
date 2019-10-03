#ifndef menuJoystick_h
#define menuJoystick_h

/*
 * file : menuJoystick.h
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

#include <Arduino.h>

#define MENU_JOYSTICK_VERSION "1.00"

#define MENU_JOYSTICK_TRACK_DEBOUCE_DURATION_MSEC 20
#define MENU_JOYSTICK_BUTTON_DEBOUCE_DURATION_MSEC 200

#define MENU_JOYSTICK_CHAR_UP    'u'
#define MENU_JOYSTICK_CHAR_DOWN  'd'
#define MENU_JOYSTICK_CHAR_RIGHT 'r'
#define MENU_JOYSTICK_CHAR_LEFT  'l'
#define MENU_JOYSTICK_CHAR_FIRE  'f'

#define MENU_JOYSTICK_BUFFER_SIZE 4

class MENU_JOYSTICK
{
    public:
        // pins sorted as in the original Atari joystick
        MENU_JOYSTICK(int _pinUp, int _pinDown, int _pinLeft, int _pinRight, int _pinFire);
        MENU_JOYSTICK(int _pinUp, int _pinDown, int _pinLeft, int _pinRight);
        void begin();
        void sequencer();
        void bind(void(*data_callback)(byte direction));
        byte available();
        byte read();

    private:
        int pinUp;
        int pinDown;
        int pinRight;
        int pinLeft;
        int pinFire;
        byte oldBits;
        void (*dataHandler)(byte direction);
        byte buffer[MENU_JOYSTICK_BUFFER_SIZE];
        void sendDirection(byte direction);
        byte bufferInPtr;
        byte bufferOutPtr;
        void bufferAdd(byte c);

};

#endif
