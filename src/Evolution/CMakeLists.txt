project(Evolution)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} 
	main.cpp
	Individual.h
	Individual.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW Math Graphics3d)


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED on)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)

				
