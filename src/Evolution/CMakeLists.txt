project(Evolution)

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(${PROJECT_NAME} 
	main.cpp
	World.h
	World.cpp
	Person.h
	Person.cpp
	Apple.h
	Apple.cpp
	Movement.h
	Movement.cpp
	EvoSim.h
	EvoSim.cpp
	PopulationCounter.h
	PopulationCounter.cpp
	Terrain.h
	Terrain.cpp
	
	Math/MathTypes.h

	Input/InputManager.h
	Input/InputManager.cpp

	Importer/stb_image.h
	Importer/stb_image.cpp
	Importer/ObjImporter.h
	Importer/ObjImporter.cpp
	
	Graphics/GlInfo.h
	Graphics/GlInfo.cpp
	Graphics/Camera.h
	Graphics/Camera.cpp
	Graphics/Texture.h
	Graphics/Texture.cpp
	Graphics/Mesh.h
	Graphics/Mesh.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW)
target_include_directories(${PROJECT_NAME} PRIVATE ./)


set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED on)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
target_compile_definitions(${PROJECT_NAME} PUBLIC _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)

				
