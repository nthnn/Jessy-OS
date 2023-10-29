<p align="center">
    <img src="assets/jessy_os_logo_dark.png#gh-dark-mode-only" width="200" />
    <img src="assets/jessy_os_logo_light.png#gh-light-mode-only" width="200" />
</p>
<h1 align="center">Jessy OS</h1>

Jessy OS is an embedded operating system designed for the ESP32 microcontroller. It provides a versatile platform for various IoT and embedded systems projects, with features like a file system, real-time clock support, networking capabilities, user accounts, and JavaScript execution via the embedded Duktape VM.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
    * [Components](#components)
    * [Flashing the OS](#flashing-the-os)
    * [Running](#running)
- [Documentations](#documentations)
- [Contributions](#contributions)
    * [Issue Reporting](#issue-reporting)
    * [Pull Requests](#pull-requests)
- [Contributors](#contributors)
- [License](#license)

## Features

- **File System**: Jessy OS includes its own file system, allowing you to manage and access files on the ESP32 device.
- **Real-Time Clock (RTC)**: It supports the DS1307 real-time clock via I2C, enabling accurate timekeeping for your applications.
- **User Accounts**: You can create and manage multiple user accounts, making it suitable for applications requiring user authentication.
- **Networking**: Jessy OS includes networking functionality with a WLAN command for managing Wi-Fi connections and network configurations.
- **JavaScript Support**: The embedded Duktape VM allows you to execute JavaScript programs, expanding the range of applications you can build.

## Getting Started

### Components

Before getting started with Jessy OS, you'll need the following:

- An ESP32 microcontroller.
- A compatible SD card adapter module for file system support.
- A DS1307 real-time clock module connected via I2C.

After gathering the aforementioned components, connect it as follows:

<table align="center">
    <thead>
        <tr>
            <td>Component</td>
            <td>ESP32</td>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan="2" align="center">SD Card</td>
        </tr>
        <tr>
            <td>VCC</td>
            <td>VCC</td>
        </tr>
        <tr>
            <td>GND</td>
            <td>GND</td>
        </tr>
        <tr>
            <td>CS</td>
            <td>GPIO5</td>
        </tr>
        <tr>
            <td>MOSI</td>
            <td>GPIO23</td>
        </tr>
        <tr>
            <td>MISO</td>
            <td>GPIO19</td>
        </tr>
        <tr>
            <td>SCK</td>
            <td>GPIO18</td>
        </tr>
        <tr>
            <td colspan="2" align="center">DS1307 RTC</td>
        </tr>
        <tr>
            <td>VCC</td>
            <td>VCC</td>
        </tr>
        <tr>
            <td>GND</td>
            <td>GND</td>
        </tr>
        <tr>
            <td>SDA</td>
            <td>SDA</td>
        </tr>
        <tr>
            <td>SCL</td>
            <td>SCL</td>
        </tr>
    </tbody>
</table>

### Flashing the OS

1. Clone the Jessy OS repository.

    ```bash
    git clone https://github.com/nthnn/Jessy-OS.git
    ```

2. Open the clone repository via PlatformIO extension on VS Code.
3. Plug your board in, and then press the upload button on the status bar of VS Code.
4. Enjoy!

### Running

Communication to Jessy OS can be done through UART serial communication, which means, it just needs to be connected to the USB port (e.g. via CH340, PL2302, etc).

1. Connect the ESP32 (e.g. ESP32 DevKit 1.0) board to USB port.
2. Open the `jsym`. Program dedicated for communicating with Jessy OS via UART.
3. Press the reset button on ESP32 DevKit board.

## Documentations

The documentation includes documentation on how Jessy OS works, the available terminal commands, and the JavaScript APIs.

<p align="center">
	<a href="https://jessy-os.vercel.app/"><img src="https://img.shields.io/badge/Jessy%20OS%20Docs-007ec6?style=for-the-badge&logoColor=white&logo=Vercel" /></a>
</p>

## Contributions

Contributions and feedback are all welcome to enhance this library. If you encounter any issues, have suggestions for improvements, or would like to contribute code, please do so.

### Issue Reporting

If you encounter a bug, have a feature request, or want to suggest improvements, please open an issue on the [GitHub Issue Tracker](https://github.com/nthnn/Jessy-OS/issues). Be sure to provide as much detail as possible, including steps to reproduce the issue if applicable.

### Pull Requests

If you want to contribute to Jessy OS, follow these steps:

1. Fork the Jessy OS repository to your GitHub account. And then clone it to your local machine.

    ```bash
    git clone https://github.com/<your username>/Jessy-OS.git
    ```

2. Create a new branch for your changes:

    ```bash
    git checkout -b feature/<your feature name>
    ```

3. You can now make changes to the repository.
4. Commit your changes:

    ```bash
    git add -A
    git commit -m "Add your meaningful commit message here"
    ```

5. Push your changes to your forked repository:

    ```bash
    git push origin feature/<your feature name>
    ```

6. Create a pull request (PR) from your branch to the main branch of the Jessy OS repository.
7. Your PR will be reviewed, and any necessary changes will be discussed and implemented.
8. Once your PR is approved, it will be merged into the main branch, and your contribution will be part of Jessy OS.

## Contributors

- [Nathanne Isip](https://github.com/nthnn) - Original Author, Developer
- [Lady Selene](https://instagram.com/lady.selenee) - Logo designer (Jessy OS is named after her)

## To-do

1. Make a GNU's `wget`-like command.
2. Port any other tiny programming/scripting language other than Duktape's JavaScript.
3. Scheduling and auto-run tasks on boot-up.
4. Design a customized PCB.

## License

Copyright 2023 - Nathanne Isip

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.