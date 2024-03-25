# ================================ #
# === Create build targets ======= #
# ================================ #
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES "./*.cpp")
add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE 
    cryptopp::cryptopp 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
    Qt6::Platform 
    Qt6::QWindowsIntegrationPlugin
    gtest::gtest) 

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# Copy over the assets directory
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${TARGET_NAME}>/assets)

# if (WIN32)
#     add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TARGET_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}> COMMAND_EXPAND_LISTS)
#     # # Remove the SopaySDR.dll file that is copied to the build directory
#     add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE_DIR:${TARGET_NAME}>/SoapySDR.dll)
# endif()


# ================================ #
# === Set up test  environment === #
# ================================ #
if (BUILD_TESTS)
    message(STATUS "Building tests")
    add_subdirectory(tests)
endif ()