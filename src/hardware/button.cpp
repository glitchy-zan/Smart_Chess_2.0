#include <Arduino.h>
#include <hardware/button.h>
#include <hardware/pin_config.h>

bool flagButtonPressed = false;

/* ISR for button press */
void IRAM_ATTR handleButtonPress()
{
    // Debounce
    static unsigned long lastInterruptTime = 0;
    unsigned long interruptTime = millis();
    if (interruptTime - lastInterruptTime > 500)
    { // 500 ms debounce time
        lastInterruptTime = interruptTime;
        flagButtonPressed = true; // Set the flag
    }
}

void setUpCallbackForButton()
{
    attachInterrupt(digitalPinToInterrupt(button_pin), handleButtonPress, FALLING);
}