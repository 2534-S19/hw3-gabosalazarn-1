#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework3.h"
#include "myGPIO.h"
#include "myTimer.h"
typedef enum {UP, DOWN} button_state;

int main(void)
{
    // Count variables to control the LEDs.
    unsigned int count0 = 0;
    unsigned int count1 = 0;
    unsigned char buttonHistory = UNPRESSED;
    bool debouncedState = false;

    // TODO: Declare the variables that main uses to interact with your state machine.


    // Stops the Watchdog timer.
    initBoard();
    // Initialize the GPIO.
    // YOU MUST WRITE THIS FUNCTION IN myGPIO.c
    initGPIO();
    // Initialize Timer0 to provide a one second count interval for updating LED2.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_0_BASE, TIMER0_PRESCALER, TIMER0_COUNT);
    // Initialize Timer1 to provide a one millisecond count interval for updating the button history.
    // YOU MUST WRITE THIS FUNCTION IN myTimer.c
    initTimer(TIMER32_1_BASE, TIMER1_PRESCALER, TIMER1_COUNT);

    while(1)
    {
        // Update the color of LED2 using count0 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        changeLaunchpadLED2(count0);
        // Update the color of the Boosterpack LED using count1 as the index.
        // YOU MUST WRITE THIS FUNCTION BELOW.
        changeBoosterpackLED(count1);

        // TODO: If Timer0 has expired, increment count0.
        // YOU MUST WRITE timer0expired IN myTimer.c
        if(timer0Expired()){
            count0++;
        }



        // TODO: If Timer1 has expired, update the button history from the pushbutton value.
        // YOU MUST WRITE timer1expired IN myTimer.c
        if(timer1Expired())
            buttonHistory = (buttonHistory << 1) + checkStatus_BoosterpackS1();


        // TODO: Call the button state machine function to check for a completed, debounced button press.
        // YOU MUST WRITE THIS FUNCTION BELOW
        debouncedState = fsmBoosterpackButtonS1(buttonHistory);


        // TODO: If a completed, debounced button press has occurred, increment count1.
        if(debouncedState){
            count1++;
        }


    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: Map the value of a count variable to a color for LED2.
// Since count is an unsigned integer, you can mask the value in some way.
void changeLaunchpadLED2(unsigned int count)
{
    if(count & BIT0)
        turnOn_LaunchpadLED2Red();
    else
        turnOff_LaunchpadLED2Red();
    if(count & BIT1)
        turnOn_LaunchpadLED2Green();
    else
        turnOff_LaunchpadLED2Green();
    if(count & BIT2)
        turnOn_LaunchpadLED2Blue();
    else
        turnOff_LaunchpadLED2Blue();
}

// TODO: Maybe the value of a count variable to a color for the Boosterpack LED
// This is essentially a copy of the previous function, using a different LED
void changeBoosterpackLED(unsigned int count)
{
    if(count & BIT0)
        turnOn_BoosterpackLEDRed();
    else
        turnOff_BoosterpackLEDRed();
    if(count & BIT1)
        turnOn_BoosterpackLEDGreen();
    else
        turnOff_BoosterpackLEDGreen();
    if(count & BIT2)
        turnOn_BoosterpackLEDBlue();
    else
        turnOff_BoosterpackLEDBlue();
}

// TODO: Create a button state machine.
// The button state machine should return true or false to indicate a completed, debounced button press.
bool fsmBoosterpackButtonS1(unsigned char buttonhistory)
{
    static button_state currentState = UP;
    bool pressed = false;
    switch(currentState){
    case UP:
        if(buttonhistory == PRESSED)
            currentState = DOWN;
        break;
    case DOWN:
        if(buttonhistory == UNPRESSED){
            currentState = UP;
            pressed = true;
        }
        break;
    }

    return pressed;
}
