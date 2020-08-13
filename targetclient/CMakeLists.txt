cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(targetclient LANGUAGES CXX)

include(GNUInstallDirs)

set (GENERIC_VERSION_MAJOR 0)
set (GENERIC_VERSION_MINOR 1)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

# find Qt5 and required components
find_package(Threads REQUIRED)
find_package(CFITSIO REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets Network)
find_package(INDI COMPONENTS client REQUIRED)

# need this to include the auto generated ui_mainwindow.h
# file from the qt5_wrap_ui call below.
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

include_directories( ${INDI_INCLUDE_DIR})


set(project_sources mainwindow.cpp main.cpp indiclient.cpp dialogprofile.cpp profile.cpp handboxreceiver.cpp )
set(project_headers mainwindow.h indiclient.h dialogprofile.h profile.h handboxreceiver.h )
set(project_ui mainwindow.ui dialogprofile.ui )

# wrap the ui file to a c++ header
qt5_wrap_ui(ui_wrap ${project_ui})
qt5_wrap_cpp(moc_sources ${project_headers})
add_executable(TargetClient 
	${project_sources} 
	${project_headers}
	${ui_wrap} 
	${moc_sources})
target_link_libraries(TargetClient 
	PUBLIC
	Qt5::Core
	Qt5::Gui 
        Qt5::Widgets
        Qt5::Network
        ${INDI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CFITSIO_LIBRARIES} m ${ZLIB_LIBRARY})


