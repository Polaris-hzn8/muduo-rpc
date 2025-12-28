
# protobuf 生成文件目录
set(PROTOBUF_GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../protobuff/pbgen)

# 收集当前目录的源文件
file(GLOB CURRENT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

# 收集 protobuf 生成的源文件
file(GLOB PROTOBUF_SOURCES "${PROTOBUF_GEN_DIR}/*.cc")

# 源文件列表
set(SOURCES
    ${CURRENT_SOURCES}
    ${PROTOBUF_SOURCES}
)

# 创建可执行文件
add_executable(rpc_provider ${SOURCES})

# 包含目录（protobuf 生成的头文件）
target_include_directories(rpc_provider PRIVATE 
    ${PROTOBUF_GEN_DIR}
)

# 链接库（mdrpc 的 PUBLIC 头文件目录会自动传递到这里）
# 注意：protobuf 库路径已在根 CMakeLists.txt 中通过 link_directories 设置
target_link_libraries(rpc_provider 
    mdrpc      # 自动获取 mdrpc 的 PUBLIC 头文件路径
    protobuf   # protobuf 库（mdrpc 已经链接了，这里显式链接以确保顺序）
    pthread
)

# 设置运行时库路径（包括 mdrpc 和 protobuf 的库路径）
set_target_properties(rpc_provider PROPERTIES
    INSTALL_RPATH "${CMAKE_SOURCE_DIR}/lib;/opt/protobuf3/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)