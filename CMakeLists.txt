cmake_minimum_required(VERSION 3.20)
project(SelfC LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Helpful warnings for learning C.
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

file(GLOB_RECURSE C_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Chap*/*.c")

if(NOT C_SOURCES)
  message(FATAL_ERROR "No C sources were found under Chap* directories.")
endif()

foreach(src ${C_SOURCES})
  file(READ "${src}" source_text)
  if(NOT source_text MATCHES "int[ \t\r\n]+main[ \t\r\n]*\\(")
    continue()
  endif()

  file(RELATIVE_PATH rel "${CMAKE_SOURCE_DIR}" "${src}")
  string(REPLACE "/" "_" target "${rel}")
  string(REPLACE ".c" "" target "${target}")

  add_executable(${target} "${src}")
  target_compile_options(${target} PRIVATE -O0 -g)
endforeach()
