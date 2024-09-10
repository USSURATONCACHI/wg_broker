cmake_minimum_required(VERSION 3.10)

project(wg_broker VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)

add_definitions(-DSIMPPL_HAVE_INTROSPECTION=1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/")
find_package(DBUS REQUIRED)
find_package(Simppl REQUIRED)

include_directories(${Simppl_INCLUDE_DIRS} ${DBUS_INCLUDE_DIRS})

add_executable(wg_broker
    src/main.cpp
    src/broker_impl.cpp
)
target_include_directories(wg_broker PUBLIC include)
target_link_libraries(wg_broker simppl ${DBUS_LIBRARIES})