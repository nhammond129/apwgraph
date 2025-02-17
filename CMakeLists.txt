cmake_minimum_required(VERSION 3.31.5)
project(apwgraph)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

## get external dependencies
add_subdirectory("extern/")

## find pipewire from system
find_package(PkgConfig REQUIRED)
pkg_check_modules(pw REQUIRED libpipewire-0.3>=1.2.7)
include_directories(${pw_INCLUDE_DIRS})

## Create main executable
add_executable(main
	src/main.cpp
)
target_include_directories(main
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/src/"
)
target_link_libraries(
	main
	PRIVATE
		SDL3
		imgui
		${pw_LIBRARIES}
)

