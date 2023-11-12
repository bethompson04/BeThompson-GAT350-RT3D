#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "World06.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

using namespace std;

int main(int argc, char* argv[])
{
//	nc::StringUtils test;
//	std::string testStr = "BaZinGa";
//	std::string testStr2 = "bAzinGa";
//	std::string testStr3 = "bEzinGa";
//	cout << testStr << "\n";
//	cout << test.ToLower(testStr) << "\n";
//	cout << test.ToUpper(testStr) << "\n";
//	cout << test.IsEqualIgnoreCase(testStr, testStr2) << "\n";
//	cout << test.IsEqualIgnoreCase(testStr, testStr3) << "\n";
//	cout << test.CreateUnique(testStr) << "\n";
//	cout << test.CreateUnique(testStr2) << "\n";
//	cout << test.CreateUnique(testStr3) << "\n";
//	cout << testStr << "\n";
//	cout << test.ToLower(testStr2) << "\n";
//	cout << test.ToUpper(testStr3) << "\n";


	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");

	ENGINE.Initialize();

	auto world = make_unique<nc::World06>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit)
	{
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();

	return 0;
}
