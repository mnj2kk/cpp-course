project(Expressions)

add_library(${PROJECT_NAME} include/${PROJECT_NAME}.hpp src/${PROJECT_NAME}.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include)

add_library(HW5::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

add_executable(ExpressionTests tests/test_simple_objects.cpp
        tests/test_template.hpp
        tests/test_template.cpp
        tests/test_unit_expressions.cpp)

find_package(Catch2 REQUIRED)

target_link_libraries(ExpressionTests PRIVATE Catch2::Catch2WithMain HW5::${PROJECT_NAME})

if (COMPILE_OPTS)
    target_compile_options(${PROJECT_NAME} PUBLIC ${COMPILE_OPTS})
    target_link_options(${PROJECT_NAME} PUBLIC ${LINK_OPTS})

    target_compile_options(ExpressionTests PUBLIC ${COMPILE_OPTS})
    target_link_options(ExpressionTests PUBLIC ${LINK_OPTS})
endif ()
