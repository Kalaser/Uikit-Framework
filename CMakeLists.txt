cmake_minimum_required(VERSION 3.20)

project(uikit VERSION 0.3.0 LANGUAGES C CXX)

# ── Options ──────────────────────────────────────────────────────────────────
option(UIKIT_BUILD_DEMO     "Build the demo application" ON)
option(UIKIT_BUILD_TESTS    "Build the test suite"        ON)
option(UIKIT_BUILD_EXAMPLES "Build the examples"          ON)

# ── LVGL ─────────────────────────────────────────────────────────────────────
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/lvgl ${CMAKE_BINARY_DIR}/lvgl)

# ── UIKit library ────────────────────────────────────────────────────────────
add_library(uikit STATIC
    # Framework umbrella
    uikit/UIKit.h
    uikit/UIKit.c

    # Core
    uikit/core/UIKitResult.h
    uikit/core/UIKitResult.c
    uikit/core/UIEvent.h
    uikit/core/UIView.h
    uikit/core/UIView.c
    uikit/core/UIViewRoot.h
    uikit/core/UIViewRoot.c

    # Components
    uikit/components/UILabel.h
    uikit/components/UILabel.c
    uikit/components/UIButton.h
    uikit/components/UIButton.c
    uikit/components/UISwitch.h
    uikit/components/UISwitch.c
    uikit/components/UISlider.h
    uikit/components/UISlider.c
    uikit/components/UITextField.h
    uikit/components/UITextField.c
    uikit/components/UIImageView.h
    uikit/components/UIImageView.c
    uikit/components/UIScrollView.h
    uikit/components/UIScrollView.c
    uikit/components/UIProgressView.h
    uikit/components/UIProgressView.c
    uikit/components/UIPageControl.h
    uikit/components/UIPageControl.c

    # Controllers
    uikit/controllers/UIViewController.h
    uikit/controllers/UIViewController.c
    uikit/controllers/UINavigationController.h
    uikit/controllers/UINavigationController.c

    # Graphics (theme + animation)
    uikit/graphics/UITheme.h
    uikit/graphics/UITheme.c
    uikit/graphics/UIViewAnimation.h
    uikit/graphics/UIViewAnimation.c

    # Layout
    uikit/layout/UIStackView.h
    uikit/layout/UIStackView.c

    # C++ wrapper (header-only)
    uikit/cpp/UIKit.hpp

    # Platform
    uikit/platform/UIKitPlatform.h
    uikit/platform/UIKitPlatform.c
)

target_include_directories(uikit PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit/core
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit/components
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit/graphics
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit/layout
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit/cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uikit/platform
)

target_link_libraries(uikit PUBLIC lvgl)

# ── Demo app ─────────────────────────────────────────────────────────────────
if(UIKIT_BUILD_DEMO)
    add_executable(UIKitDemo apps/demo/main.c)
    target_link_libraries(UIKitDemo PRIVATE uikit lvgl)
endif()

# ── Launcher app (Android-style desktop on Windows GDI) ──────────────────────
if(WIN32)
    add_executable(UIKitLauncher
        apps/launcher/launcher.cpp
        apps/launcher/app_common.h
        apps/launcher/app_factory.h
        apps/launcher/app_registry.cpp
        apps/launcher/win_driver.h
        apps/launcher/win_driver.c
        apps/launcher/apps/phone/phone_app.cpp
        apps/launcher/apps/camera/camera_app.cpp
        apps/launcher/apps/gallery/gallery_app.cpp
        apps/launcher/apps/music/music_app.cpp
        apps/launcher/apps/weather/weather_app.cpp
        apps/launcher/apps/maps/maps_app.cpp
        apps/launcher/apps/clock/clock_app.cpp
        apps/launcher/apps/calc/calc_app.cpp
        apps/launcher/apps/files/files_app.cpp
        apps/launcher/apps/mail/mail_app.cpp
        apps/launcher/apps/store/store_app.cpp
        apps/launcher/apps/video/video_app.cpp
        apps/launcher/apps/settings/settings_app.cpp
    )
    target_link_libraries(UIKitLauncher PRIVATE uikit lvgl user32 gdi32)
endif()

# ── Tests ────────────────────────────────────────────────────────────────────
if(UIKIT_BUILD_TESTS)
    enable_testing()
    add_executable(UIKitTests tests/test_uikit.c)
    target_link_libraries(UIKitTests PRIVATE uikit lvgl)
    add_test(NAME UIKitTests COMMAND UIKitTests)

    add_executable(UIKitCppTests tests/test_uikit_cpp.cpp)
    target_link_libraries(UIKitCppTests PRIVATE uikit lvgl)
    add_test(NAME UIKitCppTests COMMAND UIKitCppTests)
endif()

# ── Examples ─────────────────────────────────────────────────────────────────
if(UIKIT_BUILD_EXAMPLES)
    add_executable(UIKitExampleBasic      examples/basic/basic.c)
    add_executable(UIKitExampleNavigation examples/navigation/navigation.c)
    add_executable(UIKitExampleEvents     examples/events/events.c)
    add_executable(UIKitCppExample        examples/cpp/basic.cpp)
    target_link_libraries(UIKitExampleBasic      PRIVATE uikit lvgl)
    target_link_libraries(UIKitExampleNavigation PRIVATE uikit lvgl)
    target_link_libraries(UIKitExampleEvents     PRIVATE uikit lvgl)
    target_link_libraries(UIKitCppExample        PRIVATE uikit lvgl)
endif()
