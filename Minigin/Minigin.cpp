#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "RenderComponent.h"
#include "ResourceManager.h"

#include "ObjectConstructors.h"
#pragma warning(push)
#pragma warning(disable:26812)
#pragma warning(disable:26819)
#include "SDL.h"
#include "TextComponent.h"
#pragma warning(pop)


using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{

	
	//intialization
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_pWindow == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}


	Renderer::GetInstance().Init(m_pWindow);
	m_pTimer = new FPSTimer();
	m_pTimer->start();

	m_pInputManager = new input::InputManager();

	

	m_qbert = new QBert;
	m_qbert->GetSubject()->AddObserver(&m_qbertobserver);
	
	//InputManager
	Action newAction = Action();
	newAction.pCommand = new DieCommand(m_qbert);
	newAction.Button = input::XBoxController::ControllerButton::ButtonA;
	newAction.type = InputType::down;
	m_pInputManager->AddAction(newAction);

	newAction = Action();
	newAction.pCommand = new FireCommand(m_qbert);
	newAction.Button = input::XBoxController::ControllerButton::ButtonB;
	newAction.type = InputType::Pressed;
	
	m_pInputManager->AddAction(newAction);
	
	newAction = Action();
	newAction.pCommand = new JumpCommand(m_qbert);
	newAction.Button = input::XBoxController::ControllerButton::ButtonX;
	newAction.type = InputType::Up;
	m_pInputManager->AddAction(newAction);
}

/**
 * Code constructing the scene world starts here
 */
void dae::Minigin::LoadGame()
{
	auto scene = SceneManager::GetInstance().CreateScene("Demo");

	
	auto background = objectConstructors::RenderObject("background.jpg");
	scene->Add(background);
	
	auto logo = objectConstructors::RenderObject("logo.png", 216, 180);
	scene->Add(logo);

	auto to = objectConstructors::TextObject("lingua.otf", 36, "Programming 4 Assignment",80,20);
	scene->Add(to);


	auto font2 = ResourceManager::GetInstance().LoadFont("lingua.otf", 12);
	m_FPSText = std::make_shared<TextObject>("FPS time", font2);
	m_FPSText->SetPosition(10, 10);
	m_FPSText->SetColor(SDL_Color{ 255, 255, 0, 255 });
	scene->Add(m_FPSText);

	

}

void dae::Minigin::Update()
{
	m_pTimer->Update();
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.Update(m_pTimer->GetDeltaTime());
	m_pInputManager->Update();

}

void dae::Minigin::Render()
{
	auto& sceneManager = SceneManager::GetInstance();
	sceneManager.Render();
}

void dae::Minigin::Cleanup()
{
	delete m_pTimer;
	delete m_qbert;
	delete m_pInputManager;
	
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	float printTimer = 0.f;

	LoadGame();
	{
		//auto& Scene = SceneManager::GetInstance();
		auto& renderer = Renderer::GetInstance();
		auto& input = input::InputManager::GetInstance();

		bool doContinue = true;
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();


			doContinue = input.ProcessInput();
			Update();
			renderer.Render();
			Render();
			
			printTimer += m_pTimer->GetDeltaTime();
			if (printTimer > 1.f)
			{
				printTimer -= 1.f;
				std::cout << m_pTimer->GetFPS() << std::endl;

				m_FPSText->SetText("FPS: " + std::to_string(m_pTimer->GetFPS()));
	
			}

	/*		auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);*/
		}
	}

	Cleanup();
}