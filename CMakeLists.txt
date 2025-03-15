# 指定构建项目所需的cmake最低版本
cmake_minimum_required(VERSION 3.30.0)

# 指定项目名
project(sdl2-game VERSION 0.1.0)

# 指定C++标准
set(CMAKE_CXX_STANDARD 20)

# 将源码目录(src)赋值给变量 SOURCE_DIR
aux_source_directory(src SOURCE_DIR)

# 指定生成可执行文件 使用变量
add_executable(sdl2-game ${SOURCE_DIR})

# 添加 include 目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 指定链接库目录
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 链接库文件
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image SDL2_ttf SDL2_mixer)

# 指定静态库的位置
# set(LIBRARY_PATH ${PROJECT_SOURCE_DIR}/lib)

# target_link_libraries(${PROJECT_NAME} ${LIBRARY_PATH}/SDL2.lib ${LIBRARY_PATH}/SDL2main.lib)
