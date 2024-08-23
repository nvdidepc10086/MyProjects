/*
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting,
 different camera controls, different shaders, etc.

 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk)

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

 version 6.1a 13/02/2022 - Sorted out Release mode and a few small compiler warnings
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

*/


#include "game.h"


// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "FreeTypeFont.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Audio.h"
#include "Cube.h"
#include "Tetrahedron.h"
#include "CatmullRom.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pPlanarTerrain = NULL;
	m_pFtFont = NULL;
	//------------mesh object------------//
	//m_pBarrelMesh = NULL;
	//m_pHorseMesh = NULL;
	m_pUziMesh = NULL;
	m_pSmashMesh = NULL;
	m_pPistolMesh = NULL;
	m_pLazerMesh = NULL;
	//------------mesh object------------//
	m_pSphere = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCube = NULL;
	m_pTetrahedron = NULL;
	m_pCatmullRom = NULL;
	m_isBright = true;
	isTopView = false;
	isSideView = false;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;

}

// Destructor
Game::~Game()
{
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pPlanarTerrain;
	delete m_pFtFont;
	//------------mesh object------------//
	//delete m_pBarrelMesh;
	//delete m_pHorseMesh;
	delete m_pUziMesh;
	delete m_pSmashMesh;
	delete m_pPistolMesh;
	delete m_pLazerMesh;
	//------------mesh object------------//
	delete m_pSphere;
	delete m_pAudio;
	delete m_pCube;
	delete m_pTetrahedron;
	delete m_pCatmullRom;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
	delete m_pHighResolutionTimer;
}

