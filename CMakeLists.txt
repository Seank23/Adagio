cmake_minimum_required(VERSION 3.16)

project(Adagio VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appAdagio
    main.cpp
)

qt_add_qml_module(appAdagio
    URI Adagio
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES app/Analysis/AnalysisProcessor.h app/Analysis/AnalysisService.cpp app/Analysis/AnalysisService.h app/Analysis/FFTProcessor.h app/Analysis/temp.h app/Core/Application.cpp app/Core/Application.h app/Core/AudioUtils.h app/Debug/Instrumentation.h app/IO/AudioData.h app/IO/AudioFileHandler.cpp app/IO/AudioFileHandler.h app/IO/FileIOService.cpp app/IO/FileIOService.h app/IO/PlaybackService.cpp app/IO/PlaybackService.h app/Adagio.h app/adagio_pch.cpp app/adagio_pch.h
        SOURCES ui/controller/MainUIController.h ui/controller/MainUIController.cpp
        QML_FILES
        QML_FILES ui/components/HeaderBar/HeaderBar.qml
        QML_FILES ui/components/BottomBar/BottomBar.qml
        QML_FILES ui/components/WaveformChart/WaveformChart.qml
        SOURCES app/API/Utils.h
        SOURCES
)

include(PrecompiledHeader.cmake)
add_precompiled_header(appAdagio app/adagio_pch.h FORCEINCLUDE SOURCE_CXX app/adagio_pch.cpp)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appAdagio PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAdagio
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

find_package(Qt6 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 REQUIRED COMPONENTS Charts)

include_directories(vendor/miniaudio)
include_directories(vendor/kfr/include)
add_subdirectory(vendor/kfr)

target_link_libraries(appAdagio
    PRIVATE Qt6::Quick Qt6::Concurrent Qt6::Charts kfr_dft kfr_dsp kfr_io
)

include(GNUInstallDirs)
install(TARGETS appAdagio
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
