project(qt-cmake-project)
cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)
set (PYTHON_LIBRARIES /usr/local/Cellar/python3/3.6.3/Frameworks/Python.framework/Versions/3.6/lib/libpython3.6.dylib)
set (PYTHON_INCLUDE_DIRS /usr/local/Cellar/python3/3.6.3/Frameworks/Python.framework/Versions/3.6/include/python3.6m)

find_package(Qt5 COMPONENTS Core Widgets)

find_package(PythonLibs 3.6)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(${PYTHON_INCLUDE_DIRS})

set(SOURCES
    main.cpp widgets.cpp
    )

set(HEADERS widgets.h)

qt5_wrap_cpp(MOC_SRC ${HEADERS})

add_executable(
    my_program
    ${SOURCES} ${UI_SRC} ${MOC_SRC}
    )

python_add_module(
    _pywidgets
    _pywidgets.cpp
    widgets.cpp
    ${MOC_SRC}
    )

target_link_libraries(_pywidgets ${PYTHON_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES})

target_link_libraries(my_program
     ${Qt5Core_LIBRARIES}
     ${Qt5Widgets_LIBRARIES}
     )

add_definitions(-DQT_NO_KEYWORDS)
