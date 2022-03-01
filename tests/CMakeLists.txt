project(SpaceMissionSimVis-test)

set(SOURCE_TEST
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/PanguParser.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/FileUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/SocketUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/StringUtil.cpp
    )

add_executable("${PROJECT_NAME}" ${SOURCE_TEST})

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
    SpaceMissionSimVis-lib
    pan_protocol-lib
    gtest
    )

set_target_properties("${PROJECT_NAME}"
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin"
        )

    file(COPY files DESTINATION "${CMAKE_BINARY_DIR}/bin")
