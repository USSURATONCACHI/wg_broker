cmake_minimum_required(VERSION 3.10)

project(wg_broker VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DSIMPPL_HAVE_INTROSPECTION=1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_package(PkgConfig REQUIRED)
pkg_check_modules(GIO REQUIRED gio-2.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(TOML REQUIRED tomlplusplus)

add_library(generated STATIC
    src/gen/echo_skeleton.c
    src/gen/broker_skeleton.c
)
target_include_directories(generated PUBLIC include/wg_broker/gen/)
target_include_directories(generated PUBLIC ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS})
target_link_libraries(generated ${GLIB_LIBRARIES} ${GIO_LIBRARIES})


add_executable(wg_broker
    src/main.cpp
    src/dbus_connection.cpp
    src/owned_bus_name.cpp
    src/skeleton.cpp
    src/whole_app.cpp
    src/config.cpp
    src/util.cpp

    src/services/echo_service_impl.cpp
    src/services/broker/service_impl.cpp
    src/services/broker/service_util.cpp
    src/services/broker/profile_status.cpp
)
target_include_directories(wg_broker PUBLIC include)
target_include_directories(wg_broker PUBLIC ${GLIB_INCLUDE_DIRS} ${GIO_INCLUDE_DIRS} ${TOML_INCLUDE_DIRS})
target_link_libraries(wg_broker generated ${GLIB_LIBRARIES} ${GIO_LIBRARIES} ${TOML_LIBRARIES})

# Check if you're using GCC version less than 9 and link stdc++fs
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(wg_broker stdc++fs)
endif()