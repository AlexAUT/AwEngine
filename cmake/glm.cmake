include(ExternalProject)

set(DEPS_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(DEPS_SRC "${DEPS_PREFIX}/src/")
set(DEPS_INSTALL_DIR "${DEPS_PREFIX}/bin_${CMAKE_BUILD_TYPE}")

ExternalProject_Add(
  glm-src
  PREFIX ${DEPS_PREFIX}
  BINARY_DIR "deps/glm_build_${CMAKE_BUILD_TYPE}"
  INSTALL_DIR ${DEPS_INSTALL_DIR}
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 0.9.9.7
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEPS_INSTALL_DIR} -DGLM_TEST=OFF -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  UPDATE_COMMAND ""
)
set(FMT_INC "${DEPS_SRC}/fmt/include")
