#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{


	class World08 : public World
	{
	public:
		const uint32_t INVERT_MASK =    (1 << 0);
		const uint32_t GRAYSCALE_MASK = (1 << 1);
		const uint32_t COLORTINT_MASK = (1 << 2);
		const uint32_t GRAIN_MASK =     (1 << 3);
		const uint32_t SCANLINE_MASK =  (1 << 4);
		const uint32_t DISTORT_MASK =   (1 << 5);
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time = 0;
		float m_speed = 10;
		float m_blend{ 1 };

		int m_celLevels = 5;
		float m_celSpecularCutoff = 0.3;
		float m_celOutline = 0.3;
		glm::vec3 m_outlineColor = { 0,0,0 };



		glm::vec3 m_tint{ 1 };

		uint32_t params{ 0 };

		float m_refraction{ 1 };


	};
}