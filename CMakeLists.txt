cmake_minimum_required(VERSION 2.8)

project(customui.lv2)

include_directories(${PROJECT_SOURCE_DIR})

set(CMAKE_C_FLAGS "-std=gnu11 -Wextra -Wno-unused-parameter -ffast-math -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "-Wshadow -Wimplicit-function-declaration -Wmissing-prototypes -Wstrict-prototypes ${CMAKE_C_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,-z,nodelete -Wl,-z,defs ${CMAKE_MODULE_LINKER_FLAGS}")

set(CUSTOMUI_MAJOR_VERSION 0)
set(CUSTOMUI_MINOR_VERSION 1)
set(CUSTOMUI_MICRO_VERSION 1)
set(CUSTOMUI_VERSION "${CUSTOMUI_MAJOR_VERSION}.${CUSTOMUI_MINOR_VERSION}.${CUSTOMUI_MICRO_VERSION}")

set(DEST lib/lv2/customui.lv2)

find_package(PkgConfig) # ${PKG_CONFIG_FOUND}

set(LIBS m) # math library

pkg_search_module(LV2 REQUIRED lv2>=1.10)
include_directories(${LV2_INCLUDE_DIRS})

add_library(customui MODULE
	test.c)
set_target_properties(customui PROPERTIES PREFIX "")
install(TARGETS customui DESTINATION ${DEST})

add_library(customui_ui MODULE
	test_ui.c)
set_target_properties(customui_ui PROPERTIES PREFIX "")
install(TARGETS customui_ui DESTINATION ${DEST})

configure_file(${PROJECT_SOURCE_DIR}/manifest.ttl.in
	${PROJECT_BINARY_DIR}/manifest.ttl)
install(FILES ${PROJECT_BINARY_DIR}/manifest.ttl DESTINATION ${DEST})
install(FILES ${PROJECT_SOURCE_DIR}/customui.ttl DESTINATION ${DEST})
install(FILES ${PROJECT_SOURCE_DIR}/customui_ui.ttl DESTINATION ${DEST})
