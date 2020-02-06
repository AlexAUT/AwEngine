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
