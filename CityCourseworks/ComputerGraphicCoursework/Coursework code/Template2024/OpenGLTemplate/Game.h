#pragma once

#include "Common.h"
#include "GameWindow.h"
#include "MatrixStack.h"

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COpenAssetImportMesh;
class CAudio;
class CCube;
class CTetrahedron;
class CCatmullRom;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox* m_pSkybox;
	CCamera* m_pCamera;
	vector <CShaderProgram*>* m_pShaderPrograms;
	CPlane* m_pPlanarTerrain;
	CFreeTypeFont* m_pFtFont;
	COpenAssetImportMesh* m_pBarrelMesh;
	COpenAssetImportMesh* m_pHorseMesh;
	COpenAssetImportMesh* m_pUziMesh;
	COpenAssetImportMesh* m_pSmashMesh;
	COpenAssetImportMesh* m_pLazerMesh;
	COpenAssetImportMesh* m_pPistolMesh;
	CSphere* m_pSphere;
	CHighResolutionTimer* m_pHighResolutionTimer;
	CAudio* m_pAudio;
	CCube* m_pCube;
	CTetrahedron* m_pTetrahedron;
	CCatmullRom* m_pCatmullRom;

	// Store the position of the UZI model
	glm::vec3 m_uziPosition;

	// store the position of the top person camera
	glm::vec3 m_TopViewCameraPosition;
	//glm::vec3 m_thirdPersonCameraViewPoint;

	// store the position fo the side view camera
	glm::vec3 m_SideViewCameraPosition;

	// sotre the orientation of the uzi model
	glm::mat4 m_uziOrientation;

	// Function to update the UZI model's position
	void UpdateUziPosition(float elapsedTime);

	//Function to update the Top view camera position
	void UpdateTopViewCameraPosition(float elapsedTime);

	// Fucntion to update the side view camera position
	void UpdateSideViewCameraPosition(float elapsedTime);

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;

	//bright or dark
	bool m_isBright;

	// goes to back view
	bool isTopView;
	// goes to side view
	bool isSideView;

public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	static const int FPS = 60;
	void DisplayFrameRate();
	//object blinking
	void RedLightingObjects(glutil::MatrixStack& modelViewMatrixStack);
	void BlueLightingObjects(glutil::MatrixStack& modelViewMatrixStack);
	void GreenLightingObjects(glutil::MatrixStack& modelViewMatrixStack);
	void ToonShaderedObjects(glutil::MatrixStack& modelViewMatrixStack);
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	double m_TimePassed;


};
