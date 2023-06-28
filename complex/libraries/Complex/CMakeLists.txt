project(Complex)

add_library(${PROJECT_NAME} include/${PROJECT_NAME}.hpp src/${PROJECT_NAME}.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(HW5::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(tests tests/test_template.hpp
        tests/test_template.cpp
        tests/test_constructors.cpp
        tests/test_unary_operators.cpp
        tests/test_same_object_operators.cpp
        tests/test_methods.cpp
        tests/test_binary_operators.cpp
        tests/test_assignment.cpp)

if (NOT TESTS_BUILD_TYPE MATCHES NONE)
    list(REMOVE_ITEM tests tests/test_same_object_operators.cpp)
endif ()

add_executable(ComplexTests ${tests})

find_package(Catch2 REQUIRED)

target_link_libraries(ComplexTests PRIVATE Catch2::Catch2WithMain HW5::${PROJECT_NAME})

if (COMPILE_OPTS)
    target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILE_OPTS})
    target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTS})

    target_compile_options(ComplexTests PUBLIC ${COMPILE_OPTS})
    target_link_options(ComplexTests PUBLIC ${LINK_OPTS})
endif ()
