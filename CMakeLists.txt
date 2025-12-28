cmake_minimum_required(VERSION 3.10)
project(OS_CourseDesign_Qt)

set(CMAKE_CXX_STANDARD 14)

# 开启 Qt 自动处理机制 (MOC/UIC/RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找 Qt5 组件
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# 包含当前目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加源文件
add_executable(OS_CourseDesign_Qt
        main.cpp
        MainWindow.cpp MainWindow.h
        MemoryManager.cpp MemoryManager.h
        MemoryWidget.cpp MemoryWidget.h
        MemoryManager.cpp
        MemoryManager.h
)

# 链接 Qt 库
target_link_libraries(OS_CourseDesign_Qt Qt5::Widgets)