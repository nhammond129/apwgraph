cmake_minimum_required(VERSION 3.31.5)
project(apwgraph)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)



## get external dependencies
add_subdirectory("extern/")

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
)

