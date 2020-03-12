/**
 * @author Florian Feuerstein
 * @date 03.08.2016
 *
 * Forward declarations and typedefs of g3d lib.
 */

#pragma once

#include <memory>

namespace g3d
{

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;
typedef std::shared_ptr<Mesh const> MeshCPtr;

class Shader;
typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<Shader const> ShaderCPtr;

class Material;
typedef std::shared_ptr<Material> MaterialPtr;
typedef std::shared_ptr<Material const> MaterialCPtr;

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Texture const> TextureCPtr;

} // namespace g3d
