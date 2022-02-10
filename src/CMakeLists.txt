set(LIBRARY_PROJECT "${PROJECT_NAME}-lib")
set(EXECUTABLE_PROJECT "${PROJECT_NAME}")

set(SOURCE_PROJECT
        widgets/MainWindow.cpp
        widgets/MainWindow.hpp
        widgets/VBoxLayout.hpp
        widgets/VBoxLayout.cpp
        )

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SOURCE_PROJECT})
add_library(${LIBRARY_PROJECT} OBJECT ${SOURCE_PROJECT})

target_link_libraries(${LIBRARY_PROJECT}
        PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
        Qt5::Concurrent

        Threads::Threads
        )

target_include_directories("${LIBRARY_PROJECT}"
        PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/vendor"
        )

if (BUILD_APP)
    add_executable("${EXECUTABLE_PROJECT}" "main.cpp")

    target_link_libraries("${EXECUTABLE_PROJECT}" PUBLIC ${LIBRARY_PROJECT})

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
            MACOSX_BUNDLE_INFO_STRING "testing ci stuff"
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
