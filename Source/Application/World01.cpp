#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        //m_positions.push_back({ randomf(-1, 1) }, { randomf(-1, 1) });
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_angle += 45 * dt;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt : 0;

        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? +dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;

        if (color1 < 0) color1 = 1; else color1 -= dt / 3;
        if (color2 < 0) color1 = 1; else color2 -= dt / 10;
        if (color3 < 0) color1 = 1; else color3 -= dt / 8;

        m_time += dt;
    }

    void World01::Draw(Renderer& renderer)
    {
        // Pre-Render
        renderer.BeginFrame();

        // Render
        
      
        glPushMatrix();
        glTranslatef(m_position.x, m_position.y, 0);
        glRotatef(m_angle, 1, 0, 0);
        glScalef((sin(m_time * 10) + 1) * 0.5f, 1, 1);
        
        glBegin(GL_TRIANGLES);

         glColor3f(color1,0,0);
         glVertex2f(-0.5f, -0.5f);

         glColor3f(0, 0, color3);
         glVertex2f(0, 0.5f);

         glColor3f(0, color2, 0);
         glVertex2f(0.5f, -0.5f);
         glEnd();
            
            //c1 = c1 + 0.5f;
         glPopMatrix();
        


        // Post-Render
        renderer.EndFrame();
    }
}
