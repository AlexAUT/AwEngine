CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(FetchContent)

include(cmake/color.cmake)

function(loadDependency name url tag)
  FetchContent_Declare(
    ${name}
    GIT_REPOSITORY ${url}
    GIT_TAG ${tag}
    GIT_PROGRESS ON
  )

  FetchContent_GetProperties(${name})
  if (NOT ${name}_POPULATED)
    FetchContent_Populate(${name})
    add_subdirectory(${${name}_SOURCE_DIR} ${${name}_BINARY_DIR})
  endif()
endfunction()

set(FETCHCONTENT_QUIET OFF)

set(SPDLOG_BUILD_SHARED ON)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_EXAMPLE OFF)
loadDependency(spdlog https://github.com/gabime/spdlog v1.4.2)

set(SFML_BUILD_NETWORK OFF)
set(SFML_BUILD_GRAPHICS OFF)
set(SFML_BUILD_NETWORK OFF)
loadDependency(sfml https://github.com/sfml/sfml 2.5.1)

set(GLM_TEST_ENABLE OFF)
loadDependency(glm https://github.com/g-truc/glm 0.9.9.6)

project(awEngine)

add_library(${PROJECT_NAME})

add_subdirectory(src/aw/ecs)
add_subdirectory(src/aw/engine)
add_subdirectory(src/aw/util)

find_package(OpenGL)

target_sources(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/aw/util/log.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/_deps/glm-src)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog sfml-window sfml-system OpenGL::GL)

set(AW_BUILD_TESTS OFF CACHE BOOL "Santizer option to use")
if (AW_BUILD_TESTS)
  set(AW_TEST_SOURCES
    tests/test_runner.cpp
    tests/ecs/test_componentStorage.cpp
    #tests/ecs/test_systems.cpp
    tests/ecs/test_view.cpp
    tests/ecs/test_world.cpp
  )

add_executable(awTests EXCLUDE_FROM_ALL ${AW_TEST_SOURCES})
  target_link_libraries(awTests PRIVATE ${PROJECT_NAME})
endif()
