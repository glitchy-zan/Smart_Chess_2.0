# Smart Chessboard with ESP32

This project turns a traditional chessboard into a smart chessboard using an ESP32 microcontroller. The ESP32 collects data from sensors embedded in the chessboard, preprocesses the data, and displays Portable Game Notation (PGN) via its access point (AP) for easy viewing. The project includes C++ code for the ESP32 to handle sensor input, move detection, and PGN generation.

## Features

- **Real-time Move Detection:** Sensors detect piece movements on the board.
- **Data Preprocessing:** The ESP32 preprocesses the sensor data to identify valid moves.
- **PGN Display:** The game is displayed in Portable Game Notation (PGN) format on the ESP32's access point.
- **ESP32 Access Point:** The ESP32 runs an access point to broadcast the PGN data for easy access via a web browser.

## Table of Contents

- [Getting Started](#getting-started)
- [Hardware Setup](#hardware-setup)
- [Software Installation](#software-installation)
- [How It Works](#how-it-works)

## Getting Started

### Prerequisites

To run this project, you will need:

- ESP32 microcontroller
- 64 Hall Effect Sensors (for detecting chess piece positions)
- 64 magnets (to place on chess pieces)
- 8 PISO shift registers
- Button
- PlatformIO for programming the ESP32
- A Wi-Fi-enabled device to connect to the ESP32 access point

### Hardware Setup

1. Attach sensors to each square of the chessboard to detect piece movement.
2. Connect each row of sensors to corresponding PISO shift register (because there are not enough pins on ESP32)
3. Connect registers to the ESP32 GPIO pins.
4. Connect sensors and registers to power supply and ground.
5. Connect button to VCC and ground also (for submitting moves)
6. Power the ESP32 via USB or battery.

### Software Installation

1. Clone the repository:
   git clone https://github.com/glitchy-zan/Smart_Chess_2.0
2. Open the project in PlatformIO.
3. Install the necessary libraries (e.g., WebSockets).
4. Upload the code to the ESP32 by connecting your ESP32 via USB and clicking the **Upload** button in PlatformIO.

### How It Works

- **Sensor Input:** Each sensor is assigned to a square on the chessboard. When a piece is moved, the sensor changes state.
- **Data Processing:** The ESP32 collects sensor data and maps it to chess moves.
- **PGN Generation:** The moves are converted into Portable Game Notation (PGN) format, a standard for recording chess games.
- **Access Point:** The ESP32 creates a local Wi-Fi network. Users can connect to it and view the PGN through a web browser by navigating to the default IP address.

### Video presentation
yt: https://youtube.com/shorts/g3ANoQOUdOU
