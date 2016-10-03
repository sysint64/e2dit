#pragma once

#include "renderer/sprite.h"
#include "renderer/shader.h"
#include "renderer/texture.h"

#include <vector>

class R {
public:

	class Textures {
	public:

		static std::unique_ptr<gapi::Texture> skin;
		static std::unique_ptr<gapi::Texture> gridLight;
		static std::unique_ptr<gapi::Texture> gridDark;

		static void init (Application *app) {

			skin      = std::make_unique<gapi::Texture> (app->resPath+"/ui/skins/light/controls.png");
			gridLight = std::make_unique<gapi::Texture> (app->resPath+"/ui/icons/transparent_grid_light.png", GL_LINEAR, GL_REPEAT, GL_REPEAT);
			gridDark  = std::make_unique<gapi::Texture> (app->resPath+"/ui/icons/transparent_grid_dark.png" , GL_LINEAR, GL_REPEAT, GL_REPEAT);

		}

	};

	class Shaders {
	public:

		static std::unique_ptr<gapi::Shader> atlasShader;
		static std::unique_ptr<gapi::Shader> colorShader;
		static std::unique_ptr<gapi::Shader> atlasMaskShader;
		static std::unique_ptr<gapi::Shader> colorLineShader;
		static std::unique_ptr<gapi::Shader> colorPickerShader;

		static void init (Application *app) {

			std::vector<std::string> atlasMaskLocations   {"MVP", "Size"   , "Offset" , "Texture", "Alpha", "Mask", "MaskOffset", "MaskSize"};
			std::vector<std::string> atlasLocations       {"MVP", "Size"   , "Offset" , "Texture", "Alpha"};
			std::vector<std::string> colorLineLocations   {"MVP", "palette", "RGB"    , "HSB"};
			std::vector<std::string> colorPickerLocations {"MVP", "palette", "RGB"    , "HSB"};
			std::vector<std::string> colorLocations       {"MVP", "Color"};

			std::string GL = "GL"+std::to_string(app->OGLMajor);

			atlasShader       = std::make_unique<gapi::Shader> (app->resPath+"/glsl/"+GL+"/tex_atlas.glsl"     , atlasLocations);
			colorShader       = std::make_unique<gapi::Shader> (app->resPath+"/glsl/"+GL+"/colorize.glsl"      , colorLocations);
			atlasMaskShader   = std::make_unique<gapi::Shader> (app->resPath+"/glsl/"+GL+"/tex_atlas_mask.glsl", atlasMaskLocations);
			colorLineShader   = std::make_unique<gapi::Shader> (app->resPath+"/glsl/"+GL+"/color_line.glsl"    , colorLineLocations);
			colorPickerShader = std::make_unique<gapi::Shader> (app->resPath+"/glsl/"+GL+"/color_picker.glsl"  , colorPickerLocations);

		}

	};

	static void init (Application *app) {
		Textures::init(app);
		Shaders ::init(app);
	}

};
