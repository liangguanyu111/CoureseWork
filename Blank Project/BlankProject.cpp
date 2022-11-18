#include "../NCLGL/window.h"
#include "Renderer.h"
#include "SecondRenderer.h"

void SwitchScene(int& index);
int main()	{
	Window w("Make your own project!", 1280, 720, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	SecondRenderer secondRenderer(w);

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	
	int index = 2;


	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) 
	{
		switch (index)
		{
		case 1:
			if (renderer.HasInitialised())
			{
				renderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
				renderer.RenderScene();
				renderer.SwapBuffers();
			}
			else
			{
				renderer.Init();
			}
			break;
		case 2:
			if (secondRenderer.HasInitialised())
			{
				secondRenderer.UpdateScene(w.GetTimer()->GetTimeDeltaSeconds());
				secondRenderer.RenderScene();
				secondRenderer.SwapBuffers();
			}
			else
			{
				secondRenderer.Init();
			}
			break;
			break;
		}



		if (Window::GetKeyboard()->KeyDown(KEYBOARD_F5)) {
			Shader::ReloadAllShaders();
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			SwitchScene(index);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_2))
		{
			secondRenderer.CameraAutoMove();
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_3))
		{
			secondRenderer.CameraAutoRotate();
		}
	}

	return 0;
}


void SwitchScene(int& index)
{
	switch ((index))
	{
	case 1:
		index = 2;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glDisable(GL_CULL_FACE);
		break;
	case 2:
		index = 1;
		glEnable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		break;
	}

}