#include "resources.h"

std::unique_ptr<Texture> R::Textures::skin = nullptr;
std::unique_ptr<Texture> R::Textures::grid = nullptr;

std::unique_ptr<Shader> R::Shaders::atlasShader     = nullptr;
std::unique_ptr<Shader> R::Shaders::colorShader     = nullptr;
std::unique_ptr<Shader> R::Shaders::atlasMaskShader = nullptr;
