
# protobuf 生成文件目录
set(PROTOBUF_GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../protobuff/pbgen)

# mdrpc 头文件目录
set(MDRPC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/mdrpc/include)

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

# 包含目录（使用 target_include_directories 更现代）
target_include_directories(rpc_provider PRIVATE 
    ${PROTOBUF_GEN_DIR}
    ${MDRPC_INCLUDE_DIR}
    /opt/protobuf3/include
)

# 链接库
target_link_directories(rpc_provider PRIVATE /opt/protobuf3/lib)
target_link_libraries(rpc_provider 
    protobuf 
    pthread
)

# 设置运行时库路径
set_target_properties(rpc_provider PROPERTIES
    INSTALL_RPATH "/opt/protobuf3/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)