cmake_minimum_required(VERSION 3.5)

project(Fixity VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets PrintSupport SerialPort REQUIRED)
find_package(OpenGL REQUIRED)

# 定义 ExamplePage 源文件和头文件的路径
set(BASE_DIR ${CMAKE_SOURCE_DIR}/Base)
set(SERIAL_DIR ${CMAKE_SOURCE_DIR}/Serial)
set(NOTE_DIR ${CMAKE_SOURCE_DIR}/Note)
set(EXAMPLEPAGE_DIR ${CMAKE_SOURCE_DIR}/ExamplePage)

# 使用 GLOB_RECURSE 收集 ExamplePage 文件夹中的所有 .cpp 和 .h 文件
file(GLOB_RECURSE BASE_SOURCES "${BASE_DIR}/*.cpp")
file(GLOB_RECURSE BASE_HEADERS "${BASE_DIR}/*.h")
file(GLOB_RECURSE SERIAL_SOURCES "${SERIAL_DIR}/*.cpp")
file(GLOB_RECURSE SERIAL_HEADERS "${SERIAL_DIR}/*.h")
file(GLOB_RECURSE NOTE_SOURCES "${NOTE_DIR}/*.cpp")
file(GLOB_RECURSE NOTE_HEADERS "${NOTE_DIR}/*.h")
file(GLOB_RECURSE EXAMPLEPAGE_SOURCES "${EXAMPLEPAGE_DIR}/*.cpp")
file(GLOB_RECURSE EXAMPLEPAGE_HEADERS "${EXAMPLEPAGE_DIR}/*.h")


set(PROJECT_SOURCES
       main.cpp
       mainwindow.cpp
       mainwindow.h
       ${BASE_SOURCES}
       ${BASE_HEADERS}  # 头文件不需要在 add_executable 中列出，但需要包含路径
       ${SERIAL_SOURCES}
       ${SERIAL_HEADERS}  # 头文件不需要在 add_executable 中列出，但需要包含路径
       ${NOTE_SOURCES}
       ${NOTE_HEADERS}  # 头文件不需要在 add_executable 中列出，但需要包含路径
       ${EXAMPLEPAGE_SOURCES}
       ${EXAMPLEPAGE_HEADERS}  # 头文件不需要在 add_executable 中列出，但需要包含路径
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Fixity
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        include/ElaWidgetTools.qrc
    )
    qt_add_resources(${PROJECT_NAME}
        RESOURCES PREFIX "/"
        FILES
        ${resource_files}
    )
else()
    qt5_add_big_resources(PROJECT_SOURCES
        ElaWidgetToolsExample.qrc
    )
    if(ANDROID)
        add_library(Fixity SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Fixity
            ${PROJECT_SOURCES}
        )
    endif()
endif()

include_directories(
    include
)
target_link_libraries(Fixity PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libElaWidgetTools.dll.a
    Qt${QT_VERSION_MAJOR}::PrintSupport  # 添加这一行
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)


# 平台特定配置
if(WIN32)
    target_link_libraries(Fixity PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/libElaWidgetTools.dll.a
    )
    target_compile_definitions(Fixity PRIVATE Q_OS_WIN)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(Fixity PRIVATE pthread X11)
    target_compile_definitions(Fixity PRIVATE Q_OS_LINUX)
endif()

set_target_properties(Fixity PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Fixity)
endif()
