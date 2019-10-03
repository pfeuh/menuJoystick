
#include "menuJoystick.h"

MENU_JOYSTICK joystick = MENU_JOYSTICK(52, 50, 48, 46, 44);

// let's define a set of values
//              0        1       2             3       4
int values[] = {-1,      75,     30,           40,     20}; 
int mins[]   = {-20,     60,     18,           40,     16}; 
int maxs[]   = {-1,      90,     38,           80,     23};
#define NB_VALUES (sizeof(values) / sizeof(int))
byte valuePointer = NB_VALUES - 1;
const char label0[] PROGMEM = "Freezer";
const char label1[] PROGMEM = "Coffee";
const char label2[] PROGMEM = "Swimming pool";
const char label3[] PROGMEM = "Hammam";
const char label4[] PROGMEM = "Aquarium";
const char *const labels[] PROGMEM ={label0, label1, label2, label3, label4};

void print_P(const char* address)// _P means PROGMEM
{
    while(pgm_read_byte(address))
        Serial.write(pgm_read_byte(address++));
}

void printMenu()
{
    // lets display selected columns
    for(byte x = 0; x < NB_VALUES; x++)
    {
        if(x == valuePointer)
            Serial.write('<');
        else
            Serial.write(' ');
        print_P(pgm_read_word(labels + x));
        if(x == valuePointer)
            Serial.write('>');
        else
            Serial.write(' ');
    }
}

void joystickCB(byte direction)
{
    switch(direction)
    {
        case MENU_JOYSTICK_CHAR_UP:
            if(values[valuePointer] < maxs[valuePointer])
                values[valuePointer]++;
            break;
        case MENU_JOYSTICK_CHAR_DOWN:
            if(values[valuePointer] > mins[valuePointer])
                values[valuePointer]--;
            break;
        case MENU_JOYSTICK_CHAR_RIGHT:
            valuePointer = (valuePointer + 1) % NB_VALUES;
            printMenu();
            break;
        case MENU_JOYSTICK_CHAR_LEFT:
            if(valuePointer) 
                valuePointer = (valuePointer - 1);
            else
                valuePointer = NB_VALUES - 1;
            printMenu();
            break;
        case MENU_JOYSTICK_CHAR_FIRE:
            // just to display something...
            printMenu();
            break;
        default:
            break;
    }
    // lets display value of selected columns
    Serial.println(values[valuePointer]);
}

void setup()
{
    joystick.begin();
    joystick.bind(&joystickCB);

    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.begin(9600);
    Serial.println(F("Menu joystick v. " MENU_JOYSTICK_VERSION));
    Serial.println(F(__DATE__ " " __TIME__));
    Serial.println(F("Let's use callback way"));
    Serial.println(F("Press button short/long to switch to another setpoint."));
    Serial.println(F("Clockwise / counter clockwise to adjust setpoint."));

    // let's start with Freezer temperature
    joystickCB(MENU_JOYSTICK_CHAR_RIGHT);
}

void loop()
{
    joystick.sequencer();
    
    // status led sequencer
    if(millis() & 0x200)
        digitalWrite(LED_BUILTIN, 0);
    else
        digitalWrite(LED_BUILTIN, 1);
}
