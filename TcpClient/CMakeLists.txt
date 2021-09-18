
cmake_minimum_required (VERSION 3.8)

project ("TcpClient")

if (WIN32)
find_library(ws2_32_LIBRARY_PATH ws2_32)
endif()

add_executable (TcpClient "TcpClient.cpp" "TcpClient.h" "Socket.cpp" "Socket.h" "Asn1.cpp" "Asn1.h")

if(WIN32)
target_link_libraries(TcpClient ${ws2_32_LIBRARY_PATH})
endif()