// Initialisation:  This method only runs once at startup
void Game::Initialise()
{
	// Set the clear colour and depth
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram*>;
	m_pPlanarTerrain = new CPlane;
	m_pFtFont = new CFreeTypeFont;
	//------------mesh object------------//
	//m_pBarrelMesh = new COpenAssetImportMesh;
	//m_pHorseMesh = new COpenAssetImportMesh;
	m_pUziMesh = new COpenAssetImportMesh;
	m_pSmashMesh = new COpenAssetImportMesh;
	m_pPistolMesh = new COpenAssetImportMesh;
	m_pLazerMesh = new COpenAssetImportMesh;

	//------------mesh object------------//
	m_pSphere = new CSphere;
	m_pAudio = new CAudio;
	m_pCube = new CCube;
	m_pTetrahedron = new CTetrahedron;
	m_pCatmullRom = new CCatmullRom;

	RECT dimensions = m_gameWindow.GetDimensions();

	int width = dimensions.right - dimensions.left;
	int height = dimensions.bottom - dimensions.top;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height);
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float)width / (float)height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("RedLightingShader.vert");
	sShaderFileNames.push_back("RedLightingShader.frag");
	sShaderFileNames.push_back("BlueLightingShader.vert");
	sShaderFileNames.push_back("BlueLightingShader.frag");
	sShaderFileNames.push_back("GreenLightingShader.vert");
	sShaderFileNames.push_back("GreenLightingShader.frag");
	sShaderFileNames.push_back("ToonShader.vert");
	sShaderFileNames.push_back("ToonShader.frag");

	for (int i = 0; i < (int)sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int)sShaderFileNames[i].size() - 4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader("resources\\shaders\\" + sShaderFileNames[i], iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram* pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);

	// Create a shader program for fonts
	CShaderProgram* pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);

	// Create a shader program for red lighting
	CShaderProgram* pRedLightingProgram = new CShaderProgram;
	pRedLightingProgram->CreateProgram();
	pRedLightingProgram->AddShaderToProgram(&shShaders[4]);
	pRedLightingProgram->AddShaderToProgram(&shShaders[5]);
	pRedLightingProgram->LinkProgram();
	m_pShaderPrograms->push_back(pRedLightingProgram);

	// Create a shader program for Blue Lighting
	CShaderProgram* pBlueLightingProgram = new CShaderProgram;
	pBlueLightingProgram->CreateProgram();
	pBlueLightingProgram->AddShaderToProgram(&shShaders[6]);
	pBlueLightingProgram->AddShaderToProgram(&shShaders[7]);
	pBlueLightingProgram->LinkProgram();
	m_pShaderPrograms->push_back(pBlueLightingProgram);

	// Create a shader program for Green Lighting
	CShaderProgram* pGreenLightingProgram = new CShaderProgram;
	pGreenLightingProgram->CreateProgram();
	pGreenLightingProgram->AddShaderToProgram(&shShaders[8]);
	pGreenLightingProgram->AddShaderToProgram(&shShaders[9]);
	pGreenLightingProgram->LinkProgram();
	m_pShaderPrograms->push_back(pGreenLightingProgram);

	// Create a shader program for Green Toon shader
	CShaderProgram* pToonProgram = new CShaderProgram;
	pToonProgram->CreateProgram();
	pToonProgram->AddShaderToProgram(&shShaders[10]);
	pToonProgram->AddShaderToProgram(&shShaders[11]);
	pToonProgram->LinkProgram();
	m_pShaderPrograms->push_back(pToonProgram);

	// You can follow this pattern to load additional shaders

	// Create the skybox
	// Skybox downloaded from http://www.akimbo.in/forum/viewtopic.php?f=10&t=9
	m_pSkybox->Create(2500.0f);

	// Create the planar terrain
	m_pPlanarTerrain->Create("resources\\textures\\", "grassfloor01.jpg", 2000.0f, 2000.0f, 50.0f); // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013

	m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);

	// Load some meshes in OBJ format
	//m_pBarrelMesh->Load("resources\\models\\Barrel\\Barrel02.obj");  // Downloaded from http://www.psionicgames.com/?page_id=24 on 24 Jan 2013
	//m_pHorseMesh->Load("resources\\models\\Horse\\Horse2.obj");  // Downloaded from http://opengameart.org/content/horse-lowpoly on 24 Jan 2013
	m_pUziMesh->Load("resources\\models\\UZI\\UZI.obj"); //downloaded from https://www.dropbox.com/sh/xd8i6p8eltuqrlk/AAA1DLBW-YaQlkYng7IKv2xSa?dl=0&preview=4guns.zip on 1 Apr 2024
	m_pSmashMesh->Load("resources\\models\\Smash\\Smash.obj"); //downloaded from https://www.dropbox.com/sh/xd8i6p8eltuqrlk/AAA1DLBW-YaQlkYng7IKv2xSa?dl=0&preview=4guns.zip on 1 Apr 2024
	m_pLazerMesh->Load("resources\\models\\Lazer\\Lazer.obj"); //downloaded from https://www.dropbox.com/sh/xd8i6p8eltuqrlk/AAA1DLBW-YaQlkYng7IKv2xSa?dl=0&preview=4guns.zip on 1 Apr 2024
	m_pPistolMesh->Load("resources\\models\\Pistol\\Pistol.obj"); //downloaded from https://www.dropbox.com/sh/xd8i6p8eltuqrlk/AAA1DLBW-YaQlkYng7IKv2xSa?dl=0&preview=4guns.zip on 1 Apr 2024

	// Create a sphere
	m_pSphere->Create("resources\\textures\\", "dirtpile01.jpg", 25, 25);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);

	//Create a cube
	m_pCube->Create("resources\\textures\\golden-exclamation-mark.png"); //texture downloaded form https://www.google.com/imgres?q=%E9%BB%84%E9%87%91%E6%84%9F%E5%8F%B9%E5%8F%B7&imgurl=https%3A%2F%2Fthumbs.dreamstime.com%2Fb%2F%25E5%2583%258F%25E7%25B4%25A0%25E8%2589%25BA%25E6%259C%25AF-%25E4%25BD%258D%25E5%259B%25BE%25E6%25A0%2587%25E9%259B%2586-%25E2%2580%2594-%25E5%25B8%25A6%25E9%2597%25AE%25E5%258F%25B7%25E3%2580%2581%25E6%2584%259F%25E5%258F%25B9%25E5%258F%25B7%25E3%2580%2581%25E5%25AD%2597%25E6%25AF%258Ds%25E5%2592%258C%25E5%258A%25A0%25E5%258F%25B7%25E7%259A%2584%25E9%25BB%2584%25E8%2589%25B2%25E9%2587%2591%25E8%2589%25B2%25E6%25A1%2586-%25E7%258B%25AC%25E7%25AB%258B-%25E5%2583%258F%25E7%25B4%25A0%25E8%2589%25BA%25E6%259C%25AF%25E5%2593%2581-173539420.jpg&imgrefurl=https%3A%2F%2Fcn.dreamstime.com%2F%25E5%2583%258F%25E7%25B4%25A0%25E8%2589%25BA%25E6%259C%25AF-%25E4%25BD%258D%25E5%259B%25BE%25E6%25A0%2587%25E9%259B%2586-%25E2%2580%2594-%25E5%25B8%25A6%25E9%2597%25AE%25E5%258F%25B7%25E3%2580%2581%25E6%2584%259F%25E5%258F%25B9%25E5%258F%25B7%25E3%2580%2581%25E5%25AD%2597%25E6%25AF%258Ds%25E5%2592%258C%25E5%258A%25A0%25E5%258F%25B7%25E7%259A%2584%25E9%25BB%2584%25E8%2589%25B2%25E9%2587%2591%25E8%2589%25B2%25E6%25A1%2586-%25E7%258B%25AC%25E7%25AB%258B-%25E5%2583%258F%25E7%25B4%25A0%25E8%2589%25BA%25E6%259C%25AF%25E5%2593%2581-image173539420&docid=TPlmRxB-arpReM&tbnid=wppOOCpzCO-vPM&vet=12ahUKEwj-18fK5KGFAxUhZEEAHZfRAb0QM3oECDQQAA..i&w=800&h=800&hcb=2&ved=2ahUKEwj-18fK5KGFAxUhZEEAHZfRAb0QM3oECDQQAA on 1 Apr 2024

	//create a tetrahedron
	m_pTetrahedron->Create("resources\\textures\\golden-exclamation-mark.png");

	// Create Path
	m_pCatmullRom->CreateCentreline();
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack("resources\\textures\\grassfloor01.jpg");
	//m_pCatmullRom->CreateTrackUsingCubes("resources\\textures\\grassfloor01.jpg");
	// Initialise audio and play background music
	m_pAudio->Initialise();
	m_pAudio->LoadEventSound("resources\\Audio\\Boing.wav");					// Royalty free sound from freesound.org
	m_pAudio->LoadMusicStream("resources\\Audio\\DST-Garote.mp3");	// Royalty free music from http://www.nosoapradio.us/
	//m_pAudio->PlayMusicStream();

	//set starting position of the model
	m_uziPosition = glm::vec3(0, 0, 10);

	//set starting position of the camera

}

