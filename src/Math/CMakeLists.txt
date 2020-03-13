project(Math)

find_package(eigen3 REQUIRED)

add_library(${PROJECT_NAME} 
	include/Math/MathTypes.h
	include/Math/SomeTest.h
	src/SomeTest.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ./include)
target_link_libraries(${PROJECT_NAME} PUBLIC Eigen3::Eigen)


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED on)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
target_compile_definitions(${PROJECT_NAME} PUBLIC _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

				
