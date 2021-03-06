#include "../includes/layer.hpp"

namespace GameBoi { namespace graphics {

	Layer::Layer(Renderer2D* renderer, Shader* shader, glm::mat4 projectionMatrix) \
	: m_Renderer(renderer), m_Shader(shader), m_ProjectionMatrix(projectionMatrix) {

		m_Shader->enable();
		m_Shader->setuniformMat4("pr_matrix", m_ProjectionMatrix);
		m_Shader->disable();

	}

	Layer::Layer(const Layer &src){
		static_cast<void>(src);
		return;
	}

	Layer &Layer::operator=(const Layer &rhs){
		static_cast<void>(rhs);
		return *this;
	}

	void	Layer::add(Renderable2D* renderable) {
		m_Renderables.push_back(renderable);
	}

	Layer::~Layer() {
		delete	m_Shader;
		delete	m_Renderer;
		
		for (size_t i = 0; i < m_Renderables.size(); i++)
			delete	m_Renderables[i];
	}

	void	Layer::render() {
		
		m_Shader->enable();
		m_Renderer->begin();
		for (const Renderable2D* renderable : m_Renderables)
				renderable->Submit(m_Renderer);
		m_Renderer->end();
		m_Renderer->Flush();
		
	}

}}