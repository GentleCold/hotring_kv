file(GLOB_RECURSE ALL_TESTS *.cpp)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
  add_executable(test_all_debug ${ALL_TESTS})
  target_link_libraries(test_all_debug hotring gtest)
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
  add_executable(test_all_release ${ALL_TESTS})
  target_link_libraries(test_all_release hotring gtest)
endif()
