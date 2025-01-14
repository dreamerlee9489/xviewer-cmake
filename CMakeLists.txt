cmake_minimum_required(VERSION 3.31)

# 项目名称
project(xviewer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 Qt5 安装路径
set(CMAKE_PREFIX_PATH "D:/msys64/mingw64")

# 找到 Qt5 包（根据项目需求调整模块）
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# 设置 xcodec 的头文件和库路径
include_directories("D:/msys64/mingw64/include/xcodec") # 添加头文件路径
link_directories("D:/msys64/mingw64/lib") # 添加库文件路径

# 设置源文件和头文件
set(SOURCES
    main.cpp
    xcamera_config.cpp
    xcamera_record.cpp
    xcamera_widget.cpp
    xviewer.cpp
)

set(HEADERS
    xcamera_config.h
    xcamera_record.h
    xcamera_widget.h
    xviewer.h
)

# 包含 UI 文件
set(UI_FILES
    xviewer.ui
)

# 包含资源文件（如果有）
set(QRC_FILES
    xviewer.qrc
)

# 处理 Qt 的 UI 文件
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# 处理包含 Q_OBJECT 的类（自动调用 moc 生成代码）
qt5_wrap_cpp(MOC_SOURCES ${HEADERS})

# 处理资源文件
qt5_add_resources(RESOURCES ${QRC_FILES})

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_SOURCES} ${HEADERS} ${UI_HEADERS} ${RESOURCES})

# 链接 Qt5 库和 xcodec 库
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    xcodec
)