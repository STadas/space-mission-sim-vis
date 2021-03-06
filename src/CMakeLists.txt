set(LIBRARY_PROJECT "${PROJECT_NAME}-lib")
set(EXECUTABLE_PROJECT "${PROJECT_NAME}")

# src
set(SOURCE_PROJECT
        common/CamPoint.cpp
        common/CamPoint.hpp
        common/HBoxLayout.cpp
        common/HBoxLayout.hpp
        common/MessageController.cpp
        common/MessageController.hpp
        common/PanguConnection.cpp
        common/PanguConnection.hpp
        common/PanguParser.cpp
        common/PanguParser.hpp
        common/PanguServerProcess.cpp
        common/PanguServerProcess.hpp
        common/ParseResult.cpp
        common/ParseResult.hpp
        common/PingWorker.cpp
        common/PingWorker.hpp
        common/PreviewWorker.cpp
        common/PreviewWorker.hpp
        common/ParsedCommand.cpp
        common/ParsedCommand.hpp
        common/Resources.cpp
        common/Resources.hpp
        common/VBoxLayout.cpp
        common/VBoxLayout.hpp

        common/settings/SettingManager.cpp
        common/settings/SettingManager.hpp
        common/settings/Settings.cpp
        common/settings/Settings.hpp

        util/FileUtil.cpp
        util/FileUtil.hpp
        util/SocketUtil.cpp
        util/SocketUtil.hpp
        util/StringUtil.cpp
        util/StringUtil.hpp
        util/ThreadUtil.cpp
        util/ThreadUtil.hpp

        widgets/Action.cpp
        widgets/Action.hpp
        widgets/CamPreview.cpp
        widgets/CamPreview.hpp
        widgets/ConnectionDisplay.cpp
        widgets/ConnectionDisplay.hpp
        widgets/CoordsVis.cpp
        widgets/CoordsVis.hpp
        widgets/MainWindow.cpp
        widgets/MainWindow.hpp
        widgets/MenuSeparator.cpp
        widgets/MenuSeparator.hpp
        widgets/Editor.cpp
        widgets/Editor.hpp
        widgets/LogsView.cpp
        widgets/LogsView.hpp
        widgets/PlaybackInterface.cpp
        widgets/PlaybackInterface.hpp
        widgets/ProgressBar.cpp
        widgets/ProgressBar.hpp
        widgets/RecentsMenu.cpp
        widgets/RecentsMenu.hpp

        widgets/dialogs/AboutDialog.cpp
        widgets/dialogs/AboutDialog.hpp
        widgets/dialogs/Dialog.cpp
        widgets/dialogs/Dialog.hpp
        widgets/dialogs/SettingsDialog.cpp
        widgets/dialogs/SettingsDialog.hpp

        widgets/settingspages/CommandsPage.cpp
        widgets/settingspages/CommandsPage.hpp
        widgets/settingspages/CoordsVisPage.cpp
        widgets/settingspages/CoordsVisPage.hpp
        widgets/settingspages/EditorPage.cpp
        widgets/settingspages/EditorPage.hpp
        widgets/settingspages/ServerPage.cpp
        widgets/settingspages/ServerPage.hpp
        widgets/settingspages/SettingsPage.cpp
        widgets/settingspages/SettingsPage.hpp

        ${CMAKE_SOURCE_DIR}/resources/resources.qrc
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
        Qt5::3DCore
        Qt5::3DExtras
        Qt5::3DRender

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
        set(LINUXDEPLOY_COMMAND "bash ${CMAKE_SOURCE_DIR}/tools/linuxdeployqt.sh")
        install(CODE "execute_process(COMMAND ${LINUXDEPLOY_COMMAND})")
    endif ()
endif()

if(MSVC)
    target_compile_options(${LIBRARY_PROJECT} PUBLIC /EHsc)
else()
    target_compile_options(${LIBRARY_PROJECT} PUBLIC -Wall -Wextra -Wpedantic -Werror)
endif()
