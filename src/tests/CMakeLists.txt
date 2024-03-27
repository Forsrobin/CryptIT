add_executable(crypt-it-test
        crypto_test.cc
        helper_test.cc
        ../crypto.cpp
        ../helper.cpp
        )

target_link_libraries(crypt-it-test
        cryptopp::cryptopp 
        gtest::gtest
        )

message(STATUS " CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

add_test(NAME crypt-it-test
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/src/tests
        COMMAND crypt-it-test
        )