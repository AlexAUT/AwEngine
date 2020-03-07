set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

function(loadDependencyFromGit name url tag)
  FetchContent_Declare(
    ${name}
    #SOURCE_DIR ${FETCHCONTENT_BASE_DIR}/${name}-src
    BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${name}-build
    GIT_REPOSITORY ${url}
    GIT_TAG ${tag}
    GIT_PROGRESS ON
  )

  FetchContent_GetProperties(${name})
  if (NOT ${name}_POPULATED)
    FetchContent_Populate(
      ${name}
    )
  add_subdirectory(${${name}_SOURCE_DIR} ${${name}_BINARY_DIR})
  endif()
endfunction()

function(loadDependencyFromUrl name url)
  FetchContent_Declare(
    ${name}
    URL ${url}
  )

  FetchContent_GetProperties(${name})
  if (NOT ${name}_POPULATED)
    FetchContent_Populate(
      ${name}
    )
  add_subdirectory(${${name}_SOURCE_DIR} ${${name}_BINARY_DIR})
  endif()
endfunction()
