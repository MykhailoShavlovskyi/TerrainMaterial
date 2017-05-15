#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"

#include "mge/core/Camera.hpp"

#include "mge/core/GameObject.hpp"
#include "mge/materials/AbstractMaterial.hpp"

#include "mge/materials/LitMaterial.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include "mge/materials/TextureMaterial.hpp"
#include "mge/materials/TerrainMaterial.hpp"
#include "mge/materials/WobbleMaterial.hpp"

#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/behaviours/KeysBehaviour.hpp"
#include "mge/behaviours/OrbitBehaviour.hpp"
#include "mge/behaviours/LookAt.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"
#include "mge/core/Light.hpp"
#include "mge/util/LightUtil.hpp"

#include "mge/Slider.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)

GameObject* plane;

Slider * heightMapIntensitySlider;
Slider * shininessSlider;
Slider * diffuseRSlider;
Slider * diffuseGSlider;
Slider * diffuseBSlider;
Slider * specularRSlider;
Slider * specularGSlider;
Slider * specularBSlider;
Slider * rotationSlider;

Slider * lightRSlider;
Slider * lightGSlider;
Slider * lightBSlider;

Slider * attenuationC1Slider;
Slider * attenuationC2Slider;
Slider * attenuationC3Slider;

Slider * lightTypeSlider;

Slider * lighRotationXSlider;
Slider * lighRotationYSlider;

Slider * lightAngleSlider;
Slider * lightFallOffRateSlider;

Slider * waceRadiusMaxOffsetSlider;
Slider * waveRadiusMaxOffsetFrequencySlider;
Slider * waceWidthSlider;
Slider * waveMaxHeightSlider;
Slider * waveMaxHeightOffsetSlider;
Slider * waveHeightOffsetFrequencySlider;
Slider * waveSpeedSlider;
Slider * waveCountSlider;
Slider * debugModeSlider;

TerrainMaterial * terrainMaterial;

Light * light;

float currentTerrainRotaion = 0;

float lightRotationX = 0;
float lightRotationY = 0;

MGEDemo::MGEDemo():AbstractGame (),_hud(0)
{
}

