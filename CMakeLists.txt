cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_STANDARD 99)
project(CGui)

include(FetchContent)

FetchContent_Declare(
	raylib
	GIT_REPOSITORY https://github.com/raysan5/raylib.git
	EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(raylib)

add_executable(CGui main.c
	graphics_imp.c

	src/tree.c
	src/container.c
	src/button.c
	src/label.c
	src/drawing.c
	src/utils.c
	src/context.c
)

# if(MSVC)
# 	target_compile_options(CGui PRIVATE /W4 /Wall)
# else()
# 	target_compile_options(CGui PRIVATE -Wall -Wextra -Wpedantic)
# endif()

target_include_directories(CGui PUBLIC platform/windows)

target_link_libraries(CGui PRIVATE raylib)

target_include_directories(CGui PRIVATE
	${raylib_SOURCE_DIR}
	include
)
target_compile_definitions(CGui PRIVATE CGUI_BUILD_DLL)