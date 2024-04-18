
cmake_minimum_required(VERSION 3.25.1)
project("NOP - Processing() Multithreading Test")

set(CMAKE_CXX_STANDARD 11)

include_directories(
	"./components/ProcessingCore"
	"./components/ProcessingCommon"
	"./main"
)

add_definitions(
	"-DCONFIG_PROC_HAVE_LOG=1"
	"-DCONFIG_PROC_HAVE_DRIVERS=1"
	"-DCONFIG_PROC_INFO_BUFFER_SIZE=1024"
)

add_executable(
	app
	components/ProcessingCore/Processing.cpp
	components/ProcessingCore/Log.cpp
	components/ProcessingCore/SystemDebugging.cpp
	components/ProcessingCore/SystemCommanding.cpp
	components/ProcessingCore/TcpListening.cpp
	components/ProcessingCore/TcpTransfering.cpp
	components/ProcessingCommon/ThreadPooling.cpp
	components/ProcessingCommon/LibDriverPlatform.cpp
	main/main.cpp
	main/AppSupervising.cpp
	main/MultiThreading.cpp
	main/SizeStackPrinting.cpp
)

if (UNIX)
	target_sources(app PRIVATE components/ProcessingCommon/LibFilesys.cpp)
endif()

if (MSVC)
	set (CMAKE_CXX_FLAGS "/EHsc")
endif()

if (WIN32)
	add_definitions(
		"-D_WIN32_WINNT=_WIN32_WINNT_WIN10"
		"-DWINVER=_WIN32_WINNT_WIN10"
	)
	target_link_libraries(
		app
		"ws2_32"
	)
endif()

