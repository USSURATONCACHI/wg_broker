cmake_minimum_required(VERSION 3.10)

project(wg_broker VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DSIMPPL_HAVE_INTROSPECTION=1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

include_directories(${DBUS_INCLUDE_DIRS})

add_executable(wg_broker
    src/main.cpp
    src/dbus_connection.cpp
    src/owned_bus_name.cpp
)
target_include_directories(wg_broker PUBLIC include)
target_link_libraries(wg_broker ${DBUS_LIBRARIES})
target_compile_options(wg_broker PRIVATE ${DBUS_CFLAGS_OTHER})
