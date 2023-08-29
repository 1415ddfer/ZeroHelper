cmake_minimum_required(VERSION 3.25)

project(ZeroHelper)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address") #内存泄露检测
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_SYSTEM_VERSION 11)



set(QTVersion 6)
#set(Qt6_DIR D:/IdeaTools/Qt6/6.4.3/msvc2019_64/lib/cmake/Qt6)
set(Qt6_DIR D:/IdeaTools/Qt6/6.5.2/mingw_64/lib/cmake/Qt6)
#set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include")

find_package(
        Qt${QTVersion}
        REQUIRED
        COMPONENTS
        Core
        Gui
        Widgets
        Network
        Concurrent
        Qml
        )
# REQUIRED 必须的
find_package(GSL REQUIRED)

file(GLOB_RECURSE SRCS
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )

#添加额外的 源文件 例如资源文件等
LIST(APPEND  SRCS res.qrc)
LIST(APPEND  SRCS res.rc)

add_executable(${PROJECT_NAME}   ${SRCS} )
target_link_libraries(${PROJECT_NAME}
        PRIVATE GSL::gsl GSL::gslcblas
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Concurrent
        Qt::Qml
        )

