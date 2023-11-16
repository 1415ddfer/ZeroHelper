cmake_minimum_required(VERSION 3.25)

project(ZeroHelper)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") #内存泄露检测
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_SYSTEM_VERSION 11)



set(QTVersion 6)
set(Qt6_DIR D:/IdeaTools/Qt6/6.5.3/mingw_64/lib/cmake/Qt6)

find_package(
        Qt${QTVersion}
        REQUIRED
        COMPONENTS
        Core
        Gui
        Widgets
        Network
        HttpServer
        Concurrent
        Qml
        )
# REQUIRED 必须的
#find_package(GSL REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB_RECURSE SRCS
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

#添加额外的 源文件 例如资源文件等
#LIST(APPEND  SRCS qt.conf)
LIST(APPEND  SRCS res.qrc)
LIST(APPEND  SRCS res.rc)

add_executable(${PROJECT_NAME}   ${SRCS} )
target_link_libraries(${PROJECT_NAME}
#        PRIVATE GSL::gsl GSL::gslcblas
        OpenSSL::Crypto
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::HttpServer
        Qt::Concurrent
        Qt${QTVersion}::Qml
        )
#target_link_libraries (${PROJECT_NAME} -static gcc stdc++ -dynamic)
# 设置需要复制的Qt库
# 添加自定义命令，调用deployqt工具
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND windeployqt6.exe --qmldir ${CMAKE_SOURCE_DIR} $<TARGET_FILE_DIR:${PROJECT_NAME}>)