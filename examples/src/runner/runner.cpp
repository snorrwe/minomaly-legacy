#include "runner.h"

using namespace Mino;

void Program::start()
{
    loadScene("assets/runner/scene_0.json");

    input = getEngineCore()->getInput();
    time = Services::get<ITimeService>().get();

    initPlatforms();

    auto egg = createGameObject<SpriteRendererComponent,
                                SpriteAnimatorComponent,
                                AudioPlayerComponent,
                                BoxColliderComponent,
                                Rigidbody,
                                EggComponent>({0, 0});

    auto cameraTransform = getMainCamera()->getTransform();
    cameraTransform->setPosition(cameraTransform->getPosition()
                                 + Vector2<float>{SCREEN_WIDTH * -0.5f, SCREEN_HEIGHT * -0.5f});

    auto eggPic = getEngineCore()
                      ->getAssets()
                      ->loadSpriteSheet("assets/runner/egg.png", {{0, 0, 30, 30}})
                      ->at(0);
    assets.push_back(eggPic);
    auto eggEgg = egg->getComponent<EggComponent>();
    eggEgg->input = input;
    eggEgg->bottom = 0;
    egg->getTransform()->setPosition({50.f, 0.f});

    auto cameraProxy = createGameObject<CameraControllerComponent>({0.f, 0.f});
    auto cameraController = cameraProxy->getComponent<CameraControllerComponent>();
    cameraController->setPlayer(eggEgg);
    cameraController->setCamera(getMainCamera()->getTransform());

    auto music = getEngineCore()->getAssets()->loadMusic("assets/runner/Aoa02.wav");
    assets.push_back(music);
    getEngineCore()->getAudio()->playMusic(*music);
}

const auto barWidth = 30;
const auto barHeight = 120;

void Program::createPlatform(Vector2<float> const& position, Vector2<float> const& scale)
{
    auto bar = createGameObject<SpriteRendererComponent, BoxColliderComponent>(position);
    bar->getTransform()->setScale(scale);
    auto barImage = getEngineCore()->getAssets()->loadTexture("assets/runner/bar.png");
    bar->getComponent<SpriteRendererComponent>()->setTexture(barImage.get());
    auto barCollider = bar->getComponent<BoxColliderComponent>();
    barCollider->set(barWidth, barHeight);
    barCollider->setLayers(0x1 | 0x2);
}

void Program::initPlatforms()
{
    auto barImage = getEngineCore()->getAssets()->loadTexture("assets/runner/bar.png");
    assets.push_back(barImage);
    auto startx = (SCREEN_WIDTH - 50) * 0.5f;
    createPlatform({startx, 0.0}, {2.0f, 0.5f});
    createPlatform({startx + 2.f * barWidth, 0.5f * barHeight}, {2.0f, 0.5f});
    createPlatform({startx + 4.f * barWidth, 1.f * barHeight}, {5.0f, 0.25f});
    createPlatform({startx + 320, 0.0f}, {2.0f, 1.0f});
    createPlatform({startx + 320 - 2.f * barWidth, 0.25f * barHeight}, {2.0f, 1.0f});
    createPlatform({startx + 320 + 2.f * barWidth, 0.25f * barHeight}, {3.0f, 0.75f});
    createPlatform({startx + 320 + 4.f * barWidth, 0.0f}, {2.0f, 0.325f});
}

