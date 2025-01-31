cmake_minimum_required(VERSION 3.31)

# 项目名称
project(xcodec LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 源文件和头文件
file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

# 找到 SDL2
find_package(SDL2 REQUIRED)

# 添加动态库
add_library(xcodec SHARED ${SOURCES} ${HEADERS})

# 导出符号（在 Windows 下需要定义）
if (WIN32)
    target_compile_definitions(xcodec PRIVATE XCODEC_EXPORTS)
endif()

# 包含 SDL2 的头文件路径
target_include_directories(xcodec PRIVATE ${SDL2_INCLUDE_DIRS})

# 链接 SDL2 和其他依赖库
target_link_libraries(xcodec PRIVATE ${SDL2_LIBRARIES}
    avcodec
    avformat
    avutil
    swscale
)