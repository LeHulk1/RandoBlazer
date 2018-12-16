cmake_minimum_required(VERSION 3.12)
project(RandoBlazer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(ASAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
  set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
endif()

include(FetchContent)
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/Orphis/boost-cmake.git
)

FetchContent_GetProperties(Boost)
if(NOT Boost_POPULATED)
  FetchContent_Populate(Boost)
  add_subdirectory(${boost_SOURCE_DIR} ${boost_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_library(randoblazerlib STATIC
  Item.cpp
  Item.h
  Lair.cpp
  Lair.h
  Log.cpp
  Log.h
  Map.cpp
  Map.h
  Random.cpp
  Random.h
  Randomizer.cpp
  Randomizer.h
  ROMCheck.cpp
  ROMCheck.h
  ROMData.cpp
  ROMData.h
  ROMUpdate.cpp
  ROMUpdate.h
  Sprite.cpp
  Sprite.h
  TextUpdate.cpp
  TextUpdate.h
)
target_link_libraries(randoblazerlib PRIVATE Boost::random)

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
  add_executable(randoblazer_emscripten
    Emscripten.cpp
  )
  target_link_libraries(randoblazer_emscripten PRIVATE randoblazerlib)
  set_target_properties(randoblazer_emscripten PROPERTIES
    OUTPUT_NAME randoblazer
  )
else()
  add_executable(randoblazer_cli
    cli.cpp
  )
  target_link_libraries(randoblazer_cli PRIVATE randoblazerlib)
  set_target_properties(randoblazer_cli PROPERTIES
    OUTPUT_NAME randoblazer
  )
endif()

if (ROM)
  message(STATUS "ROM found at: ${ROM}")
  enable_testing()

  add_test(
    NAME portability_check
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/run-test.cmake "-DEXE=$<TARGET_FILE:randoblazer_cli>" "-DROM=${ROM}"
  )
  set_tests_properties(portability_check PROPERTIES
    ENVIRONMENT "EXE=$<TARGET_FILE:randoblazer_cli>;ROM=${ROM}"
  )
endif()