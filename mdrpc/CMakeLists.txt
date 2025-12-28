
cmake_minimum_required(version 3.0)
project(mdrpc)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# -I
include_directories(${PROJECT_SOURCE_DIR}/include)

# -L
link_directories(${PROJECT_SOURCE_DIR}/lib)

