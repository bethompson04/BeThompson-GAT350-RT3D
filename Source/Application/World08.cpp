#include "World08.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Core/Core.h"

#include "glm/glm/gtc/type_ptr.hpp"
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace nc
{
    bool World08::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_editor.json");
        m_scene->Load("scenes/scene_cel.json");
        m_scene->Initialize();

        m_editor = std::make_unique<Editor>();

        // depth texture create
        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);

        // depth buffer create :O
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthBuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        // Set depth texture to debug sprite
        auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        auto materials = GET_RESOURCES(Material);
        for (auto material : materials)
        {
            material->depthTexture = texture;
        }

        return true;
    }

    void World08::Shutdown()
    {
    }

    void World08::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();
        
        m_scene->Update(dt);

        m_editor->Update();
        m_editor->ProcessGui(m_scene.get());

        ImGui::Begin("Cel Shader");
        ImGui::DragInt("Levels", &m_celLevels, 1, 0, 20);
        ImGui::DragFloat("Specular Cutoff", &m_celSpecularCutoff);
        ImGui::DragFloat("Outline", &m_celOutline);
        ImGui::ColorEdit3("Outline Color", glm::value_ptr(m_outlineColor));

        ImGui::End();
        auto program = GET_RESOURCE(Program, "shaders/lit_cel.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("celLevels", m_celLevels);
            program->SetUniform("celSpecularCutoff", m_celSpecularCutoff);
            program->SetUniform("celOutline", m_celOutline);
            program->SetUniform("outlineColor", m_outlineColor);

        }
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World08::Draw(Renderer& renderer)
    {
        // ** PASS ONE **
        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights)
        {
            if (light->castShadow)
            {
                glm::mat4 shadowMatrix = light->getShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }

        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto model : models)
        {
            if (model->castShadow)
            {
                //glCullFace(GL_FRONT);
                program->SetUniform("model", model->m_owner->transform.GetMatrix());
                model->m_model->Draw();
            }
        }

        framebuffer->Unbind();
        
        // ** PASS TWO **

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // Post-Render
        renderer.EndFrame();
    }
}
