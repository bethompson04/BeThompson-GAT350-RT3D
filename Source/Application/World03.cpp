#include "World03.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

#include "glm/glm/gtc/type_ptr.hpp"

#define INTERLEAVE



namespace nc
{
    bool World03::Initialize()
    {

        m_program = GET_RESOURCE(Program, "Shaders/unlit_color.prog");
        m_program->Use();

#ifdef INTERLEAVE

        // Vertex Data
        float vertexData[] = {
            -0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f, // vertex 1
             0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, // vertex 2
            -0.8f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f, // vertex 3
             0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 1.0f
        };

        GLuint vbo; // Vertex Array Object
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));

        // position_data
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        // Color Data
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

#else
        // Vertex Data
        float positionData[] = {
            -0.8f, -0.8f, 0.0f,
             0.8f, -0.8f, 0.0f,
            -0.8f,  0.8f, 0.0f,
             0.8f,  0.8f, 0.0f
    };

        float colorData[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f
        };

        GLuint vbo[2]; // Vertex Array Object
        glGenBuffers(2, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // position_data
        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));
        glVertexAttribBinding(0, 0);

        // color_data
        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 1);


#endif
        //m_position.z = -10.0f;
        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::End();

        m_transform.rotation.z += 720 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * +dt : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_R) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_F) ? m_speed * +dt : 0;

        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * +dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * -dt : 0;
        m_time += dt;


        // Model
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_angle), glm::vec3{0, 0, 1});
        //glm::mat4 model = position * rotation;
        
        m_program->SetUniform("model", m_transform.GetMatrix());

        // View
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 5, 5}, glm::vec3{0,0,0}, glm::vec3{0,1,0});
        m_program->SetUniform("view", view);

        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        m_program->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        // Pre-Render
        renderer.BeginFrame();

        // Render
        
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        ENGINE.GetSystem<Gui>()->Draw();

        // Post-Render
        renderer.EndFrame();
    }
}
