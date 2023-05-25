# CMakeList.txt: CMakeSocket 的 CMake 專案，在此包含來源及定義
# 專案專屬邏輯。
#
cmake_minimum_required (VERSION 3.5)
project(CMakeSocket VERSION 0.2.0)
# 將來源新增到此專案的可執行檔。
add_executable (CMakeSocket "CMakeSocket.cpp" "CMakeSocket.h" "EasySocket.hpp")
if(WIN32)
  target_link_libraries(CMakeSocket wsock32 ws2_32)
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CMakeSocket PROPERTY CXX_STANDARD 20)
endif()

# TODO: 新增測試，並視需要安裝目標。
