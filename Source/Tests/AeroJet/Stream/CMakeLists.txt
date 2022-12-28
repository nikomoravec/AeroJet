add_executable(test_AeroJet_StreamReader Reader_Test.cpp)
target_link_libraries(test_AeroJet_StreamReader AeroJet)
message(STATUS ${CMAKE_BINARY_DIR})
add_test(NAME test_AeroJet_StreamReader COMMAND test_AeroJet_StreamReader WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Source/Tests/AeroJet/Stream")