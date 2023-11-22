<p align="center">
    <img src="assets/jessy_os_logo_dark.png#gh-dark-mode-only" width="200" />
    <img src="assets/jessy_os_logo_light.png#gh-light-mode-only" width="200" />
</p>
<h1 align="center">Jessy OS</h1>

![Go Build](https://github.com/nthnn/Jessy-OS/actions/workflows/go-build.yml/badge.svg)
![Spell Check](https://github.com/nthnn/Jessy-OS/actions/workflows/spellcheck.yml/badge.svg)
![Latest Version](https://img.shields.io/badge/Latest%20Version-1.0.0-blue?logo=Espressif)
![GitHub repo size](https://img.shields.io/github/repo-size/nthnn/Jessy-OS?logo=git&label=Repository%20Size)
[![License: CERN-OHL SV2](https://img.shields.io/badge/License-CERN%20OHL%20SV2-yellow.svg)](https://ohwr.org/cern_ohl_s_v2.pdf)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Jessy OS is an embedded operating system designed for the ESP32 microcontroller. It provides a versatile platform for various IoT and embedded systems projects, with features like a file system, real-time clock support, networking capabilities, user accounts, and JavaScript execution via the embedded Duktape VM.

<p align="center">
    <img src="assets/demo.gif" width="100%" />
</p>

## Table of Contents

* [Features](#features)
* [Documentations](#documentations)
* [Contributions](#contributions)
    - [Issue Reporting](#issue-reporting)
    - [Pull Requests](#pull-requests)
* [Contributors](#contributors)
* [To-do](#to-do)
* [Known Bugs](#known-bugs)
* [License](#license)

## Features

- **File System**: Jessy OS includes its own file system, allowing you to manage and access files on the ESP32 device.
- **Real-Time Clock (RTC)**: It supports the DS1307 real-time clock via I2C, enabling accurate timekeeping for your applications.
- **User Accounts**: You can create and manage multiple user accounts, making it suitable for applications requiring user authentication.
- **Networking**: Jessy OS includes networking functionality with a WLAN command for managing Wi-Fi connections and network configurations.
- **JavaScript Support**: The embedded Duktape VM allows you to execute JavaScript programs, expanding the range of applications you can build.

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

1. Improve the available APIs on the JavaScript Duktape VM.
2. Terminal command for editing files.
3. Update documentations for version 1.1.0.

## Known Bugs

1. The `wget` command only works on HTTP non-secure hosts/targets.

    (See line [#957](#) of [src/jessy_terminal.cpp](src/jessy_terminal.cpp))

## License

Jessy OS has two (2) license. The hardware itself (including the PCB Gerber files and the schematic diagram) is under the CERN Open Hardware Licence Version 2 - Strongly Reciprocal, while the Jessy OS repository as a whole is under MIT Public License.

Both licenses have distinct characteristics and implications.

The CERN Open Hardware License (CERN OHL) is designed to facilitate the sharing and collaboration of open hardware designs. The CERN v2 Strong license places an emphasis on ensuring modifications to the original hardware design remain open and under the same license. It promotes the sharing of derivative works and requires contributors to provide proper attribution.

The MIT License is one of the most permissive open-source licenses. It allows others to use, modify, distribute, and even incorporate your code into proprietary projects. Users of MIT-licensed software typically have fewer restrictions, making it a widely adopted license for software libraries and components.