CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(awEngine)
add_library(${PROJECT_NAME} SHARED)

# Hide symbols by default
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_VISIBILITY_PRESET hidden)

include(FetchContent)

include(cmake/color.cmake)
include(cmake/loadDependency.cmake)

set(FETCHCONTENT_QUIET OFF)

set(BUILD_STATIC_LIBS OFF)

set(SPDLOG_BUILD_SHARED ON CACHE BOOL "")
set(SPDLOG_BUILD_STATIC OFF CACHE BOOL "")
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "")
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "")
loadDependencyFromGit(spdlog https://github.com/gabime/spdlog v1.4.2)

set(GLM_TEST_ENABLE OFF CACHE BOOL "Disable test")
loadDependencyFromGit(glm https://github.com/g-truc/glm 0.9.9.6)

loadDependencyFromGit(freetype https://github.com/aseprite/freetype2 VER-2-10-0)

set(GHC_FILESYSTEM_EXAMLES OFF CACHE BOOL "")
set(GHC_FILESYSTEM_BUILD_TESTING OFF CACHE BOOL "")
set(GHC_FILESYSTEM_WITH_INSTALL OFF CACHE BOOL "")
loadDependencyFromGit(filesystem https://github.com/gulrak/filesystem v1.3.2)

set(JUST_INSTALL_CEREAL ON CACHE BOOL "")
loadDependencyFromGit(cereal https://github.com/USCiLab/cereal v1.3.0)

set(BUILD_TESTING OFF CACHE BOOL "")
loadDependencyFromGit(platformheaders https://github.com/sago007/PlatformFolders 4.0.0)

loadDependencyFromGit(entt https://github.com/skypjack/entt v3.3.2)

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
set(SDL_STATIC OFF OFF CACHE BOOL "")
#set(SDL_TEST OFF)
#set(SDL_THREADS OFF)
#set(SDL_TIMERS OFF)
#set(SDL_OPENGLES OFF)
#set(SDL_RPI OFF)
#set(SDL_VIVANTE OFF)
#set(SDL_VULKAN ON)
set(VIDEO_KMSDRM OFF CACHE BOOL "")

loadDependencyFromGit(sdl https://github.com/spurious/SDL-mirror.git dc5800a1a4e5635858b8c4caf9f522aa63f87f7b)
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
add_subdirectory(src/aw/graphics)

target_sources(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include/aw/util/log.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${FETCHCONTENT_BASE_DIR}/glm-src)
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog freetype SDL2 platform_folders EnTT::EnTT cereal)

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

include(cmake/enableWarnings.cmake)
enable_warnings(${PROJECT_NAME})
