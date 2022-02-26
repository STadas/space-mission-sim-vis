set(LIBRARY_PROJECT "${PROJECT_NAME}-lib")
set(EXECUTABLE_PROJECT "${PROJECT_NAME}")

# src
set(SOURCE_PROJECT
        common/MessageController.cpp
        common/MessageController.hpp
        common/PanguConnection.cpp
        common/PanguConnection.hpp
        common/PanguWorker.cpp
        common/PanguWorker.hpp
        common/ParsedCommand.cpp
        common/ParsedCommand.hpp
        common/VBoxLayout.cpp
        common/VBoxLayout.hpp

        util/CommandUtil.cpp
        util/CommandUtil.hpp
        util/FileUtil.cpp
        util/FileUtil.hpp
        util/SocketUtil.cpp
        util/SocketUtil.hpp
        util/StringUtil.cpp
        util/StringUtil.hpp
        util/ThreadUtil.cpp
        util/ThreadUtil.hpp

        widgets/MainWindow.cpp
        widgets/MainWindow.hpp
        widgets/PanguEditor.cpp
        widgets/PanguEditor.hpp
        widgets/SimPreview.cpp
        widgets/SimPreview.hpp
        )

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE_PROJECT})
add_library(${LIBRARY_PROJECT} OBJECT ${SOURCE_PROJECT})

# pan_protocol
set(LIBRARY_PAN_PROTOCOL "pan_protocol-lib")
set(DIR_PAN_PROTOCOL "${CMAKE_SOURCE_DIR}/vendor/pan_protocol")

set(SOURCE_PAN_PROTOCOL
        "${DIR_PAN_PROTOCOL}/pan_protocol_lib.cpp"
        "${DIR_PAN_PROTOCOL}/pan_protocol_lib.h"
        "${DIR_PAN_PROTOCOL}/pan_socket_io.cpp"
        "${DIR_PAN_PROTOCOL}/pan_socket_io.h"
        )

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE_PAN_PROTOCOL})
add_library(${LIBRARY_PAN_PROTOCOL} OBJECT ${SOURCE_PAN_PROTOCOL})

target_include_directories("${LIBRARY_PROJECT}"
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/vendor"
        )


target_link_libraries(${LIBRARY_PROJECT}
        PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        Qt5::Concurrent

        Threads::Threads
        )


if (WIN32)
    target_link_libraries(${LIBRARY_PROJECT}
        PUBLIC
        wsock32
        ws2_32
        )
endif ()


# build app
if (BUILD_APP)
    add_executable("${EXECUTABLE_PROJECT}" "main.cpp")

    target_link_libraries("${EXECUTABLE_PROJECT}" PUBLIC ${LIBRARY_PROJECT})
    target_link_libraries("${EXECUTABLE_PROJECT}" PUBLIC ${LIBRARY_PAN_PROTOCOL})

    set_target_properties("${EXECUTABLE_PROJECT}"
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
            RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin"
            )

    if (MSVC)
        get_target_property(Qt_Core_Location Qt5::Core LOCATION)
        get_filename_component(QT_BIN_DIR ${Qt_Core_Location} DIRECTORY)
        set(WINDEPLOYQT_COMMAND
            "${QT_BIN_DIR}/windeployqt.exe ${CMAKE_BINARY_DIR}/bin/${EXECUTABLE_PROJECT}.exe \
                --release --no-compiler-runtime --no-translations --no-opengl-sw")

        install(TARGETS ${EXECUTABLE_PROJECT}
                RUNTIME DESTINATION .
                )

        install(CODE "execute_process(COMMAND ${WINDEPLOYQT_COMMAND} --dir ${CMAKE_INSTALL_PREFIX})")
    elseif (APPLE)
        install(TARGETS ${EXECUTABLE_PROJECT}
                RUNTIME DESTINATION bin
                BUNDLE DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static
                )

        set_target_properties(${EXECUTABLE_PROJECT} PROPERTIES MACOSX_BUNDLE TRUE)
        set_target_properties(${EXECUTABLE_PROJECT}
            PROPERTIES
            MACOSX_BUNDLE_BUNDLE_NAME "SpaceMissionSimVis"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.SpaceMissionSimVis"
            MACOSX_BUNDLE_INFO_STRING "A tool to make producing spacecraft landing simulations easier"
            MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
            )
    else ()
        install(TARGETS ${EXECUTABLE_PROJECT}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib/static
                )
    endif ()
endif()

if (MSVC)
    target_compile_options(${LIBRARY_PROJECT} PUBLIC /EHsc)
endif ()
