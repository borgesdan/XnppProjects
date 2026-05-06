# Xnpp (XN++) Sample Projects
Example projects for the **Xnpp (XN++)** library.
[https://github.com/borgesdan/xnpp](https://github.com/borgesdan/xnpp)

These projects were created using the **Visual Studio 2022 CMake template**.

## Technologies Used
* **C++ 20** - Core language
* **CMake** - Build system
* **vcpkg** - Package manager

The `/ports` directory is responsible for providing access to **xnpp** on GitHub for **vcpkg**.

---

## Build Instructions

### Using Visual Studio 2022
1.  In the root `CMakeLists.txt`, set `ON` for the projects you wish to build.
2.  Go to the **Project** menu > **Configure XnppProjects** (if it hasn't happened automatically).
3.  Go to the **Build** menu > **Build All** or build a specific executable.
4.  Select the project in the **Startup Item** dropdown.
5.  Go to the **Debug** menu > **Start Debugging** (F5).

### Using the Terminal (Command Line)
To build the projects via the command line, follow these steps:

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/borgesdan/xnpp-samples.git](https://github.com/borgesdan/xnpp-samples.git)
    cd xnpp-samples
    ```

2.  **Generate build files:**
    Ensure you specify the path to your `vcpkg` toolchain file so CMake can find the dependencies:
    ```bash
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake
    ```

3.  **Compile the projects:**
    ```bash
    cmake --build build --config Debug
    ```

4.  **Run an example:**
    The compiled binaries will be located inside the `build` folder.
    ```bash
    ./build/ProjectName
    ```

---

## License
This project is licensed under the **MIT License**.