#include "resources.h"

std::unique_ptr<Texture> R::Textures::skin      = nullptr;
std::unique_ptr<Texture> R::Textures::gridLight = nullptr;
std::unique_ptr<Texture> R::Textures::gridDark  = nullptr;

std::unique_ptr<Shader> R::Shaders::atlasShader       = nullptr;
std::unique_ptr<Shader> R::Shaders::colorShader       = nullptr;
std::unique_ptr<Shader> R::Shaders::atlasMaskShader   = nullptr;
std::unique_ptr<Shader> R::Shaders::colorLineShader   = nullptr;
std::unique_ptr<Shader> R::Shaders::colorPickerShader = nullptr;
