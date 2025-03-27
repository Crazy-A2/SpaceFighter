# 指定构建项目所需的cmake最低版本
cmake_minimum_required(VERSION 3.30.0)

# 指定项目名
project(SpaceFighter VERSION 0.1.0)

# 指定C++标准
set(CMAKE_CXX_STANDARD 20)

# 设置编译器选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 将源码目录(src)赋值给变量 SOURCE_DIR
aux_source_directory(src SOURCE_DIR)

# 指定要链接的库文件所在目录 须在 add_executable 之前
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 指定生成可执行文件 使用变量
add_executable(${PROJECT_NAME} ${SOURCE_DIR})

# 指定可执行文件的生成位置
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加 include 目录 
include_directories(${PROJECT_SOURCE_DIR}/include)

# 链接库文件
# Windows 下链接动态库需要编译时链接动态库的导入库（.lib）
target_link_libraries(${PROJECT_NAME} SDL2 SDL2main SDL2_image SDL2_ttf SDL2_mixer)
# target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image SDL2_ttf SDL2_mixer)

# 打包准备 禁止弹出控制台窗口
Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

# -----------------------------------------------------------------------------
# 第二种链接动态库的方式
# 指定库的位置
# set(LIBRARY_PATH ${PROJECT_SOURCE_DIR}/lib)

# target_link_libraries(${PROJECT_NAME} ${LIBRARY_PATH}/SDL2.lib)
# target_link_libraries(${PROJECT_NAME} SDL2)