// Render method runs repeatedly in a loop
void Game::Render()
{

	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	// Use the main shader program 
	CShaderProgram* pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	// Note: cubemap and non-cubemap textures should not be mixed in the same texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10;
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);


	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Call LookAt to create the view matrix and put this on the modelViewMatrix stack. 
	// Store the view matrix and the normal matrix associated with the view matrix for later (they're useful for lighting -- since lighting is done in eye coordinates)
	if (isTopView) {
		modelViewMatrixStack.LookAt(m_TopViewCameraPosition, m_uziPosition * 2.f, m_pCamera->GetUpVector());
	}
	else if (isSideView) {
		modelViewMatrixStack.LookAt(m_SideViewCameraPosition, m_uziPosition * 2.f, m_pCamera->GetUpVector());
	}
	else {
		modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());

	}
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);


	// Render Path
	m_pCatmullRom->RenderCentreline();
	m_pCatmullRom->RenderOffsetCurves();
	m_pCatmullRom->RenderTrack();
	//m_pCatmullRom->RenderTrackUsingCubes();

	/*
	// Set light and materials in main shader program
	glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
	pMainProgram->SetUniform("light1.position", viewMatrix*lightPosition1); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
	pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance
	pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
	*/
	// change light between bright and dark
	if (m_isBright) {
		// Bright environment settings
		// Set light properties for bright environment
		glm::vec4 lightPosition1 = glm::vec4(-100, 100, -100, 1); // Position of light source *in world coordinates*
		pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
		pMainProgram->SetUniform("light1.La", glm::vec3(1.0f));		// Ambient colour of light
		pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f));		// Diffuse colour of light
		pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f));		// Specular colour of light
		pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance

		// Turn on diffuse + specular materials
		pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

		pMainProgram->SetUniform("material1.shininess", 15.0f);		// Shininess material property
	}
	else {
		glm::vec4 lightPosition1 = glm::vec4(-50, 5, -5, 1); // Position of light source *in world coordinates*
		pMainProgram->SetUniform("light1.position", viewMatrix * lightPosition1); // Position of light source *in eye coordinates*
		pMainProgram->SetUniform("light1.La", glm::vec3(0.2));		// Ambient colour of light
		pMainProgram->SetUniform("light1.Ld", glm::vec3(0.2));		// Diffuse colour of light
		pMainProgram->SetUniform("light1.Ls", glm::vec3(0.2));		// Specular colour of light
		pMainProgram->SetUniform("material1.Ma", glm::vec3(1.0f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));	// Specular material reflectance

		// Turn on diffuse + specular materials
		pMainProgram->SetUniform("material1.Ma", glm::vec3(0.1f));	// Ambient material reflectance
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.1f));	// Diffuse material reflectance
		pMainProgram->SetUniform("material1.Ms", glm::vec3(0.1f));	// Specular material reflectance	

		pMainProgram->SetUniform("material1.shininess", 10.0f);		// Shininess material property
	}

	// Render the skybox and terrain with full ambient reflectance 
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("renderSkybox", true);

	// Translate the modelview matrix to the camera eye point so the skybox stays centered around the camera
	glm::vec3 vEye = m_pCamera->GetPosition();
	modelViewMatrixStack.Translate(vEye);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));

	// Set fog parameters specifically for the skybox
	pMainProgram->SetUniform("fogColor", glm::vec3(0.7, 0.7, 0.7)); // Example fog color for the skybox
	pMainProgram->SetUniform("fogDensity", 0.0001f); // Example fog density for the skybox

	m_pSkybox->Render(cubeMapTextureUnit);
	pMainProgram->SetUniform("renderSkybox", false); // Reset renderSkybox flag for other objects
	modelViewMatrixStack.Pop();

	// Render the planar terrain
	modelViewMatrixStack.Push();
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPlanarTerrain->Render();
	modelViewMatrixStack.Pop();


	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f));	// Ambient material reflectance
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f));	// Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms", glm::vec3(1.0f));	// Specular material reflectance	

	// Render the sphere
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 2.0f, 150.0f));
	modelViewMatrixStack.Scale(2.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pSphere->Render();
	modelViewMatrixStack.Pop();

	//render the tetrahedron
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(40, 3, 20));
	modelViewMatrixStack.Scale(1.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pTetrahedron->Render();
	modelViewMatrixStack.Pop();

	//Render the cube
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(40.0f, 4.0f, 0.0f));
	modelViewMatrixStack.Scale(1.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pCube->Render();
	modelViewMatrixStack.Pop();

	/*
	//Render the cube
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-20.0f, 4.0f, 0.0f));
	modelViewMatrixStack.Scale(1.0f);
	ToonShaderedObjects(modelViewMatrixStack);
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pCube->Render();
	modelViewMatrixStack.Pop();
	*/

	//Render the cube
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-40.0f, 4.0f, 0.0f));
	modelViewMatrixStack.Scale(1.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);
	m_pCube->Render();
	modelViewMatrixStack.Pop();


	//Render the UZI
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(m_uziPosition.x * 2, m_uziPosition.y * 2 + 2, m_uziPosition.z * 2));
	modelViewMatrixStack *= m_uziOrientation;
	modelViewMatrixStack.Scale(5.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pUziMesh->Render();
	modelViewMatrixStack.Pop();


	// Render the Smash
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(20.0f, 7.5f, 20.0f));
	modelViewMatrixStack.Scale(4.f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pSmashMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Pistol
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(30.0f, 4.5f, -20.0f));
	modelViewMatrixStack.Scale(5.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pPistolMesh->Render();
	modelViewMatrixStack.Pop();

	// Render the Lazer
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(-20.0f, 7.0f, -30.0f));
	modelViewMatrixStack.Scale(5.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	m_pLazerMesh->Render();
	modelViewMatrixStack.Pop();

	//Render the cube
	modelViewMatrixStack.Push();
	modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 0.0f));
	modelViewMatrixStack.Scale(2.0f);
	pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
	// To turn off texture mapping and use the sphere colour only (currently white material), uncomment the next line
	//pMainProgram->SetUniform("bUseTexture", false);

	modelViewMatrixStack.Pop();


	// Draw the 2D graphics after the 3D graphics
	DisplayFrameRate();

	if (!m_isBright) {
		// Render red lighting models
		modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(glm::vec3(25, 0.0f, -10.0f));
			modelViewMatrixStack.Scale(2.0f);
			RedLightingObjects(modelViewMatrixStack);
			m_pSphere->Render(); // Assuming m_pModel is the pointer to your model object
		modelViewMatrixStack.Pop();

		// Render blue lighting models
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(-25.0f, 0.0f, -10.0f));
		modelViewMatrixStack.Scale(2.0f);
		BlueLightingObjects(modelViewMatrixStack);
		m_pSphere->Render(); // Assuming m_pModel is the pointer to your model object
		modelViewMatrixStack.Pop();

		// Render green lighting models
		modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 0.0f, 20.0f));
		modelViewMatrixStack.Scale(2.0f);
		GreenLightingObjects(modelViewMatrixStack);
		m_pSphere->Render(); // Assuming m_pModel is the pointer to your model object
		modelViewMatrixStack.Pop();
	}




	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());

}

