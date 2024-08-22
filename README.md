# C Data Strues

## Description

A library of data structures in C emulating part of the C++ standard library,
written for personal use.

This repository has only been tested with MSVC on Windows 10.

## Dependencies

- CMake 3.22

## Build and Install

This repository's MSVC debug build is configured to work with Visual Studio's
address sanitiser. Hence, when building this repository with MSVC on debug mode,
the following paths must be included in the shell's Path environment variable
for CMake to build properly:

- clang_rt.asan_dbg_dynamic-x86_64.dll
- clang_rt.asan_dynamic-x86_64.dll

```powershell
mkdir build;
cd build;
cmake ..;
cmake --build . --config Release;

cmake --install . #Run this line with administrative rights.
```

## Testing

Unit tests are available under CTest. To run the tests, navigate to the build
folder and type either:

```powershell
# if built in Debug mode
ctest -C Debug
# if built in Release mode
ctest -C Release
```

## LICENSE

This project is licensed under the MIT License - see the
[LICENSE](LICENSE) file.
