#include "Material.h"
#include "Program.h"
#include "Texture.h"
#include "Core/Core.h"

namespace nc
{
	bool Material::Create(std::string filename, ...)
	{
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if (!success)
		{
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		m_program = GET_RESOURCE(Program, program);

		// read the textures name
		std::vector<std::string> textures;
		READ_DATA(document, textures);
		for (auto texture : textures)
		{
			// get texture resource
			m_textures.push_back(GET_RESOURCE(Texture, texture));
		}

		READ_DATA(document, color);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		READ_DATA(document, ambientLightColor);
		READ_DATA(document, lightPosition);
		READ_DATA(document, diffusedLightColor);

		return true;
	}

	void Material::Bind()
	{
		m_program->Use();
		m_program->SetUniform("material.color", color);
		m_program->SetUniform("material.tiling", tiling);
		m_program->SetUniform("material.offset", offset);

		m_program->SetUniform("light.lightPosition", lightPosition);
		m_program->SetUniform("light.ambientLightColor", ambientLightColor);
		m_program->SetUniform("light.diffusedLightColor", diffusedLightColor);


		for (size_t i = 0; i < m_textures.size(); i++)
		{
			m_textures[i]->SetActive(GL_TEXTURE0 + (int)i);
			m_textures[i]->Bind();
		}
	}

	void Material::ProcessGui()
	{
		ImGui::Begin("Material");

		ImGui::ColorEdit4("Color", glm::value_ptr(color));
		ImGui::DragFloat2("Tiling", glm::value_ptr(tiling), 0.1f);
		ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.1f);

		ImGui::End();

		ImGui::Begin("Lighting");

		ImGui::DragFloat3("Position", glm::value_ptr(lightPosition), 0.1f);
		ImGui::DragFloat3("Ambient Color", glm::value_ptr(ambientLightColor), 0.001f);
		ImGui::DragFloat3("Diffused Color", glm::value_ptr(diffusedLightColor), 0.001f);

		ImGui::End();
	}
}