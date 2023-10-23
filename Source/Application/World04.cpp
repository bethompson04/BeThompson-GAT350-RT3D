#include "World04.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

#include "glm/glm/gtc/type_ptr.hpp"

#define INTERLEAVE

namespace nc
{
    bool World04::Initialize()
    {
        auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("Models/plane.obj", glm::vec3{0}, glm::vec3{0, 0, 0});
        m_transform.position.y = -1;

        m_light.type = light_t::eType::Point;
        m_light.position = glm::vec3{ 0, 5, 0 };
        m_light.direction = glm::vec3{ 0, -1, 0 };
        m_light.color = glm::vec3{ 1 };
        m_light.cutoff = 30.0f;

        
		//READ_DATA(document, ambientLightColor);
		//READ_DATA(document, lightPosition);
		//READ_DATA(document, diffusedLightColor);
 
        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::End();

        ImGui::Begin("Lighting");
        const char* types[] = {"Point", "Directional", "Spot"};
        ImGui::Combo("Type", (int*)& m_light.type, types, 3);

        if (m_light.type != light_t::Directional)ImGui::DragFloat3("Position", glm::value_ptr(m_light.position), 0.1f);
        if (m_light.type != light_t::Point)ImGui::DragFloat3("Direction", glm::value_ptr(m_light.direction), 0.1f);
        if (m_light.type != light_t::Spot)ImGui::DragFloat("Cutoff", &m_light.cutoff, 1, 0, 90);
        ImGui::ColorEdit3("Diffused Color", glm::value_ptr(m_light.color), 0.001f);

        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(ambientLightColor), 0.001f);
        ImGui::End();




        // Silly Input

        // Position Inputs
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_R) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_F) ? m_speed * +dt : 0;

        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * +dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;
        m_time += dt;


        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();

        
        material->GetProgram()->SetUniform("light.type", m_light.type);
        material->GetProgram()->SetUniform("light.lightPosition", m_light.position);
        material->GetProgram()->SetUniform("light.direction", m_light.direction);
        material->GetProgram()->SetUniform("ambientLightColor", ambientLightColor);
        material->GetProgram()->SetUniform("light.diffusedLightColor", m_light.color);
        material->GetProgram()->SetUniform("light.cutoff", m_light.cutoff);

        // Model
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        // View
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 5}, glm::vec3{0,0,0}, glm::vec3{0,1,0});
        material->GetProgram()->SetUniform("view", view);

        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)ENGINE.Instance().GetSystem<Renderer>()->GetWidth() / (float)ENGINE.Instance().GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // Pre-Render
        renderer.BeginFrame();

        // Render
        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        ENGINE.GetSystem<Gui>()->Draw();

        // Post-Render
        renderer.EndFrame();
    }
}
