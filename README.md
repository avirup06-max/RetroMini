# RetroMini
Turn your ESP32 into a pocket arcade with 7 classic games, joystick controls, buzzer beeps, and high-score magic!

*RetroMini* is a 7-in-1 retro-style handheld gaming console built using the ESP32 microcontroller. It features an OLED display, analog joystick, push buttons, a buzzer for sound effects, and SD card support for saving high scores. This project brings multiple classic arcade games into one compact, portable device.

## Features

- 128x64 OLED display (SSD1306, I2C)
- Analog joystick and 5 push buttons for game controls
- SD card support to store and retrieve high scores
- Buzzer for simple game sound effects
- Includes 7 classic games:
  - Snake
  - Tetris
  - Flappy Bird
  - Pong
  - Asteroid Shooter
  - Car Racing
  - Tic Tac Toe
- Game selection and menu navigation via joystick and buttons
- Organized and modular code with easy game switching

## Hardware Used

- ESP32 Dev Board
- OLED Display (SSD1306, I2C)
- Analog Joystick Module
- 5 Tactile Push Buttons
- Buzzer
- SD Card Module
- Breadboard and Jumper Wires

## Pin Configuration

| Component        | ESP32 Pin        |
|------------------|------------------|
| OLED SDA         | GPIO 21          |
| OLED SCL         | GPIO 22          |
| Joystick VERT    | GPIO 34 (Analog) |
| Joystick HORZ    | GPIO 35 (Analog) |
| Joystick SEL     | GPIO 14          |
| Button Up        | GPIO 32          |
| Button Down      | GPIO 33          |
| Button Left      | GPIO 25          |
| Button Right     | GPIO 26          |
| Button Center    | GPIO 27          |
| Buzzer           | GPIO 15          |
| SD Card CS       | GPIO 5           |
| SD Card MOSI     | GPIO 23          |
| SD Card MISO     | GPIO 19          |
| SD Card SCK      | GPIO 18          |
| SD Card VCC      | 3.3V             |
| SD Card GND      | GND              |

## Libraries Required

Ensure the following libraries are installed in the Arduino IDE:

- Adafruit SSD1306
- Adafruit GFX
- SPI
- SD
- Wire

## File Structure

ESPcade/ ├── ESPcade.ino ├── images/ │   ├── menu.png │   └── snake.png ├── README.md └── highscores/ ├── snake.txt └── tetris.txt

<img width="701" height="492" alt="image" src="https://github.com/user-attachments/assets/45faf5f6-28c6-4825-ad8c-2c8d25843766" />

## Getting Started

1. Assemble the hardware according to the pin configuration.
2. Install the required libraries using the Arduino Library Manager.
3. Upload the ESPcade.ino sketch to the ESP32 board.
4. Insert an SD card with optional pre-created score files (e.g., snake.txt, tetris.txt).
5. Power the device and enjoy playing the games.


## Author

Developed by Avirup Bhunia. Contributions and suggestions are welcome.
