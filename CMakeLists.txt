cmake_minimum_required(VERSION 3.10)

project(wg_broker VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DSIMPPL_HAVE_INTROSPECTION=1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${DBUS_INCLUDE_DIRS})

add_library(generated STATIC
    src/gen/echo_skeleton.c
)
target_include_directories(generated PUBLIC include/wg_broker/gen/)
target_include_directories(generated PUBLIC ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
target_link_libraries(generated ${GLIB_LIBRARIES} ${GIO_LIBRARIES})


add_executable(wg_broker
    src/main.cpp
    src/dbus_connection.cpp
    src/owned_bus_name.cpp
)
target_include_directories(wg_broker PUBLIC include)
target_include_directories(wg_broker PUBLIC ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
target_link_libraries(wg_broker generated ${GLIB_LIBRARIES} ${GIO_LIBRARIES})
