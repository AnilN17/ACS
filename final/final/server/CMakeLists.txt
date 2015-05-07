
set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(server server.cpp)
target_link_libraries(server -Wall -g -lsctp pthread)
