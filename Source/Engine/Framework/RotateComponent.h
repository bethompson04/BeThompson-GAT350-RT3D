#pragma once
#include "Components/Component.h"
#include <glm/glm/glm.hpp>
#include "Renderer/Program.h"

namespace nc
{
	class RotateComponent : public Component
	{
	public:
		CLASS_DECLARATION(RotateComponent);

		bool Initialize() override;
		void Update(float dt) override;
		void SetProgram(const res_t<Program> program);
		void ProcessGui();


	private:
		glm::vec3 euler{ 1 };
	};
}