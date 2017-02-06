#include "stdafx.h"
#include "Editor.hpp"
#include "Gui.hpp"

shared_ptr<Scene> scenes[8];

int selectedScene = 0;
int selectedSprite;
bool editing;
Camera mainCamera;

SceneSelector sceneSelector;
PaletteSelector paletteSelector;

bool Editor::mouseAvailable;
XMVECTOR Editor::mouseVector;

void Editor::init()
{
	mouseAvailable = true;
	// Set camera to default position
	mainCamera.SetPosition(0, 0, -2);
	for (int i = 0; i < 8; i++)
	{
		scenes[i] = make_shared<Scene>();
	}
	scenes[0]->setBackgroundSprite(0, 0, { 1,0,false,false });
	scenes[0]->setBackgroundSprite(1, 0, { 2,0,false,false });
	scenes[0]->setBackgroundSprite(15, 15, { 3,1,false,false });
	scenes[0]->setBackgroundSprite(3, 0, { 4,0,false,false });
	scenes[1]->setBackgroundSprite(0, 0, { 1,0,false,false });
	scenes[1]->setBackgroundSprite(1, 0, { 2,0,false,false });
	scenes[1]->setBackgroundSprite(2, 0, { 3,0,false,false });
	scenes[1]->setBackgroundSprite(3, 0, { 4,0,false,false });
}

void Editor::update()
{
	mouseAvailable = sceneSelector.update(mouseAvailable);
	mouseAvailable = paletteSelector.update(mouseAvailable, scenes[selectedScene]);
	// Update camera position
	if (InputState::keyboardMouse->mouseButtons[right_mouse].state > 0)
	{
		float xRot = InputState::keyboardMouse->mouseDeltaX / 3;
		float yRot = InputState::keyboardMouse->mouseDeltaY / 3;
		mainCamera.AdjustRotation(xRot, 0.0f, yRot);
	}
	// Calculate mouse vector
	
}

void Editor::render()
{
	// Update view with whatever camera
	mainCamera.Render();
	N3s3d::updateMatricesWithCamera(&mainCamera);
	// Enable depth buffer
	N3s3d::setDepthBufferState(true);
	// Render the scene
	scenes[selectedScene]->render();
	// Render GUI
	N3s3d::setDepthBufferState(false);
	N3s3d::setGuiProjection();
	sceneSelector.render();
	paletteSelector.render(scenes[selectedScene]);
}

void Editor::setScene(int s)
{
	selectedScene = s;
}

Hue Editor::getBackgroundColor()
{
	return scenes[selectedScene]->getSelectedPalette()->getBackgroundColor();
}
