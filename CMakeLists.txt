CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(awEngine)
add_library(${PROJECT_NAME})

include(FetchContent)

include(cmake/color.cmake)
include(cmake/loadDependency.cmake)

set(FETCHCONTENT_QUIET OFF)

set(SPDLOG_BUILD_SHARED ON)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_EXAMPLE OFF)
loadDependencyFromGit(spdlog https://github.com/gabime/spdlog v1.4.2)

set(GLM_TEST_ENABLE OFF)
loadDependencyFromGit(glm https://github.com/g-truc/glm 0.9.9.6)
set(GLM_TEST_ENABLE OFF)

loadDependencyFromGit(freetype https://github.com/aseprite/freetype2 VER-2-10-0)

if (NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Android")
#set(SDL_ATOMIC OFF)
#set(SDL_AUDIO OFF)
#set(SDL_CPUINFO OFF)
#set(SDL_DLOPEN OFF)
#set(SDL_FILE OFF)
#set(SDL_FILESYSTEM OFF)
#set(SDL_HAPTIC OFF)
#set(SDL_JOYSTICK OFF)
#set(SDL_LOADSO OFF)
#set(SDL_POWER OFF)
#set(SDL_RENDER OFF)
#set(SDL_SENSOR OFF)
  set(SDL_STATIC OFF)
#set(SDL_TEST OFF)
#set(SDL_THREADS OFF)
#set(SDL_TIMERS OFF)
#set(SDL_OPENGLES OFF)
#set(SDL_RPI OFF)
#set(SDL_VIVANTE OFF)
#set(SDL_VULKAN ON)
set(VIDEO_KMSDRM OFF)
  loadDependencyFromGit(sdl https://github.com/spurious/SDL-mirror.git 1beeec74ae3c32b55966564045009398ead04ec8)
else()
  set(SDL_INC "${ANDROID_NDK}/sources/third_party/sdl/include/")
  set(SDL_LIB "${ANDROID_NDK}/sources/third_party/sdl/lib/${ANDROID_ABI}/")
  # Sadly this way breaks gradle...
  #add_library(SDL2 SHARED IMPORTED)
  #set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION ${SDL_LIB}/libSDL2.so)
  #set_property(TARGET SDL2 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SDL_INC})
  # Instead for the old school way
  target_include_directories(${PROJECT_NAME} PUBLIC ${SDL_INC})
  target_link_directories(${PROJECT_NAME} PUBLIC ${SDL_LIB})
endif()

add_subdirectory(src/aw/ecs)
add_subdirectory(src/aw/engine)
add_subdirectory(src/aw/util)

target_sources(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/aw/util/log.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${FETCHCONTENT_BASE_DIR}/glm-src)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog freetype SDL2)

if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Android")
  target_link_libraries(${PROJECT_NAME} PUBLIC SDL2)

  find_package(OpenGL)
  target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)

  cmake_policy(SET CMP0079 NEW)
  target_link_libraries(SDL2 ${CMAKE_DL_LIBS} dl)
else()
  target_link_libraries(${PROJECT_NAME} PUBLIC android log GLESv3 GLESv2 GLESv1_CM)
endif()


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
