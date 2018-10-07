#pragma once

#include <glad/glad.h>
#include <iostream>
#include <vector>

namespace GameBoi { namespace graphics {

	class Skybox {

	public:
		Skybox(std::vector<std::string> faces);
		Skybox(const Skybox &src);
		Skybox &operator=(const Skybox &rhs);
		void    Draw();
		~Skybox();
	private:
		void    Init();
		unsigned int loadCubemap(std::vector<std::string> faces);
		unsigned int skyboxVAO, skyboxVBO;
		std::vector<std::string> faces;
	};
}}