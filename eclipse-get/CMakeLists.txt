set(CMAKE_VERBOSE_MAKEFILE ON)

project(eclipse-get)

set(PROJECT_SRC_DIR "src/")
set(PROJECT_INC_DIR "include/")
set(SUBPROJECT_OBJ_DIR "../eclipse/build/CMakeFiles/eclipse.dir/src/")

file(GLOB PROJECT_SRC "${PROJECT_SRC_DIR}*.c")
file(GLOB PROJECT_INC "${PROJECT_INC_DIR}*.h")
file(GLOB SUBPROJECT_OBJ
"${SUBPROJECT_OBJ_DIR}arena.o"
"${SUBPROJECT_OBJ_DIR}breaktile.o"
"${SUBPROJECT_OBJ_DIR}breaktilefn.o"
"${SUBPROJECT_OBJ_DIR}cachefn.o"
"${SUBPROJECT_OBJ_DIR}cast.o"
"${SUBPROJECT_OBJ_DIR}castfn.o"
"${SUBPROJECT_OBJ_DIR}character.o"
"${SUBPROJECT_OBJ_DIR}debug.o"
"${SUBPROJECT_OBJ_DIR}dglobal.o"
"${SUBPROJECT_OBJ_DIR}display.o"
"${SUBPROJECT_OBJ_DIR}displayctrl.o"
"${SUBPROJECT_OBJ_DIR}dmapgen.o"
"${SUBPROJECT_OBJ_DIR}dmtable.o"
"${SUBPROJECT_OBJ_DIR}dynamicmap.o"
#"${SUBPROJECT_OBJ_DIR}eclipse.o"
"${SUBPROJECT_OBJ_DIR}itemcache.o"
"${SUBPROJECT_OBJ_DIR}itemcachefn.o"
"${SUBPROJECT_OBJ_DIR}iterfn.o"
"${SUBPROJECT_OBJ_DIR}keybinding.o"
"${SUBPROJECT_OBJ_DIR}map.o"
"${SUBPROJECT_OBJ_DIR}maploader.o"
"${SUBPROJECT_OBJ_DIR}market.o"
"${SUBPROJECT_OBJ_DIR}mathfunction.o"
"${SUBPROJECT_OBJ_DIR}npc.o"
"${SUBPROJECT_OBJ_DIR}npcfn.o"
"${SUBPROJECT_OBJ_DIR}package.o"
"${SUBPROJECT_OBJ_DIR}park.o"
"${SUBPROJECT_OBJ_DIR}path.o"
"${SUBPROJECT_OBJ_DIR}save.o"
"${SUBPROJECT_OBJ_DIR}spritearray.o"
"${SUBPROJECT_OBJ_DIR}text.o"
"${SUBPROJECT_OBJ_DIR}tilefn.o"
"${SUBPROJECT_OBJ_DIR}tscreenobj.o"
"${SUBPROJECT_OBJ_DIR}usermap.o"
)

if(${CMAKE_SYSTEM} MATCHES "Linux")
	add_definitions(-DUNIX)
endif()

include_directories(${PROJECT_INC_DIR})

if(BUILD STREQUAL "i")
	set(INSTALL_USER $ENV{USER})
	set(INSTALL_USER_HOME /home/$ENV{USER})

	set(PROJECT_INSTALL_BIN_DIR /bin/)
	set(PROJECT_INSTALL_STATIC_DIR /usr/share/eclipse/)
	set(PROJECT_INSTALL_DYNAMIC_DIR ${INSTALL_USER_HOME}/.eclipse/)
	add_definitions(
		#-DPATH_D_MAIN_PREFIX=\\"${PROJECT_INSTALL_DYNAMIC_DIR}\\"
		-DEID_SEARCH_DIRS=\\"${INSTALL_USER_HOME}/Downloads/\\",\\"${INSTALL_USER_HOME}/.eclipse/\\",\\"/tmp\\"
	)

	add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${SUBPROJECT_OBJ})
	install(TARGETS ${PROJECT_NAME} DESTINATION ${PROJECT_INSTALL_BIN_DIR})
else()
	add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${SUBPROJECT_OBJ})
	add_definitions(-DPATH_D_MAIN_PREFIX=\\"../../eclipse/dynamic/\\")
endif()

target_link_libraries(${PROJECT_NAME} SDL2 SDL2main m)
