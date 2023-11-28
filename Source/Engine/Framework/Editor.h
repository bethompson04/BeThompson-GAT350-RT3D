#pragma once
namespace nc
{
	class Editor
	{
	public:
		void ProcessGui(class Scene* scene);
		void Update();

	private:
		class Object* m_selected = nullptr;
		bool m_active = true;
	};
}