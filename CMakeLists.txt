# The minimum required version hasn't been tested. Feel free to adjust
# downwards if necessary.
cmake_minimum_required(VERSION 3.0)
project(hello-mysql-world C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

include(FindPkgConfig)
pkg_check_modules(LIBMYSQLCLIENT REQUIRED mysqlclient)

foreach(FLAG ${LIBMYSQLCLIENT_CFLAGS_OTHER})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
endforeach()

link_directories(${LIBMYSQLCLIENT_LIBRARY_DIRS})

add_executable(hello hello.c)
target_include_directories(hello PRIVATE ${LIBMYSQLCLIENT_INCLUDE_DIRS})
target_link_libraries(hello PRIVATE ${LIBMYSQLCLIENT_LIBRARIES})
