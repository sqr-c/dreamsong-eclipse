set(CMAKE_VERBOSE_MAKEFILE ON)

project(eclipse)

set(PROJECT_SRC_DIR "src/")
set(PROJECT_INC_DIR "include/")

file(GLOB PROJECT_SRC "${PROJECT_SRC_DIR}*.c")
file(GLOB PROJECT_INC "${PROJECT_INC_DIR}*.h")

include_directories(${PROJECT_INC_DIR})

add_definitions(-DSDL_SDL_INCLUDE="<SDL2/SDL.h>")
add_definitions(-DUNIX)

add_executable(${PROJECT_NAME} ${PROJECT_SRC})

if(BUILD STREQUAL "i")
	file(GLOB STATIC_DATA "static/*")

	set(PROJECT_STATIC_DIRECTORY /usr/share/eclipse/)
	set(PROJECT_BIN_DIRECTORY /bin/)

	add_definitions(-DPATH_STATIC_PREFIX=\\"${PROJECT_STATIC_DIRECTORY}\\")
	add_definitions(-DPATH_DYNAMIC_PREFIX=\\"/home/~/eclipse/\\")

	file(MAKE_DIRECTORY ~/eclipse)

	install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_BIN_DIRECTORY})
	install(FILES ${STATIC_DATA} DESTINATION ${PROJECT_STATIC_DIRECTORY})
endif()

target_link_libraries(${PROJECT_NAME} SDL2 SDL2main m)