// Update method runs repeatedly with the Render method
void Game::Update()
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);
	//m_pCamera->Set(glm::vec3(0.f, 300, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));

	m_pAudio->Update();

	// Update the UZI model's position
	UpdateUziPosition(m_dt);

	// Update the camera position
	if (isTopView) {
		UpdateTopViewCameraPosition(m_dt);
	}
	else if (isSideView) {
		UpdateSideViewCameraPosition(m_dt);
	}

}

void Game::UpdateTopViewCameraPosition(float elapsedTime) {
	// Calculate the distance traveled based on the elapsed time
	float distancePerSecond = 0.01f; // Adjust as needed
	float distanceTraveled = distancePerSecond * elapsedTime;

	m_TopViewCameraPosition = m_pCatmullRom->MoveAlongCenterline(m_uziPosition, distanceTraveled);

	m_TopViewCameraPosition = glm::vec3(m_TopViewCameraPosition.x * 2.f, m_TopViewCameraPosition.y * 2.f + 25.5f, m_TopViewCameraPosition.z * 2.f);

	//m_TopViewCameraViewPoint = glm::vec3(m_TopViewCameraPosition.x * 5.f, m_TopViewCameraPosition.y * 5.f + 5.5f, m_TopViewCameraPosition.z * 5.f);
}

