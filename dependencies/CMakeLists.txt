include(FetchContent)

#=====================================
# Helper functions and macros
#=====================================

# Adds external dependency into a project
function(add_external_dependency dependency)
  add_subdirectory(${${dependency}_SOURCE_DIR} ${${dependency}_BINARY_DIR})
endfunction()

#=====================================
# Options and initial settings
#=====================================

message(STATUS "Fetching third party libraries")
option(LINK_DEPS_STATIC CACHE ON)

if (NOT LINK_DEPS_STATIC)
  set(BUILD_SHARED_LIBS ON)
else()
  set(BUILD_SHARED_LIBS OFF)
endif()

#=====================================
# FetchContent declarations
#=====================================

if(BUILD_TESTING)
  FetchContent_Declare(
    googletest
    #GIT_REPOSITORY "https://github.com/google/googletest"
    #GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e # 1.10
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )
endif()

#=====================================

if(BUILD_TESTING)
  add_subdirectory(googletest)
endif()