/*                                           ### description ###   
    Library file for using a DS1803 dual digital potentiometer with an arduino.  
    please see 'README.txt' for more information

    Copyright (C) <2020>  <Chris Nichols.  github/return5>
    Modified to run on wire1, for Esp32-Heltec by github/qwksilver
    Run on Pins 21,22 with 2k pullup confirmed

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//-------------------------------------------- headers --------------------------------------------

#include "DS1803.h"     //header file for DS1803 library
#include "Arduino.h"
#include "heltec.h"

//-------------------------------------------- prototypes -----------------------------------------

//function checks to make sure only a valid wiper was selected
static int checkValidWiper(const int wiper);

//function checks to makes sure value is a valid value in the range [0,255]
static int checkValidValue(const int value);

//-------------------------------------------- code -----------------------------------------------

//function checks to make sure only a valid wiper was selected
static int checkValidWiper(const int wiper) {
    char c[26];     //holds string to be printed
    switch(wiper) {  
        case WIPER_0: //FALLTHROUGH
        case WIPER_1: //FALLTHROUGH
        case WIPER_BOTH:
            return 1;  //valid wiper so return 1;
        default:
            snprintf(c,26,"%#04x is incorrect wiper.",wiper);   //format the string to print
            Serial.println(c);                                 //print the string to serial monitor
            return 0;                                         //invalid wiper so return 0;
    }
}

//function checks to makes sure value is a valid value in the range [0,255]
static int checkValidValue(const int value) {
    char c[22];     //holds string to be printed
    switch(value) { 
        case 0 ... 255:  
            return 1;  //valid value so return 1;
        default: 
            snprintf(c,22,"%d is out of range.",value);  //format the string to print
            Serial.println(c);                          //print the string to serial monitor
            return 0;                                  //incorrect value so return 0;
    }
}

//function to set value of wiper. takes in which address of device, wiper to write to and the value to write to it.
void setWiper(const ADDRESS address,const int wiper, const int value) {    
    if(checkValidWiper(wiper) && checkValidValue(value)) {            //if both value and wiper are valid
        char c[25];                                                  //holds string to be printed
        Wire1.beginTransmission(address);                            //begin transmiting to potentiometer at ADRESS
        Wire1.write(wiper);                                         //tell it which wiper to write to   
        Wire1.write(value);                                        //write the value to the wiper
        Wire1.endTransmission();                                  //end transmission to the DS1803
        snprintf(c,25,"writing %d to wiper %#04x",value,wiper); //format the string to print
        Serial.println(c);                                     //print string to serial monitor
    }
}

//function which reads the current value for both wipers at given address and prints values to the serial monitor
void readWipers(const ADDRESS address) {
    int wiper = 0;                                                       //shows which wiper is being read.
    char c[15];                                                         //hold formatted string. 
    Wire1.requestFrom(address,2);                                      //write the value to the wiper
    while(Wire1.available()) {                                        // slave may send less than requested
        snprintf(c,15,"wiper %d is %d",wiper++,(int)Wire1.read());    //format the string before printing
        Serial.println(c);                                     // print the formatted string
    }                                                         
}

//function which starts serial communication and starts i2c communication with potentiometer

void initDS1803() {
    Serial.begin(115200);  //serial communication at 9600 baud
    Wire1.begin(21, 22); //init wire.h library and join the i2c bus
}   
