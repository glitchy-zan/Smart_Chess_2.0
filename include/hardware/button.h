#ifndef BUTTON_H
#define BUTTON_H

#include <hardware/pin_config.h>

void IRAM_ATTR handleButtonPress();
void setUpCallbackForButton();

#endif // BUTTON_H