file(GLOB_RECURSE ALL_SRCS *.cpp)
add_library(hotring STATIC ${ALL_SRCS})
# add_library(shared SHARED ${ALL_SRCS})

#
# Possible link
#

# target_link_libraries(static armadillo)
# add_executable(main main.cpp)
