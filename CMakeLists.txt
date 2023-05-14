cmake_minimum_required(VERSION 3.5)

project(LANchat VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Network REQUIRED)

set(PROJECT_SOURCES
        main.cpp

        # Qt forms
        src/mainwindow.h src/mainwindow.cpp src/mainwindow.ui
        src/connectiondialog.h src/connectiondialog.cpp src/connectiondialog.ui
        src/settingsdialog.h src/settingsdialog.cpp src/settingsdialog.ui

        # C++ classes
        src/globaldefs.h src/globaldefs.cpp
        src/argumentprocessor.h src/argumentprocessor.cpp
        app/noguimode.h app/noguimode.cpp
        app/appsettings.h app/appsettings.cpp
        app/filemanager.h
        app/exceptions.h app/exceptions.cpp
        connection/abstracttypes.h connection/abstracttypes.cpp
        connection/client.h connection/client.cpp
        connection/requestpacket.h connection/requestpacket.cpp
        connection/informationpacket.h connection/informationpacket.cpp
        connection/messagepacket.h connection/messagepacket.cpp
        connection/messagetimepair.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LANchat
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET LANchat APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(LANchat SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(LANchat
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(LANchat PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(LANchat PRIVATE Qt${QT_VERSION_MAJOR}::Network)

set_target_properties(LANchat PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER worriedlemon.lanchat.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LANchat)
endif()
