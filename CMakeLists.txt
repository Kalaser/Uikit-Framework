cmake_minimum_required(VERSION 3.20)

project(uikit VERSION 0.3.0 LANGUAGES C CXX)

# ── Options ──────────────────────────────────────────────────────────────────
option(UIKIT_BUILD_DEMO     "Build the demo application" ON)
option(UIKIT_BUILD_TESTS    "Build the test suite"        ON)
option(UIKIT_BUILD_EXAMPLES "Build the examples"          ON)
option(UIKIT_BUILD_LAUNCHER "Build the Windows launcher application" ${WIN32})

# ── LVGL ─────────────────────────────────────────────────────────────────────
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/third_party/lvgl/CMakeLists.txt")
    message(FATAL_ERROR
        "LVGL is unavailable. Fetch repository dependencies with: "
        "git submodule update --init --recursive")
endif()
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
if(UIKIT_BUILD_LAUNCHER)
    if(NOT WIN32)
        message(FATAL_ERROR
            "UIKitLauncher uses the Windows GDI driver. Configure with a Windows "
            "toolchain, or set -DUIKIT_BUILD_LAUNCHER=OFF on non-Windows hosts.")
    endif()
    # XML page compiler (route B): when Python is available the build
    # auto-regenerates the *_xml.c/h from res/pages/*.xml whenever the XML
    # changes (edit XML -> just `make`). Without Python, the checked-in
    # generated files are used as-is (re-run tools/xml2c.py manually).
    set(UIKIT_XML2C ${CMAKE_SOURCE_DIR}/tools/xml2c.py)
    set(UIKIT_XML_PAGES
        ${CMAKE_SOURCE_DIR}/apps/launcher/res/pages/settings.xml
        ${CMAKE_SOURCE_DIR}/apps/launcher/res/pages/terminal.xml
    )
    set(UIKIT_XML_GENERATED)
    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        foreach(xml ${UIKIT_XML_PAGES})
            get_filename_component(xml_name ${xml} NAME_WE)
            set(xml_out_c ${CMAKE_SOURCE_DIR}/apps/launcher/apps/${xml_name}_xml/${xml_name}_xml.c)
            set(xml_out_h ${CMAKE_SOURCE_DIR}/apps/launcher/apps/${xml_name}_xml/${xml_name}_xml.h)
            add_custom_command(
                OUTPUT ${xml_out_c} ${xml_out_h}
                COMMAND ${Python3_EXECUTABLE} ${UIKIT_XML2C} ${xml} ${xml_out_c} ${xml_out_h}
                DEPENDS ${xml} ${UIKIT_XML2C}
                COMMENT "xml2c: ${xml_name}.xml -> ${xml_name}_xml.c/h"
            )
            list(APPEND UIKIT_XML_GENERATED ${xml_out_c})
        endforeach()
        add_custom_target(xml_pages DEPENDS ${UIKIT_XML_GENERATED})
    endif()
    if(NOT UIKIT_XML_GENERATED)
        # No Python available: use the checked-in generated sources as-is.
        set(UIKIT_XML_GENERATED
            ${CMAKE_SOURCE_DIR}/apps/launcher/apps/settings_xml/settings_xml.c)
    endif()

    add_executable(UIKitLauncher
        apps/launcher/launcher.cpp
        apps/launcher/app_common.h
        apps/launcher/activity_registry.h
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
        ${UIKIT_XML_GENERATED}
        apps/launcher/apps/settings_xml/settings_xml_app.cpp
        apps/launcher/apps/terminal_xml/terminal_app.cpp
        apps/launcher/system/drawer.cpp
        apps/launcher/system/recents.cpp
        apps/launcher/system/quicksettings.cpp
        apps/launcher/system/app_menu.cpp
    )
    if(TARGET xml_pages)
        add_dependencies(UIKitLauncher xml_pages)
    endif()
    target_link_libraries(UIKitLauncher PRIVATE uikit lvgl user32 gdi32)
    install(TARGETS UIKitLauncher RUNTIME DESTINATION bin)
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

# ── Release package ──────────────────────────────────────────────────────────
# On Windows, `cpack -G ZIP` produces a portable launcher archive containing
# `bin/UIKitLauncher.exe`. The launcher only links against Windows system DLLs.
set(CPACK_PACKAGE_NAME "UIKitLauncher")
set(CPACK_PACKAGE_VENDOR "UIKit-Framework")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "UIKitLauncher-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_GENERATOR "ZIP")
include(CPack)

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
