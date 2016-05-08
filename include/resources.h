#pragma once

#include "renderer/sprite.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

#include <vector>

class R {
public:

	class Textures {
	public:

		static std::unique_ptr<Texture> skin;
		static std::unique_ptr<Texture> gridLight;
		static std::unique_ptr<Texture> gridDark;

		static void init() {

			skin      = std::make_unique<Texture> ("../res/ui/skins/light/controls.png");
			gridLight = std::make_unique<Texture> ("../res/ui/icons/transparent_grid_light.png", GL_LINEAR, GL_REPEAT, GL_REPEAT);
			gridDark  = std::make_unique<Texture> ("../res/ui/icons/transparent_grid_dark.png" , GL_LINEAR, GL_REPEAT, GL_REPEAT);

		}

	};

	class Shaders {
	public:

		static std::unique_ptr<Shader> atlasShader;
		static std::unique_ptr<Shader> colorShader;
		static std::unique_ptr<Shader> atlasMaskShader;

		static void init() {

			std::vector<std::string> atlasMaskLocations {"MVP", "Size", "Offset", "Texture", "Alpha", "Mask", "MaskOffset", "MaskSize"};
			std::vector<std::string> atlasLocations     {"MVP", "Size", "Offset", "Texture", "Alpha"};
			std::vector<std::string> colorLocations     {"MVP", "Color"};

			atlasMaskShader = std::make_unique<Shader> ("../res/glsl/GL2/tex_atlas_mask.glsl", atlasMaskLocations);
			atlasShader     = std::make_unique<Shader> ("../res/glsl/GL2/tex_atlas.glsl"     , atlasLocations);
			colorShader     = std::make_unique<Shader> ("../res/glsl/GL2/colorize.glsl"      , colorLocations);

		}

	};

	static void init() {
		Textures::init();
		Shaders::init();
	}

};
