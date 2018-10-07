#include "../includes/tilelayer.hpp"

namespace GameBoi { namespace graphics {

	TileLayer::TileLayer(Shader* shader) \
	: Layer(new BatchRenderer2D(), shader, glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)) {

	}

	TileLayer::~TileLayer() {
	
	}

	TileLayer::TileLayer(const TileLayer &src){
		static_cast<void>(src);
		return;
	}

	TileLayer &TileLayer::operator=(const TileLayer &rhs){
		static_cast<void>(rhs);
		return *this;
	}
}}