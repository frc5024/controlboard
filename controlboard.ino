/**
 * ############################## License Info ################################
 * This file is part of ControlBoard <https://github.com/frc5024/controlboard>.
 * Copyright (c) 2020 Raider Robotics.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * ############################## Program Info ################################
 *
 * Team 5024's Arduino-based DriverStation control board source code.
 *
 * This software is designed to run on an Arduino Leonardo. It will register
 * itself with a host computer as a standard HID joystick. This joystick can be
 * read by FRC DriverStation software, and can be used in FRC robot code with
 * the help of Lib5K's frc.lib5k.interface.controlboard package.
 */

// Include needed headers
#include "Joystick.h"

// Constants
#define updatePeriodMS 20

// Create a HID Joystick interface class
Joystick_ Joystick;

class JoystickButton {
   private:
    // Digital pin to read from
    int digitalPin;

    // "Button" ID to write to on computer
    int buttonId;

   public:
    /**
     * Create a new JoystickButton
     *
     * @param digitalPin Arduino digital pin to read button data from
     * @param buttonID HID button ID to write state to
     */
    JoystickButton(int digitalPin, int buttonId) {
        // Set local variables
        this->digitalPin = digitalPin;
        this->buttonId = buttonId;
    }

    /**
     * Initialize button I/O
     */
    void init() {
        // Set pin mode to pull up. More info about this feature can be found
        // here: https://www.arduino.cc/en/Tutorial/InputPullupSerial
        pinMode(this->digitalPin, INPUT_PULLUP);
    }

    /**
     * Get the un-filtered button state.
     *
     * @return Button state
     */
    bool getRawButton() {
        // The INPUT_PULLUP mode set for the pin will cause button states to be
        // inverted. Using "!" will flip them to what we expect
        return !(digitalRead(this->digitalPin) == 0);
    }
}

// Button mappings
JoystickButton buttons[] = {new JoystickButton(2, 0)};

// Count the number of buttons registered
const int buttonCount = (sizeof(buttons) / sizeof(buttons[0]));

/**
 * Arduino setup function. All init code should go here
 */
void setup() {
    // Init each button
    for (int i = 0; i < buttonCount; i++) {
        // Call button's init function
        buttons[i].init();
    }

    // Init the Joystick library
    Joystick.begin();
}

/**
 * Arduino loop function. This will loop
 */
void loop() {
    // Wait for the set period
    delay(updatePeriodMS);
}
