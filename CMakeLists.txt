cmake_minimum_required(VERSION 3.15..4.0)

project(AOC VERSION 1.0
            DESCRIPTION "Advent of Code"
            LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

find_package(Eigen3 3.4 REQUIRED)

add_subdirectory(lib)

# each year directory
file(GLOB YEARS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 20[0-9][0-9])

foreach(year ${YEARS})
  if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${year})
    file(GLOB DAYS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${year} ${year}/day*)
    message(STATUS "Year ${year} days: ${DAYS}")

    foreach(day ${DAYS})
      set(SRC_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${year}/${day}/solution.cpp)

      if(EXISTS ${SRC_FILE})
          set(EXEC_TARGET "${year}${day}")
          add_executable(${EXEC_TARGET} ${SRC_FILE})
          target_link_libraries(${EXEC_TARGET} PRIVATE lib)
          set_target_properties(${EXEC_TARGET} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${year}/bin
            OUTPUT_NAME ${day}
          )
        endif()

        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${year}/${day}/CMakeLists.txt")
          add_subdirectory(${year}/${day} ${year}/${day})
        else()

      endif()

    endforeach()

  endif()
endforeach()
