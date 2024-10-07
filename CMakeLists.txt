cmake_minimum_required(VERSION 3.0)
project(lab1)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(myfilemanager_library STATIC "lab1/MyFileManagerLib/MyFileManager.cpp")
add_executable(main lab1/MyFileManagerApp/MyFileManagerApp.cpp)
target_link_libraries(main myfilemanager_library)

