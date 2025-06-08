cmake_minimum_required(VERSION 3.5)

project(dicom_client LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# specify that we have a config file. This is required, since DCMTK package
# uses /usr/include/dcmtk/config/cfunix.h to setup dcmtk. The file is only loaded by
# DCMTK's /usr/include/dcmtk/config/osconfig.h when HAVE_CONFIG_H is set.
add_definitions(-DHAVE_CONFIG_H)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(DCMTK REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        dicomreader.cpp
        dicomreader.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

# specify DCMTK header include directories
include_directories(${DCMTK_INCLUDE_DIRS})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(dicom_client
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(dicom_client SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(dicom_client
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(dicom_client PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${DCMTK_LIBRARIES})
