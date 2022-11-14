#include "Renderer.h"

Renderer::Renderer(Window & parent) : OGLRenderer(parent) {
	cube = Mesh::LoadFromMeshFile("OffsetCubeY.msh");

	if (!cube)
	{
		std::cout << "mesh_Cube load failed" << std::endl;
		return;
	}
	camera = new Camera();
	
	shader = new Shader("Tutoria6Vertex.glsl", "Tutorial6Fragment.glsl");
	
	if (!shader -> LoadSuccess()) {
		std::cout << "Shader load failed" << std::endl;
		return;
	}
	 projMatrix = Matrix4::Perspective(1.0f, 10000.0f,(float)width / (float)height, 45.0f);
	 camera -> SetPosition(Vector3(0, 30, 175));
	
	root = new SceneNode();
	root -> AddChild(new CubeRobot(cube));
	
	glEnable(GL_DEPTH_TEST);
	init = true;
	
}
Renderer ::~Renderer(void) {
	delete root;
	delete shader;
	delete camera;
	delete cube;
	camera -> UpdateCamera(dt);
	viewMatrix = camera -> BuildViewMatrix();
	root -> Update(dt);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	BindShader(shader);
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(shader -> GetProgram(),
	"diffuseTex"), 1);
	DrawNode(root);
	if (n -> GetMesh()) {
	Matrix4 model = n -> GetWorldTransform() * Matrix4::Scale(n -> GetScale());
	glUniformMatrix4fv(glGetUniformLocation(shader -> GetProgram(),"modelMatrix"), 1, false, model.values);
	glUniform4fv(glGetUniformLocation(shader -> GetProgram(),"nodeColour"), 1, (float*)&n -> GetColour());
	glUniform1i(glGetUniformLocation(shader -> GetProgram(), "useTexture"), 0); // Next tutorial ;)
	n -> Draw(*this);
	}
	 for (vector < SceneNode* >::const_iterator i = n -> GetChildIteratorStart();i != n -> GetChildIteratorEnd(); ++i) {
	 DrawNode(*i);
	}	