void MGEDemo::initialize() 
{
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part
	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

//build the game _world
void MGEDemo::_initializeScene()
{
	//MATERIAL SLIDERS
	heightMapIntensitySlider = new Slider(glm::vec2(5, 80), "", "height map intensity", sf::Color::White, _window);
	shininessSlider = new Slider(glm::vec2(5, 110), "", "shininess", sf::Color::White, _window);
	diffuseRSlider = new Slider(glm::vec2(5, 150), "R", "diffuse", sf::Color::Red, _window);
	diffuseGSlider = new Slider(glm::vec2(5, 180), "G", "diffuse", sf::Color::Green, _window);
	diffuseBSlider = new Slider(glm::vec2(5, 210), "B", "diffuse", sf::Color::Blue, _window);
	specularRSlider = new Slider(glm::vec2(5, 250), "R", "specular", sf::Color::Red, _window);
	specularGSlider = new Slider(glm::vec2(5, 280), "G", "specular", sf::Color::Green, _window);
	specularBSlider = new Slider(glm::vec2(5, 310), "B", "specular", sf::Color::Blue, _window);
	rotationSlider = new Slider(glm::vec2(5, 350), "", "rotation", sf::Color::White, _window);
	heightMapIntensitySlider->setValue(0.15);
	shininessSlider->setValue(0.45);
	diffuseRSlider->setValue(1);
	diffuseGSlider->setValue(1);
	diffuseBSlider->setValue(1);
	specularRSlider->setValue(1);
	specularGSlider->setValue(1);
	specularBSlider->setValue(1);
	_world->add(heightMapIntensitySlider);
	_world->add(shininessSlider);
	_world->add(diffuseRSlider);
	_world->add(diffuseGSlider);
	_world->add(diffuseBSlider);
	_world->add(specularRSlider);	
	_world->add(specularGSlider);
	_world->add(specularBSlider);
	_world->add(rotationSlider);

	//LIGHT SLIDERS
	lightRSlider = new Slider(glm::vec2(880, 20), "R", "light color", sf::Color::Red, _window);
	lightGSlider = new Slider(glm::vec2(880, 50), "G", "light color", sf::Color::Green, _window);
	lightBSlider = new Slider(glm::vec2(880, 80), "B", "light color", sf::Color::Blue, _window);
	attenuationC1Slider = new Slider(glm::vec2(880, 120), "", "light intensity", sf::Color::White, _window);
	attenuationC2Slider = new Slider(glm::vec2(880, 150), "", "linear attenuation", sf::Color::White, _window);
	attenuationC3Slider = new Slider(glm::vec2(880, 180), "", "quad attenuation", sf::Color::White, _window);
	lightTypeSlider = new Slider(glm::vec2(880, 220), "", "light type", sf::Color::White, _window);
	lighRotationXSlider = new Slider(glm::vec2(880, 260), "", "light rotX", sf::Color::White, _window);
	lighRotationYSlider = new Slider(glm::vec2(880, 290), "", "light rotY", sf::Color::White, _window);
	lightAngleSlider = new Slider(glm::vec2(880, 350), "", "angle", sf::Color::White, _window);
	lightFallOffRateSlider = new Slider(glm::vec2(880, 380), "", "fall off rate", sf::Color::White, _window);
	lightRSlider->setValue(1);
	lightGSlider->setValue(1);
	lightBSlider->setValue(1);
	lightAngleSlider->setValue(1./180 * 50);
	lightFallOffRateSlider->setValue(1. / 180 * 25);
	_world->add(lightRSlider);
	_world->add(lightGSlider);
	_world->add(lightBSlider);
	_world->add(attenuationC1Slider);
	_world->add(attenuationC2Slider);
	_world->add(attenuationC3Slider);
	_world->add(lightTypeSlider);
	_world->add(lighRotationXSlider);
	_world->add(lighRotationYSlider);
	_world->add(lightAngleSlider);
	_world->add(lightFallOffRateSlider);	

	//WAVE SLIDERS
	waceRadiusMaxOffsetSlider = new Slider(glm::vec2(5, 650), "", "wave radius max offset", sf::Color::White, _window);
	waveRadiusMaxOffsetFrequencySlider = new Slider(glm::vec2(5, 680), "", "wave radius offset frequency", sf::Color::White, _window);
	waceWidthSlider = new Slider(glm::vec2(5, 710), "", "wave width", sf::Color::White, _window);
	waveMaxHeightSlider = new Slider(glm::vec2(5, 740), "", "wave height", sf::Color::White, _window);
	waveMaxHeightOffsetSlider = new Slider(glm::vec2(5, 770), "", "wave height offset", sf::Color::White, _window);
	waveHeightOffsetFrequencySlider = new Slider(glm::vec2(5, 800), "", "wave height offset frequency", sf::Color::White, _window);
	waveSpeedSlider = new Slider(glm::vec2(5, 830), "", "wave speed", sf::Color::White, _window);
	waveCountSlider = new Slider(glm::vec2(5, 860), "", "wave count", sf::Color::White, _window);
	debugModeSlider = new Slider(glm::vec2(820, 860), "", "wave debug mode", sf::Color::White, _window);
	waceRadiusMaxOffsetSlider->setValue(0.36);
	waveRadiusMaxOffsetFrequencySlider->setValue(0.233);
	waceWidthSlider->setValue(0);
	waveMaxHeightSlider->setValue(0.053);
	waveMaxHeightOffsetSlider->setValue(0.2);
	waveHeightOffsetFrequencySlider->setValue(0.873);
	waveSpeedSlider->setValue(0.1);
	waveCountSlider->setValue(0.7);
	_world->add(waceRadiusMaxOffsetSlider);
	_world->add(waveRadiusMaxOffsetFrequencySlider);
	_world->add(waceWidthSlider);
	_world->add(waveMaxHeightSlider);
	_world->add(waveMaxHeightOffsetSlider);
	_world->add(waveHeightOffsetFrequencySlider);
	_world->add(waveSpeedSlider);
	_world->add(waveCountSlider);
	_world->add(debugModeSlider);

	//CAMERA
    Camera* camera = new Camera ("camera", glm::vec3(0,5,9));
    camera->rotate(glm::radians(-40.0f), glm::vec3(1,0,0));
    _world->add(camera);
    _world->setMainCamera(camera);

	//MAETERIALS
	std::vector<Texture*> diffuseTextureBlock;
	diffuseTextureBlock.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse1.jpg"));
	diffuseTextureBlock.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/water.jpg"));
	diffuseTextureBlock.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse3.jpg"));
	diffuseTextureBlock.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse4.jpg"));

	std::vector<Texture*> diffuseTextureBlock2;
	diffuseTextureBlock2.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse2.jpg"));
	diffuseTextureBlock2.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse1.jpg"));
	diffuseTextureBlock2.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse3.jpg"));
	diffuseTextureBlock2.push_back(Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse4.jpg"));

	light = new Light("light", glm::vec3(0, 4, 0), glm::vec3(1, 1, 1), Light::Type::POINT, glm::vec3(1, 0, 0));
	_world->add(light);
	LightUtil::AddLight(light);

	terrainMaterial = new TerrainMaterial(Texture::load(config::MGE_TEXTURE_PATH + "terrain/heightmap.png"), Texture::load(config::MGE_TEXTURE_PATH + "terrain/splatmap.png"), diffuseTextureBlock, _world->getMainCamera());
	//terrainMaterial = new TerrainMaterial(Texture::load(config::MGE_TEXTURE_PATH + "terrain/unityHeightmap.png"), Texture::load(config::MGE_TEXTURE_PATH + "terrain/unitySplatMap.png"), diffuseTextureBlock, _world->getMainCamera());

	//PLANE
    plane = new GameObject ("plane", glm::vec3(0,0,0));
    plane->scale(glm::vec3(5,5,5));
    plane->setMesh(Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj"));
    plane->setMaterial(terrainMaterial);
    _world->add(plane);

    camera->setBehaviour(new OrbitBehaviour(15, 45, 0.5, plane, _window));
}

void MGEDemo::_render() 
{
    AbstractGame::_render();
    _updateHud();
}

void MGEDemo::_updateHud() 
{
	//take values from sliders
	terrainMaterial->setIntensity(heightMapIntensitySlider->getValue()*4); //value from 0 to 4;
	terrainMaterial->setShininess(int((shininessSlider->getValue() * 16) * (shininessSlider->getValue() * 16))); //value from 0 to 256;
	terrainMaterial->setDiffuseColor(glm::vec3(diffuseRSlider->getValue(), diffuseGSlider->getValue(), diffuseBSlider->getValue()));
	terrainMaterial->setSpecularColor(glm::vec3(specularRSlider->getValue(), specularGSlider->getValue(), specularBSlider->getValue()));

	plane->rotate(glm::radians(rotationSlider->getValue() * 360 - currentTerrainRotaion), glm::vec3(0, 1, 0));
	currentTerrainRotaion = rotationSlider->getValue() * 360;

	float waceRadiusMaxOffset = waceRadiusMaxOffsetSlider->getValue()* (1. / 20) ; // vallue from 0 to 1./20
	float waveRadiusMaxOffsetFrequency = waveRadiusMaxOffsetFrequencySlider->getValue() * 10 + 10; //value from 10 to 20 
	float waveWidth = waceWidthSlider->getValue() * 0.09 + 0.06;//value from 0.06 to 0.15
	float waveMaxHeight = waveMaxHeightSlider->getValue() *(1./05-1./40)+ 1. / 40; //valude from 1./40 to 1./05
	float waveMaxHeightOffset = waveMaxHeightOffsetSlider->getValue()*(1. / 30 ) ; //valude from 0 to 1./30
	float waveHeightOffsetFrequency = waveHeightOffsetFrequencySlider->getValue() * 10 +5; //value from 5 to 15
	float waveSpeed = waveSpeedSlider->getValue()* 0.003;//value from 0 to 0.003
	float waveCount = int(waveCountSlider->getValue() * 10);//value from 0 to 0.003
	terrainMaterial->setWaveParameters(std::vector<float>{ waceRadiusMaxOffset ,waveRadiusMaxOffsetFrequency ,waveWidth,waveMaxHeight,waveMaxHeightOffset,waveHeightOffsetFrequency, waveSpeed, waveCount });
	if (debugModeSlider->getValue() < 0.5) { terrainMaterial->debugMode = false; } else { terrainMaterial->debugMode = true; } //set debug mode

	light->setColor(glm::vec3(lightRSlider->getValue(), lightGSlider->getValue(), lightBSlider->getValue()));
	light->setAttenuation(glm::vec3(attenuationC1Slider->getValue() *9 + 1, attenuationC2Slider->getValue(), attenuationC3Slider->getValue()));
	if (lightTypeSlider->getValue() <= 0.33f)
	{
		light->changeType(Light::Type::POINT);
	}
	else if (lightTypeSlider->getValue() <= 0.66f)
	{
		light->changeType(Light::Type::DIRRECTIONAL);
	}
	else
	{
		light->changeType(Light::Type::SPOT);
	}

	light->rotate(glm::radians(lighRotationXSlider->getValue() * 360 - lightRotationX), glm::vec3(1, 0, 0));
	light->rotate(glm::radians(lighRotationYSlider->getValue() * 360 - lightRotationY), glm::vec3(0, 1, 0));
	lightRotationX = lighRotationXSlider->getValue() * 360;
	lightRotationY = lighRotationYSlider->getValue() * 360;

	light->setAngle(lightAngleSlider->getValue() * 180 + 0.001);
	light->setFallOffRate(lightFallOffRateSlider->getValue() * 180 + 0.001);

	//update hud
    string debugInfo = "";
    debugInfo += string ("FPS:") + std::to_string((int)_fps)+"\n";

    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

MGEDemo::~MGEDemo()
{
	//dtor
}
