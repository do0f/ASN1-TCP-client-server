
cmake_minimum_required (VERSION 3.8)

project ("TcpServer")

if(WIN32)
find_library(ws2_32_LIBRARY_PATH ws2_32)
endif()

add_executable (TcpServer "TcpServer.cpp" "TcpServer.h" "Socket.cpp" "Socket.h" "Asn1.cpp" "Asn1.h")

if (WIN32)
target_link_libraries(TcpServer ${ws2_32_LIBRARY_PATH})
else()
target_link_libraries(TcpServer pthread)
endif()