void Game::UpdateSideViewCameraPosition(float elapsedTime) {
	// Calculate the distance traveled based on the elapsed time
	float distancePerSecond = 0.01f; // Adjust as needed
	float distanceTraveled = distancePerSecond * elapsedTime;

	m_SideViewCameraPosition = m_pCatmullRom->MoveAlongCenterline(m_uziPosition, distanceTraveled);

	m_SideViewCameraPosition = glm::vec3(m_SideViewCameraPosition.x * 4.f, m_SideViewCameraPosition.y * 2.f + 5.5f, m_SideViewCameraPosition.z * 4.f);

	//m_TopViewCameraViewPoint = glm::vec3(m_TopViewCameraPosition.x * 5.f, m_TopViewCameraPosition.y * 5.f + 5.5f, m_TopViewCameraPosition.z * 5.f);
}

void Game::UpdateUziPosition(float elapsedTime) {
	// Calculate the distance traveled based on the elapsed time
	float distancePerSecond = 0.01f; // Adjust as needed
	float distanceTraveled = distancePerSecond * elapsedTime;

	// Move the UZI model along the centerline
	m_uziPosition = m_pCatmullRom->MoveAlongCenterline(m_uziPosition, distanceTraveled);

	m_uziOrientation = m_pCatmullRom->ChangeOrientationOfModel(m_uziPosition, distanceTraveled);
}

void Game::RedLightingObjects(glutil::MatrixStack& modelViewMatrixStack) {
	// Use the red lighting shader program
	CShaderProgram* pRedLightingProgram = (*m_pShaderPrograms)[2]; // Assuming red lighting shader is at index 2
	pRedLightingProgram->UseProgram();

	// Calculate the time elapsed for the blinking effect
	float t = fmodf((float)m_pHighResolutionTimer->Elapsed(), 2.0f * M_PI); // Use a period of 2*pi for one complete blink cycle

	// Set uniforms for the red lighting shader program
	pRedLightingProgram->SetUniform("modelViewMatrix", modelViewMatrixStack.Top());
	pRedLightingProgram->SetUniform("projectionMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pRedLightingProgram->SetUniform("t", t); // Update the time variable for the blinking effect

	// Render the model with the red lighting shader

	pRedLightingProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pRedLightingProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));

}

