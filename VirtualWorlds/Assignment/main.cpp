#include <Instance.h>
#include <Scene.h>
#include <ShaderManager.h>
#include <TextureManager.h>
#include <Camera.h>
#include <LightManager.h>
#include <Skybox.h>

#include "Terrain.h"
#include "TerrainBehaviour.h"

#include "WaterPlane.h"
#include "WaterPlaneBehaviour.h"

#include "ProceduralTexture.h"



Instance* instance;
Scene* scene;

Camera* mainCamera;


int main(int argc, char** argv) {

	scene = new Scene();

	instance = new Instance();
	instance->init(argc, argv, 1200, 900, "window", scene);

	Clock::start();
	
	GLuint basic_shader = setupShaders("shader.vert", "shader.frag");
	GLuint terrain_basic_shader = setupShaders("terrain_basic.vert", "terrain_basic.frag");
	GLuint water_basic_shader = setupShaders("water_basic.vert", "water_basic.frag");
	GLuint terrain_wireframe_shader = setupShaders("terrain_wireframe.vert", "terrain_wireframe.geom", "terrain_wireframe.frag");
	GLuint skybox_shader = setupShaders("skyboxShader.vert", "skyboxShader.frag");
	//Compute shader for generating procedural textures
	GLuint perlin_noise_compute_shader = setupShaders("computePerlin.cmps");
	//Tesselation shader for terrain
	GLuint terrain_tess_basic_shader = setupShaders("terrain_tess.vert", "terrain_tess.tcs", "terrain_tess.tes", "terrain_tess.frag");
	//Basic shader for instanced objects
	GLuint instanced_basic_shader = setupShaders("instanced_basic.vert", "instanced_basic.frag");

	//Add all shaders to the manager for easy access
	ShaderManager::addShader("basic", basic_shader);
	ShaderManager::addShader("terrain_basic", terrain_basic_shader);
	ShaderManager::addShader("water_basic", water_basic_shader);
	ShaderManager::addShader("terrain_wireframe", terrain_wireframe_shader);
	ShaderManager::addShader("skyboxShader", skybox_shader);
	ShaderManager::addShader("perlinNoiseComputeShader", perlin_noise_compute_shader);
	ShaderManager::addShader("terrain_tess_basic", terrain_tess_basic_shader);
	ShaderManager::addShader("instanced_basic", instanced_basic_shader);

	ShaderManager::initTextureLocations();

	//Load all textures
	TextureManager::loadTexture("Assets/grass_terrain.jpg");
	TextureManager::loadTexture("Assets/snow_terrain.jpg");
	TextureManager::loadTexture("Assets/gravel_terrain.jpg");
	TextureManager::loadTexture("Assets/water.png");

	//Generate Procedural Textures
	ProceduralTexture* perlinTexture = new ProceduralTexture();
	perlinTexture->setComputeShader(perlin_noise_compute_shader);
	GLuint procTexture = perlinTexture->generateTexture(512, 512);
	TextureManager::addTexture("perlin_noise", procTexture);


	//Pull in any models that might be used
	VAOLoader::LoadOBJ("box.obj");
	VAOLoader::LoadOBJ("rock.obj");

	//Initialises a completely empty scene and 1st person camera
	scene->initScene();

	mainCamera = scene->getMainCamera();
	mainCamera->setWorldPos(glm::vec3(50, 30, 50));

	Skybox* newSkybox = new Skybox("Skybox", skybox_shader);
	scene->setSkybox(newSkybox);

	//Create an ambient light for the scene
	DirectionalLight* mainLight = new DirectionalLight();
	mainLight->direction = glm::vec4(0.6f, 1.0f, 0.3f, 0.0f);
	mainLight->ambient = 0.02f;
	mainLight->colourIntensities = glm::vec4(0.7f, 0.7f, 0.7f, 0.0f);
	LightManager::addLight(mainLight);

	//Create a new terrain
	Terrain* terrain = new Terrain();

	TerrainBehaviour terrainBehaviour;
	terrainBehaviour.setActiveTerrain(terrain);
	scene->addBehaviour(&terrainBehaviour);

	//Create level plane of water.
	WaterPlane* water = new WaterPlane();
	water->m_Shader = water_basic_shader;

	WaterPlaneBehaviour waterBehaviour;
	waterBehaviour.setActiveWaterPlane(water);
	scene->addBehaviour(&waterBehaviour);

	glutMainLoop();

	ShaderManager::cleanup();
	TextureManager::cleanup();
	delete water;
	delete terrain;
	delete instance;
	delete scene;

	return 0;
}