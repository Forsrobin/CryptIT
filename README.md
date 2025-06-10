# CryptIT

<img src="src/assets/logo_big.png" width="50%">

## Why?
I decided to do this project to learn more about bigger c++ project and how to develop, maintain and create unit tests and pipelines. 

## Description
This is a Qt application that uses the Crypto++ library, allowing users to select a directory and perform various cryptographic operations. The application utilizes QMainWindow for the main window and QVBoxLayout for the layout.
CryptIT utilizes GitHub Actions to automatically run unit tests on pull requests to the main branch, ensuring code integrity and reliability throughout development. This streamlined process enhances collaboration and maintains high standards of software quality and security, so feel free to contribute as much as you want and can!

## How to use
To use the program, you will need to open the executable and select a directory that you either want to encrypt or decrypt. Once a directory is selected, you can choose the number of threads the program should use; the more threads selected, the faster the process. Afterward, you can press the "Load files" button to view an overview of the number of files and a small sample of the selected files that will either be decrypted or encrypted. The program includes checks to ensure you cannot encrypt already encrypted files and verifies the correctness of the password before decrypting the files. Once a password is entered, only one button, either "encrypt" or "decrypt," will be available depending on whether the files are already encrypted or decrypted. It's up to you

## Available presets
<div id="available-presets" />

| Preset     | Os      | Info                             | Prerequsite                                                  |
|:-----------|:--------|:---------------------------------|:-------------------------------------------------------------|
| vs2022-rel | Windows | Compiles Release build with MSVC | [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) or newer (not tested with older versions) |
| vs2022-deb | Windows | Compiles Debug build with MSVC   | [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) or newer (not tested with older versions) |

## How to Run
### Build locally
1. Clone the repository.
2. Ensure you have Conan 2.0 installed and an up-to-date compiler of your choice.
3. Run `build.sh` followed with an argument `rel` or `deb` to build it of your liking.
4. - Run CMake configuration by pressing `Ctrl + Shift + P`, then search for `CMake: configure` and select the release or debug kit depending on your previous choice.
   - Or run <a href="#available-presets">`cmake --preset {preset}`</a>, se table above for available presets
6. - Run CMake build by pressing `Ctrl + Shift + P`, then search for `CMake: build`, or simply press `F7` on the keyboard.
   - OR run `cmake --build --preset {preset}`
7. If you're using VS Code, select the appropriate launch option. Otherwise, navigate to the build folder located at `build/{preset}/src/{Debug/Release}/CryptIT.exe`.

### Download binary
Go to [releases](https://github.com/Forsrobin/CryptIT/releases) and download the latest version depending on your operating system.

## Dependencies

- Qt/6.6.2
- Crypto++/8.9.0
- gtest/1.14.0

## License
Common
[MIT](LICENSE)
