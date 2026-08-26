# Building a launcher release

`UIKitLauncher` has a Windows GDI display and input backend. Build it with a
Windows toolchain and make sure the LVGL submodule is present:

```sh
git clone --recurse-submodules <repository-url>
cmake --preset launcher-release
cmake --build --preset launcher-release
cpack --config build/launcher-release/CPackConfig.cmake -G ZIP
```

The final archive is named `UIKitLauncher-<version>-Windows.zip` and includes
`bin/UIKitLauncher.exe`.

On a non-Windows host, leave `UIKIT_BUILD_LAUNCHER` disabled. If a launcher
build is requested without a Windows toolchain, CMake terminates with a clear
message instead of attempting to compile the GDI backend.
