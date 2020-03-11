project(Graphics3d)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_library(${PROJECT_NAME} 
	# 
	include/Graphics3d/GlInfo.h
	src/GlInfo.cpp
	include/Graphics3d/SharedPtrTypes.h
	
	# Assets
	include/Graphics3d/Assets/Material.h
	src/Assets/Material.cpp
	include/Graphics3d/Assets/MaterialManager.h
	src/Assets/MaterialManager.cpp
	include/Graphics3d/Assets/Mesh.h
	src/Assets/Mesh.cpp
	include/Graphics3d/Assets/MeshManager.h
	src/Assets/MeshManager.cpp
	include/Graphics3d/Assets/Shader.h
	src/Assets/Shader.cpp
	include/Graphics3d/Assets/ShaderManager.h
	src/Assets/ShaderManager.cpp
	include/Graphics3d/Assets/Texture.h
	src/Assets/Texture.cpp
	include/Graphics3d/Assets/TextureManager.h
	src/Assets/TextureManager.cpp

	# Assets/Materials
	include/Graphics3d/Assets/Materials/DefaultDepthMaterial.h
	src/Assets/Materials/DefaultDepthMaterial.cpp
	include/Graphics3d/Assets/Materials/DiffuseMaterial.h
	src/Assets/Materials/DiffuseMaterial.cpp
	include/Graphics3d/Assets/Materials/PBRMaterial.h
	src/Assets/Materials/PBRMaterial.cpp
	include/Graphics3d/Assets/Materials/PBRTexturedMaterial.h
	src/Assets/Materials/PBRTexturedMaterial.cpp
	include/Graphics3d/Assets/Materials/SpecularMaterial.h
	src/Assets/Materials/SpecularMaterial.cpp
	include/Graphics3d/Assets/Materials/TerrainDepthMaterial.h
	src/Assets/Materials/TerrainDepthMaterial.cpp
	include/Graphics3d/Assets/Materials/TerrainMaterial.h
	src/Assets/Materials/TerrainMaterial.cpp

	# Components
	include/Graphics3d/Components/Camera.h
	src/Components/Camera.cpp
	include/Graphics3d/Components/CameraController.h
	src/Components/CameraController.cpp
	include/Graphics3d/Components/Component.h
	src/Components/Component.cpp
	include/Graphics3d/Components/Light.h
	src/Components/Light.cpp
	include/Graphics3d/Components/MeshRenderer.h
	src/Components/MeshRenderer.cpp
	include/Graphics3d/Components/Transform.h
	src/Components/Transform.cpp

	# Importer
	include/Graphics3d/Importer/ObjImporter.h
	src/Importer/ObjImporter.cpp
	include/Graphics3d/Importer/stb_image.h
	src/Importer/stb_image.cpp

	# Input
	include/Graphics3d/Input/InputManager.h
	src/Input/InputManager.cpp

	# Rendering
	include/Graphics3d/Rendering/FrameBufferObject.h
	src/Rendering/FrameBufferObject.cpp
	include/Graphics3d/Rendering/RenderPath.h
	src/Rendering/RenderPath.cpp
	include/Graphics3d/Rendering/SceneRenderer.h
	src/Rendering/SceneRenderer.cpp

	# Rendering/RenderPaths
	include/Graphics3d/Rendering/RenderPaths/DeferredShadingPath.h
	src/Rendering/RenderPaths/DeferredShadingPath.cpp
	include/Graphics3d/Rendering/RenderPaths/FinalShadingPath.h
	src/Rendering/RenderPaths/FinalShadingPath.cpp
	include/Graphics3d/Rendering/RenderPaths/GraphicsBufferPath.h
	src/Rendering/RenderPaths/GraphicsBufferPath.cpp
	include/Graphics3d/Rendering/RenderPaths/ShadowMapPath.h
	src/Rendering/RenderPaths/ShadowMapPath.cpp

	# Resources

	# Scene
	include/Graphics3d/Scene/SceneObject.h
	src/Scene/SceneObject.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ./include)
target_link_libraries(${PROJECT_NAME} PUBLIC glm GLEW::GLEW)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED on)
target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)

				
