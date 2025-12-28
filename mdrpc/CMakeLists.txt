# 注意：此文件作为子目录被包含，不需要 cmake_minimum_required 和 project

# 输出目录（使用 CMAKE_SOURCE_DIR 指向根项目目录）
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

# 收集源文件
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

# 创建共享库（动态库）
add_library(mdrpc SHARED ${SOURCES})

# 设置包含目录（PUBLIC 使得链接此库的目标自动获得头文件路径）
target_include_directories(mdrpc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    /opt/protobuf3/include
)

# 链接依赖库（使用完整路径以兼容旧版 CMake）
target_link_libraries(mdrpc
    /opt/protobuf3/lib/libprotobuf.so
    pthread
)

# 设置运行时库路径
set_target_properties(mdrpc PROPERTIES
    INSTALL_RPATH "/opt/protobuf3/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
    VERSION 1.0.0
    SOVERSION 1
)