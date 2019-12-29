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

/**
 * Button data structure containing HID button id, button state, and if this
 * state is new for the button
 */
typedef struct {
    // HID ID for the button the produced this data
    int HIDid;

    // Current state of the button
    bool state;

    // Is this data new?
    bool isNew;
} ButtonData;

class JoystickButton {
   private:
    // Digital pin to read from
    int digitalPin;

    // "Button" ID to write to on computer
    int buttonId;

    // Keep track of button state
    bool lastState = false;

   public:
    /**
     * Create a new JoystickButton
     *
     * @param digitalPin Arduino digital pin to read button data from
     * @param buttonID HID button ID to write state to
     */
    JoystickButton(int, int);

    /**
     * Initialize button I/O
     */
    void init();

    /**
     * Get the un-filtered button state.
     *
     * @return Button state
     */
    bool getRawButton();

    /**
     * Get button data
     *
     * @return Current button data
     */
    ButtonData getButtonData();
};

JoystickButton::JoystickButton(int digitalPin, int buttonId) {
    // Set local variables
    this->digitalPin = digitalPin;
    this->buttonId = buttonId;
}

void JoystickButton::init() {
    // Set pin mode to pull up. More info about this feature can be found
    // here: https://www.arduino.cc/en/Tutorial/InputPullupSerial
    pinMode(this->digitalPin, INPUT_PULLUP);
}

bool JoystickButton::getRawButton() {
    // The INPUT_PULLUP mode set for the pin will cause button states to be
    // inverted. Using "!" will flip them to what we expect
    return !(digitalRead(this->digitalPin) == 0);
}

ButtonData JoystickButton::getButtonData() {
    // Alloc a ButtonData struct
    ButtonData output;

    // Fill output with state Info
    output.HIDid = this->buttonId;
    output.state = this->getRawButton();
    output.isNew = (this->lastState != output.state);

    // Set the last state for the button
    this->lastState = output.isNew;

    // Return the output
    return output;
}

// Button mappings
// For the people who only know java, this is a quick way to make an array of
// objects in C++. It is equivalent to the following java code:
// JoystickButton[] buttons = new JoystickButton[]{ ... }
// The innter curly braces define the arguments passed to each object's
// constructor
JoystickButton buttons[] = {{2, 0}, {3, 1}};

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

    // Init the Joystick library (passing false will disable auto-updates)
    Joystick.begin(false);
}

/**
 * Arduino loop function. This will loop
 */
void loop() {
    // Read each button for new data
    for (int i = 0; i < buttonCount; i++) {
        // Get the button's current data
        ButtonData data = buttons[i].getButtonData();

        // Check if this data is new
        if (data.isNew) {
            // Push data
            Joystick.setButton(data.HIDid, data.state);
        }
    }

    // Push HID data to host computer
    Joystick.sendState();

    // Wait for the set period
    delay(updatePeriodMS);
}
