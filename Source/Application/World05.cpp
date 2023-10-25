#include "World05.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

#include "glm/glm/gtc/type_ptr.hpp"
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace nc
{
    bool World05::Initialize()
    {
        m_scene = std::make_unique<Scene>();

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelComponent);
            modelComponent->m_model = std::make_shared<Model>();
            modelComponent->m_model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
            modelComponent->m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "light1";
            actor->transform.position = glm::vec3{ 3, 3, 3 };
            auto lightComponent = CREATE_CLASS(LightComponent);
            lightComponent->type = LightComponent::eType::Point;
            lightComponent->color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
            lightComponent->intensity = 1;
            lightComponent->range = 20;
            lightComponent->innerAngle = 10.0f;
            lightComponent->outerAngle = 30.0f;
            actor->AddComponent(std::move(lightComponent));
            m_scene->Add(std::move(actor));
        }
        return true;
    }

    void World05::Shutdown()
    {
    }

    void World05::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        //m_scene->ProcessGui();

        // Silly Input
        auto actor = m_scene->GetActorByName<Actor>("actor1");

        // Position Inputs
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;

        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_R) ? m_speed * -dt : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_F) ? m_speed * +dt : 0;

        actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * +dt : 0;
        actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;
        m_time += dt;


        auto material = actor->GetComponent<ModelComponent>()->m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();

        material->GetProgram()->SetUniform("ambientLightColor", ambientLightColor);

        //// Model
        //material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        // View
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 5}, glm::vec3{0,0,0}, glm::vec3{0,1,0});
        material->GetProgram()->SetUniform("view", view);

        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), (float)ENGINE.Instance().GetSystem<Renderer>()->GetWidth() / (float)ENGINE.Instance().GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer)
    {
        // Pre-Render
        renderer.BeginFrame();

        // Render
        
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_scene->Draw(renderer);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        ENGINE.GetSystem<Gui>()->Draw();

        // Post-Render
        renderer.EndFrame();
    }
}
