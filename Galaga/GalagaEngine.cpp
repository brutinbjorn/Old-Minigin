#include "GalagaEngine.h"


#include "HealthBarComponent.h"
#include "Locator.h"
#include "ObjectConstructors.h"
#include "GalagaConstructor.h"
#include "AllComponents.h"



void GalagaEngine::LoadGame()
{
	auto scene = dae::SceneManager::GetInstance().CreateScene("Galaga");
	glm::ivec2 size = dae::Renderer::GetInstance().GetWindowSize();




	//SDL_Color test = SDL_Color{100,100,0,255};
	//auto cell = new CellComponent(glm::vec3(300, 200, 0), glm::ivec2(0, 0), test, 30);	
	//obj->AddComponent(cell);
	//scene->Add(obj);

	//background
	auto SpaceBackGround = objectConstructors::RenderObject("Galaga/SpaceBackground.png");
	scene->Add(SpaceBackGround);
	auto background = SpaceBackGround->GetComponent<RenderComponent>();
	background->SetSize(float(size.x) / 3 * 2, float(size.y));


	//scoreBoard
	auto Stats = std::make_shared<dae::GameObject>();
	auto* rect = new RectComponent();
	
	rect->SetPosition(size.x / 2, 0);
	rect->SetSize(size.x / 2, 0);
	Stats->AddComponent(rect);
	scene->Add(Stats);
	//score
	
	auto score = objectConstructors::Score("lingua.otf", 12, "score:", 10, 70);
	scene->Add(score);


	
	//SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(),255, 0, 0, 255);


	
	//player character
	playerCharacter = objectConstructors::BasicActor(3);// qbert is 16 on 16 big on sprite sheet
	playerCharacter->GetComponent<RenderComponent>()->SetTexture("Galaga/Player1_default.png");
	playerCharacter->GetComponent<RenderComponent>()->SetSize(30, 30);
	playerCharacter->GetComponent<RenderComponent>()->SetOffset(-15, -15);
	scene->Add(playerCharacter);
	
	auto PlayerActor = playerCharacter->GetComponent<ActorComponent>();
	PlayerActor->Translate(float(size.x / 3), float(size.y) - 40 );
	PlayerActor->BindRenderComponent(playerCharacter->GetComponent<RenderComponent>());

	
	auto Healthbar = objectConstructors::LivesBar("Galaga/Player1_default.png", float(size.x/3 * 2 +20.f), float(size.y / 2 + 20.f), playerCharacter->GetComponent<HealthComponent>());
	Healthbar->GetComponent<HealthBarComponent>()->SetSize({ 20,20 });
	Healthbar->GetComponent<HealthBarComponent>()->SetPosition(float(size.x / 3 * 2), float(size.y) + 40);
	scene->Add(Healthbar);

	LoadInputs();

	//TestEnemy
	auto TestEnemy = objectConstructors::BeeEnemy("Galaga/bee.png", glm::vec2(size.x / 3.f, 20.f));
	scene->Add(TestEnemy);
	
}


void GalagaEngine::LoadInputs()
{
	auto PlayerActor = playerCharacter->GetComponent<ActorComponent>();
	// input Action
	auto& inputManager = input::InputManager::GetInstance();

	Action ShootAction = Action();

	ShootAction.pCommand = new FireCommand(PlayerActor);
	ShootAction.type = InputType::IsPressed;
	ShootAction.key = SDL_SCANCODE_Z;
	inputManager.AddAction(ShootAction);

	Action MoveRight = Action();
	MoveRight.pCommand = new MoveUnitCommand(PlayerActor,10.f,0.f );
	MoveRight.type = InputType::IsPressed;
	MoveRight.key = SDL_SCANCODE_RIGHT;
	inputManager.AddAction(MoveRight);

	Action MoveLeft = Action();
	MoveLeft.pCommand = new MoveUnitCommand(PlayerActor, -10.f, 0.f);
	MoveLeft.type = InputType::IsPressed;
	MoveLeft.key = SDL_SCANCODE_LEFT;
	inputManager.AddAction(MoveLeft);
}


void GalagaEngine::Update()
{	
	Minigin::Update();

	
}
