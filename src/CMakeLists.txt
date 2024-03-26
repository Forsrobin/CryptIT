# ================================ #
# === Create build targets ======= #
# ================================ #
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES "./*.cpp")

# Add icons
include(${CMAKE_SOURCE_DIR}/AddIconToBinary.cmake)
AddIconToBinary(SOURCE_FILES ICONS ${CMAKE_SOURCE_DIR}/src/assets/icon.ico ${CMAKE_SOURCE_DIR}/src/assets/icon.icns)
if (MSVC)
    add_executable(${TARGET_NAME} WIN32 ${SOURCES} ${SOURCE_FILES} ${HEADERS})
elseif(APPLE)
    add_executable(${TARGET_NAME} MACOSX_BUNDLE ${SOURCES} ${SOURCE_FILES} ${HEADERS})
else()
    message(FATAL_ERROR "Unsupported platform, currently we are only supporting MacOS / Windows")
endif()


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

# ================================ #
# === Set up test  environment === #
# ================================ #
if (BUILD_TESTS)
    message(STATUS "Building tests")
    add_subdirectory(tests)
endif ()