cmake_minimum_required(VERSION 3.16)

project(CGS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets Network PrintSupport SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Network PrintSupport SerialPort)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CGS
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        mavlinkdevice.h mavlinkdevice.cpp
        udpmavlinkdevice.h udpmavlinkdevice.cpp
        attitude.h attitude.cpp
        localpositionned.h localpositionned.cpp
        parameterlist.h parameterlist.cpp parameterlist.ui
        statustext.h statustext.cpp
        logswindow.h logswindow.cpp logswindow.ui
        segmentationchecker.h segmentationchecker.cpp
        segmentmap.h segmentmap.cpp
        globalpositionint.h globalpositionint.cpp
        mavlinkpacketizer.h mavlinkpacketizer.cpp
        qcustomplot.h qcustomplot.cpp
        telemetrywindow.h telemetrywindow.cpp telemetrywindow.ui
        plotter.h plotter.cpp plotter.ui
        plotgroup.h plotgroup.cpp plotgroup.ui
        plot.h plot.cpp
        dataflashparser.h dataflashparser.cpp
        logplotwindow.h logplotwindow.cpp logplotwindow.ui
        logparameterswindow.h logparameterswindow.cpp logparameterswindow.ui
        serialmavlinkdevice.h serialmavlinkdevice.cpp
        serialscanner.h serialscanner.cpp serialscanner.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CGS APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CGS SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CGS
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(CGS PRIVATE ${CMAKE_SOURCE_DIR}/external/c_library_v2)

target_link_libraries(CGS PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::PrintSupport
    Qt${QT_VERSION_MAJOR}::SerialPort
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CGS)
endif()
set_target_properties(CGS PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CGS
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CGS)
endif()
