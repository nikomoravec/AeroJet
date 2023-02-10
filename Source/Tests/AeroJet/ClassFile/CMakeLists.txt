#
# CMakeLists.txt
# Copyright © 2023 AeroJet Developers. All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
# OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

add_executable(test_AeroJet_ClassInfo ClassInfo.cpp)
add_executable(test_AeroJet_ExceptionsAttribute ExceptionsAttribute.cpp)
add_executable(test_AeroJet_InnerClassesAttribute InnerClassesAttributeTest.cpp)
add_executable(test_AeroJet_RuntimeVisibleAnnotations RuntimeVisibleAnnotationsTest.cpp)

add_custom_command(
        TARGET test_AeroJet_ClassInfo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/TestJavaBytecodeTableSwitch.class
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/TestJavaBytecodeTableSwitch.class)

add_custom_command(
        TARGET test_AeroJet_ExceptionsAttribute POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/TestExceptionsAttribute.class
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/TestExceptionsAttribute.class)

add_custom_command(
        TARGET test_AeroJet_InnerClassesAttribute POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/TestInnerClassesAttribute.class
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/TestInnerClassesAttribute.class)

add_custom_command(
        TARGET test_AeroJet_RuntimeVisibleAnnotations POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/RuntimeVisibleAnnotationsTest.class
        ${CMAKE_CURRENT_BINARY_DIR}/Resources/RuntimeVisibleAnnotationsTest.class)

add_test(NAME test_AeroJet_ClassInfo COMMAND test_AeroJet_ClassInfo)
add_test(NAME test_AeroJet_ExceptionsAttribute COMMAND test_AeroJet_ExceptionsAttribute)
add_test(NAME test_AeroJet_InnerClassesAttribute COMMAND test_AeroJet_InnerClassesAttribute)
add_test(NAME test_AeroJet_RuntimeVisibleAnnotations COMMAND test_AeroJet_RuntimeVisibleAnnotations)
