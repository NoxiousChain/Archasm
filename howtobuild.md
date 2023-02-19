# Building and Using C++ Modules

### Important to note:
- All C++ modules must be written within the src/ directory.
- All C++ classes must be registered in NativeLib.cpp
- To build, run build.bat

## Adding C++ modules into GoDot:
1. NativeLib.tres should already exist and refer to godotproject/bin/win64/libgdt2.dll under the Windows 64 category. If not, create this in the filesystem by right-clicking and hitting "New Resource"
2. Create a NativeScript resource in the same way. In the inspector menu, set Class Name to the actual class name, and Library to NativeLib.tres
3. Attach the NativeScript to the existing node by drag and dropping it in
4. If register properties don't exist yet, reopen the project
5. Fill properties in and run