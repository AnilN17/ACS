
set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(client client.cpp)
target_link_libraries(client -Wall -g -lsctp)
