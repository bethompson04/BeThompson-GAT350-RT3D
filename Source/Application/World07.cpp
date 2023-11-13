#include "World07.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Core/Core.h"

#include "glm/glm/gtc/type_ptr.hpp"
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace nc
{
    bool World07::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }

        return true;
    }

    void World07::Shutdown()
    {
    }

    void World07::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        ImGui::Begin("Post Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);

        bool effect = params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            (effect) ? params |= INVERT_MASK : params ^= INVERT_MASK;
        }

        effect = params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect))
        {
            (effect) ? params |= GRAYSCALE_MASK : params ^= GRAYSCALE_MASK;
        }

        effect = params & COLORTINT_MASK;
        if (ImGui::Checkbox("Colortint", &effect))
        {
            (effect) ? params |= COLORTINT_MASK : params ^= COLORTINT_MASK;
        }
        ImGui::ColorEdit3("Tint", glm::value_ptr(m_tint));

        effect = params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect))
        {
            (effect) ? params |= GRAIN_MASK : params ^= GRAIN_MASK;
        }

        effect = params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect))
        {
            (effect) ? params |= SCANLINE_MASK : params ^= SCANLINE_MASK;
        }

        effect = params & DISTORT_MASK;
        if (ImGui::Checkbox("Distortion", &effect))
        {
            (effect) ? params |= DISTORT_MASK : params ^= DISTORT_MASK;
        }
        ImGui::End();


        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("tint", m_tint);
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", params);
            program->SetUniform("time", m_time);
            GLfloat move = (GL_TIME_ELAPSED) / 1000.0 * 2 * 3.14159 * 0.75;
            program->SetUniform("offset", move);
        }

      
        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer)
    {
        // ** PASS ONE **
        m_scene->GetActorByName("postprocess")->active = false;
        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();



        renderer.BeginFrame({0,0,0});
        m_scene->Draw(renderer);

        framebuffer->Unbind();
        
        // ** PASS TWO **
        m_scene->GetActorByName("postprocess")->active = true;
        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        // Post-Render
        renderer.EndFrame();
    }
}
