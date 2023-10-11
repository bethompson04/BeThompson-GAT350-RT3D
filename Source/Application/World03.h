#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include <vector>

namespace nc
{
	class World03 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 10;

		Transform m_transform;

		GLuint m_vao = 0;
		res_t<Program> m_program;
		res_t<Texture> m_texture;
	};
}