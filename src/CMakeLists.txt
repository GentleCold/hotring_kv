set(SOURCES hotring.cpp hrnode.cpp)
add_library(hotring STATIC ${SOURCES})
# add_library(shared SHARED ${ALL_SRCS})

#
# Possible link
#

add_executable(client client.cpp)
add_executable(random_load random_load.cpp)
add_executable(server server.cpp)
target_link_libraries(server hotring)
