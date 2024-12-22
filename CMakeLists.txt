cmake_minimum_required(VERSION 3.16)

project(spi-exchange VERSION 0.1 LANGUAGES CXX)

set(EXECUTABLE_NAME app-spi-exchange)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick SerialPort)

qt_standard_project_setup()

qt_add_executable(${EXECUTABLE_NAME}
    src/main.cpp
    include/app_engine.h
    src/app_engine.cpp

    include/proto/exchange.h
    src/proto/exchange.cpp
    include/proto/crc16.h
    src/proto/crc16.cpp
    include/proto/proto.h
    src/proto/proto.cpp
    include/proto/parser.h
    src/proto/parser.cpp

    include/gui/spi_page_gui.h
    src/gui/spi_page_gui.cpp
    include/gui/connection_page_gui.h
    src/gui/connection_page_gui.cpp
)

set(qml_files
    qml/Main.qml
    qml/SpiControls.qml
    qml/Logs.qml
    qml/ConnectionPage.qml
    qml/SpiPage.qml
    qml/Status.qml
)

qt_add_qml_module(${EXECUTABLE_NAME}
    URI SpiExchangeApp
    VERSION 1.0
    QML_FILES
        ${qml_files}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.apparinc429-spi
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_subdirectory(ext)

target_include_directories(${EXECUTABLE_NAME}
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/ext/qt-module-serial-port/include"
)

target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE
        Qt6::Quick
        Qt6::SerialPort
        io-port-libplugin
)

include(GNUInstallDirs)
install(TARGETS ${EXECUTABLE_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
