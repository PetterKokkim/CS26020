/*
 * File:   newmainXC16.c
 * Author: pwk1 (Petter W. Kokkim)
 *
 * Created on April 16, 2020, 9:30 PM
 */

#include "allcode_api.h"    // MUST include this to define robot functions

int main() {
    FA_RobotInit(); // MUST call this before any other robot functions
    FA_CompassInit(); //  Initiating the internal compass
    
    FA_LCDBacklight(50); // Switch on backlight (half brightness)
    FA_LCDPrint("Hello", 5, 20, 25, FONT_NORMAL, LCD_OPAQUE); // Say hi!
    FA_DelayMillis(1000); // Pause 1 sec
    
    struct map{
        int x;
        int y;
        int z;
    };
    
    int nest;
    
    int axes[3]; 
    
    struct map mapped[4][4];
    
    while (1) {
        FA_SetMotors(25, 25);

        if (FA_ReadLight() < 80) {
            nest++;
        }

        // Turn and dead end turn
        if (FA_ReadIR(0) > 500 && FA_ReadIR(2) > 500 && FA_ReadIR(4) > 500) {
            FA_Backwards(25);
            FA_Right(180);
        }
        if (FA_ReadIR(4) > 500 && FA_ReadIR(2) > 700 && FA_ReadIR(0) < 200) {
            FA_Backwards(15);
            FA_Left(90);
        }
        if (FA_ReadIR(0) > 500 && FA_ReadIR(2) > 700 && FA_ReadIR(4) < 200) {
            FA_Backwards(15);
            FA_Right(90);
        }

        // Wall following
        if (700 <= FA_ReadIR(4) && FA_ReadIR(4) <= 2000) {
            FA_Left(10);
            FA_Forwards(15);
        }
        if (0 <= FA_ReadIR(4) && FA_ReadIR(4) <= 500) {
            FA_Right(10);
            FA_Forwards(15);
        }
        
        if (FA_ReadIR(4) > 3000 || FA_ReadIR(3) > 2000) {
            FA_Backwards(10);
            FA_Left(40);
        }
        if (FA_ReadIR(0) > 3000 ||FA_ReadIR(1) > 2000) {
            FA_Backwards(10);
            FA_Right(40);
        }
        
        FA_LCDClear(); // Clear display
        // Display left and right wheel encoder values
        FA_LCDNumber(FA_ReadEncoder(CHANNEL_LEFT), 0, 0, FONT_NORMAL, LCD_OPAQUE);
        FA_LCDNumber(FA_ReadEncoder(CHANNEL_RIGHT), 40, 0, FONT_NORMAL, LCD_OPAQUE);

        // Display light reading and nr of nests detected. 
        FA_LCDPrint("Light: ", 7, 0, 8, FONT_NORMAL, LCD_OPAQUE);
        FA_LCDNumber(FA_ReadLight(), 40, 8, FONT_NORMAL, LCD_OPAQUE);
        
        FA_LCDPrint("Nest: ", 7, 0, 16, FONT_NORMAL, LCD_OPAQUE);
        FA_LCDNumber(nest, 40, 16, FONT_NORMAL, LCD_OPAQUE);
        
        FA_DelayMillis(100); // Pause 0.1 sec

        int i;
        for (i = 0; i < 8; i++) { // Loop over all 8 IR sensors
            if (FA_ReadIR(i) > 600) { // If obstacle, light corresponding LED
                FA_LEDOn(i);
            } else {
                FA_LEDOff(i); // else switch LED off
            }
        }
        
        if(FA_ReadSwitch(1) == 1){
            int i;
            for(i = 0; i < nest; i++);
            FA_PlayNote(2200,200);
            FA_DelayMillis(100);
        }
    }
    return 0; // Actually, we should never get here...
}
