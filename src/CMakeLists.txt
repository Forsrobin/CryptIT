# ================================ #
# === Create build targets ======= #
# ================================ #
add_executable(${TARGET_NAME} main.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Platform Qt6::QWindowsIntegrationPlugin) 

install(TARGETS ${TARGET_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})

# if (WIN32)
#     add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${TARGET_NAME}> $<TARGET_FILE_DIR:${TARGET_NAME}> COMMAND_EXPAND_LISTS)
#     # # Remove the SopaySDR.dll file that is copied to the build directory
#     add_custom_command(TARGET ${TARGET_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove $<TARGET_FILE_DIR:${TARGET_NAME}>/SoapySDR.dll)
# endif()

