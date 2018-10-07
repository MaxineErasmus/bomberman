#include "../includes/group.hpp"

namespace GameBoi { namespace graphics {

	Group::Group(const glm::mat4& transform) : m_TransformationMatrix(transform) {

	}

	Group::~Group(void){
		return;
	}

	Group::Group(const Group &src){
		static_cast<void>(src);
		return;
	}

	Group &Group::operator=(const Group &rhs){
		static_cast<void>(rhs);
		return *this;
	}

	void	Group::Submit(Renderer2D* renderer) const  {

		renderer->push(m_TransformationMatrix);
		for (const Renderable2D* renderable : m_Renderables)
			renderable->Submit(renderer);
		
		renderer->pop();
	}
	void	Group::add(Renderable2D* renderable) {
		m_Renderables.push_back(renderable);
	}

}}