void Game::BlueLightingObjects(glutil::MatrixStack& modelViewMatrixStack) {
	// Use the red lighting shader program
	CShaderProgram* pRedLightingProgram = (*m_pShaderPrograms)[3]; // Assuming red lighting shader is at index 2
	pRedLightingProgram->UseProgram();

	// Calculate the time elapsed for the blinking effect
	float t = fmodf((float)m_pHighResolutionTimer->Elapsed(), 2.0f * M_PI); // Use a period of 2*pi for one complete blink cycle

	// Set uniforms for the red lighting shader program
	pRedLightingProgram->SetUniform("modelViewMatrix", modelViewMatrixStack.Top());
	pRedLightingProgram->SetUniform("projectionMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pRedLightingProgram->SetUniform("t", t); // Update the time variable for the blinking effect

	// Render the model with the red lighting shader

	pRedLightingProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pRedLightingProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));

}

void Game::GreenLightingObjects(glutil::MatrixStack& modelViewMatrixStack) {
	// Use the red lighting shader program
	CShaderProgram* pRedLightingProgram = (*m_pShaderPrograms)[4]; // Assuming red lighting shader is at index 2
	pRedLightingProgram->UseProgram();

	// Calculate the time elapsed for the blinking effect
	float t = fmodf((float)m_pHighResolutionTimer->Elapsed(), 2.0f * M_PI); // Use a period of 2*pi for one complete blink cycle

	// Set uniforms for the red lighting shader program
	pRedLightingProgram->SetUniform("modelViewMatrix", modelViewMatrixStack.Top());
	pRedLightingProgram->SetUniform("projectionMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
	pRedLightingProgram->SetUniform("t", t); // Update the time variable for the blinking effect

	// Render the model with the red lighting shader

	pRedLightingProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pRedLightingProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));

}


void Game::ToonShaderedObjects(glutil::MatrixStack& modelViewMatrixStack) {
	// Use the red lighting shader program
	CShaderProgram* pToonProgram = (*m_pShaderPrograms)[5]; // Assuming red lighting shader is at index 2
	pToonProgram->UseProgram();

	pToonProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
	pToonProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));


}



void Game::DisplayFrameRate()
{


	CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	m_elapsedTime += m_dt;
	m_frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
	if (m_elapsedTime > 1000)
	{
		m_elapsedTime = 0;
		m_framesPerSecond = m_frameCount;

		// Reset the frames per second
		m_frameCount = 0;
	}

	if (m_framesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
		// Calculate the time in seconds
		m_TimePassed += static_cast<float>(m_elapsedTime) / 4000000.0f ;
		

		// Render the time in seconds
		m_pFtFont->Render(20, height - 40, 20, "Time: %.2f s", m_TimePassed * 5);
		
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/


	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();



}


WPARAM Game::Execute()
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if (!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();


	MSG msg;

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (m_appActive) {
			GameLoop();
		}
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch (LOWORD(w_param))
		{
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			m_appActive = true;
			m_pHighResolutionTimer->Start();
			break;
		case WA_INACTIVE:
			m_appActive = false;
			break;
		}
		break;
	}

	case WM_SIZE:
		RECT dimensions;
		GetClientRect(window, &dimensions);
		m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch (w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case '1':
			m_pAudio->PlayEventSound();
			break;
		case VK_F1:
			m_pAudio->PlayEventSound();
			break;
		case VK_SPACE:
			if (m_isBright) {
				m_isBright = !m_isBright;
			}
			else {
				m_isBright = !m_isBright;
			}
			break;
		case VK_F2:
			if (!isTopView) {
				isTopView = true;
			}
			else {
				isTopView = false;
			}
		case VK_F3:
			if (!isSideView) {
				isSideView = true;
			}
			else {
				isSideView = false;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance()
{
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance)
{
	m_hInstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int)
{
	Game& game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return int(game.Execute());
}
