cmake_minimum_required(VERSION 3.16)

project(PathFinder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.10)

qt_add_executable(appPathFinder
    main.cpp
)

qt_add_qml_module(appPathFinder
    URI PathFinder
    QML_FILES
        Main.qml
        SOURCES src/jsonparser.h src/jsonparser.cpp
        SOURCES src/gridstate.h src/gridstate.cpp
        SOURCES src/pathfinder.h src/pathfinder.cpp
        SOURCES src/tileset.h src/tileset.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPathFinder PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPathFinder
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPathFinder
    PRIVATE Qt6::Quick
)

install(TARGETS appPathFinder
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
