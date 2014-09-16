#using bundler exe. file with windows
cmake_minimum_required(VERSION 3.0)

project(BundlerInWin)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(Qt5 REQUIRED)
find_package(OpenCV REQUIRED)

file(GLOB ALL_SOURCES
	bundlerinwin.*
	main.cpp
	)
	
set(MISTUDIO_HEADERS
	bundlerinwin.h
	)
set(MISTUDIO_UIS
	bundlerinwin.ui
	)
set(MISTUDIO_QRCS bundlerinwin.qrc)

QT5_WRAP_CPP (MISTUDIO_HEADERS_MOC ${MISTUDIO_HEADERS})
QT5_WRAP_UI (MISTUDIO_UIS_UIC ${MISTUDIO_UIS})
QT5_ADD_RESOURCES (MISTUDIO_QRC_RCC ${MISTUDIO_QRCS})

add_executable(BundlerInWin
	${ALL_SOURCES}
	${MISTUDIO_HEADERS_MOC}
	${MISTUDIO_UIS_UIC}
	${MISTUDIO_QRC_RCC}
	bundlerinwin.qrc
	)
	
target_link_libraries(BundlerInWin
	Qt5::Core
	Qt5::Widgets
	Qt5::Gui
	${OpenCV_LIBS}
	)