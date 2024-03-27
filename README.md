# CryptIT

<img src="src/assets/logo_big.png" width="50%">

## Description

This is a Qt application that uses the Crypto++ library, allowing users to select a directory and perform various cryptographic operations. The application utilizes QMainWindow for the main window and QVBoxLayout for the layout.

The application comprises a section for cryptographic operations (`Crypto`), a button to select a directory, and a label to display the selected directory. When the directory select button is clicked, a file dialog opens for the user to choose a directory. Subsequently, the selected directory path is passed to the `Crypto` section, and the GUI updates to reflect the new directory.

## How to Run

### Build Localy
1. Clone the repository.
2. Ensure you have Conan 2.0 installed and an up-to-date MSVC compiler.
3. Run `build.sh` to build it for release and `debug_build.sh` to build it for debug.
4. Run CMake configuration by pressing `Ctrl + Shift + P`, then search for `CMake: configure` and select the release or debug kit depending on your previous choice.
5. Run CMake build by pressing `Ctrl + Shift + P`, then search for `CMake: build`, or simply press `F7` on the keyboard.
6. If you're using VS Code, select the appropriate launch option. Otherwise, navigate to the build folder located at `build/vs2022-{deb/rel}/src/{Debug/Release}/CryptIT.exe`.

### Downlaod binary
Go to [releases](https://github.com/Forsrobin/CryptIT/releases) and download the latest version depending on your operating system.

## Dependencies

- Qt/6.6.2
- Crypto++/8.9.0
- gtest/1.14.0

## License

[MIT](LICENSE)
