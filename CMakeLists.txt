cmake_minimum_required(VERSION 3.20)
project(CGui)

include(FetchContent)

FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(raylib)

add_executable(CGui main.c
	src/tree.c
	src/panel.c
	src/button.c
	src/drawing.c
)

target_include_directories(CGui PUBLIC platform/windows)

target_link_libraries(CGui PRIVATE raylib)

target_include_directories(CGui PRIVATE
	${raylib_SOURCE_DIR}
	include
)