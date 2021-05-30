#include <iostream>
#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/System/Time.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



// Kody shaderów
const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
out vec3 Color;
in vec2 aTexCoord;
out vec2 TexCoord;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;
void main(){
TexCoord = aTexCoord;
//Color = color;
gl_Position = proj * view * model * vec4(position, 1.0);
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;
out vec4 outColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
//outColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
outColor = texture(texture1, TexCoord);
//outColor = vec4(Color, 1.0);
}
)glsl";

void cube(int buffer) {
	int points = 36;

	float vertices[] = {
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 3.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 3.0f, 3.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 3.0f, 3.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 3.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f, 1.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 0.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points * 8, vertices, GL_STATIC_DRAW);
}

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
int lastX;
int lastY;
double yaw = -90;
double pitch = 0;

void moveCamByKeyboard(GLint uniView, float time) {
	float speed = 0.00001f * time;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		cameraPos += speed * cameraFront;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		cameraPos -= speed * cameraFront;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}

	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

void moveCamByMouse(GLint uniView, float time, const sf::Window& window) {
	sf::Vector2i localPosition = sf::Mouse::getPosition(window);
	sf::Vector2i position;
	bool relocation = false;

	if (localPosition.x <= 0) {
		position.x = window.getSize().x;
		position.y = localPosition.y;
		relocation = true;
	}
	if (localPosition.x >= window.getSize().x - 1) {
		position.x = 0;
		position.y = localPosition.y;
		relocation = true;
	}
	if (localPosition.y <= 0) {
		position.y = window.getSize().y;
		position.x = localPosition.x;
		relocation = true;
	}
	if (localPosition.y >= window.getSize().y - 1) {
		position.y = 0;
		position.x = localPosition.x;
		relocation = true;
	}

	if (relocation) {
		sf::Mouse::setPosition(position, window);
		firstMouse = true;
	}
	localPosition = sf::Mouse::getPosition(window);

	if (firstMouse) {
		lastX = localPosition.x;
		lastY = localPosition.y;
		firstMouse = false;
	}

	double xoffset = localPosition.x - lastX;
	double yoffset = localPosition.y - lastY;
	lastX = localPosition.x;
	lastY = localPosition.y;
	double sensitivity = 0.001f;
	double cameraSpeed = 0.002f * time;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset * cameraSpeed;
	pitch -= yoffset * cameraSpeed;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

void updateGPUData(int numberOfVertices) {
	//TUTAJ JEST ILOSC PUNKTOW
	GLfloat* vertices = new GLfloat[numberOfVertices * 6];

	float dalfa = 2 * 3.1415 / numberOfVertices;
	float alfa = 0;
	float R = 1;


	for (int i = 0; i < numberOfVertices * 6; i += 6) {
		vertices[i] = R * cos(alfa);
		vertices[i + 1] = R * sin(alfa);
		vertices[i + 2] = 0;

		/*
		vertices[i + 3] = (cos(alfa) + 1) / 2.0;
		vertices[i + 4] = (sin(alfa) + 1) / 2.0;
		vertices[i + 5] = (cos(alfa) + 1) / 2.0;
		*/
		vertices[i + 3] = (cos(alfa) + 1) / 2.0;
		vertices[i + 4] = (sin(alfa) + 1) / 2.0;
		//DODALEM TA LINIE, ZEBY POSZERZYC GAME KOLOROW (inny source)
		vertices[i + 5] = (float)i / (float)(numberOfVertices * 6);
		alfa += dalfa;
	}


	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numberOfVertices * 6, vertices, GL_STATIC_DRAW);

	delete[] vertices;

}

void StereoProjection(GLuint shaderProgram_, float _left, float _right, float _bottom, float _top, float _near, float _far, float _zero_plane, float _dist, float _eye)
{
	float   _dx = _right - _left;
	float   _dy = _top - _bottom;

	float   _xmid = (_right + _left) / 2.0;
	float   _ymid = (_top + _bottom) / 2.0;

	float   _clip_near = _dist + _zero_plane - _near;
	float   _clip_far = _dist + _zero_plane - _far;

	float  _n_over_d = _clip_near / _dist;

	float   _topw = _n_over_d * _dy / 2.0;
	float   _bottomw = -_topw;
	float   _rightw = _n_over_d * (_dx / 2.0 - _eye);
	float   _leftw = _n_over_d * (-_dx / 2.0 - _eye);

	glm::mat4 proj = glm::frustum(_leftw, _rightw, _bottomw, _topw, _clip_near, _clip_far);

	proj = glm::translate(proj, glm::vec3(-_xmid - _eye, -_ymid, 0));

	GLint uniProj = glGetUniformLocation(shaderProgram_, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
}

void loadModelOBJ(int& points, const char* filename, int buffer) {
	int vert_num = 0;
	int trian_num = 0;

	std::ifstream myReadFile;
	myReadFile.open(filename);

	std::string output;

	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			myReadFile >> output;
			if (output == "v") vert_num++;
			if (output == "f") trian_num++;
		}
	}

	myReadFile.close();
	myReadFile.open(filename);

	float** vert = new float* [vert_num];
	for (int k = 0; k < vert_num; k++)
		vert[k] = new float[3];

	int** trian = new int* [trian_num];
	for (int k = 0; k < trian_num; k++)
		trian[k] = new int[3];

	int licz_vert = 0;
	int licz_elem = 0;

	while (!myReadFile.eof()) {
		myReadFile >> output;
		if (output == "v") {
			for (int i = 0; i < 3; i++) {
				myReadFile >> vert[licz_vert][i];
			}
			licz_vert++;
		}
		if (output == "f") {
			for (int i = 0; i < 3; i++) {
				myReadFile >> trian[licz_elem][i];
			}
			licz_elem++;
		}
		output.clear();
	}

	GLfloat* vertices = new GLfloat[trian_num * 9];
	int vert_current = 0;

	for (int i = 0; i < trian_num; i++) {
		vertices[vert_current + 0] = vert[trian[i][0] - 1][0];
		vertices[vert_current + 1] = vert[trian[i][0] - 1][1];
		vertices[vert_current + 2] = vert[trian[i][0] - 1][2];

		vertices[vert_current + 3] = vert[trian[i][1] - 1][0];
		vertices[vert_current + 4] = vert[trian[i][1] - 1][1];
		vertices[vert_current + 5] = vert[trian[i][1] - 1][2];

		vertices[vert_current + 6] = vert[trian[i][2] - 1][0];
		vertices[vert_current + 7] = vert[trian[i][2] - 1][1];
		vertices[vert_current + 8] = vert[trian[i][2] - 1][2];

		vert_current += 9;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * trian_num * 9, vertices, GL_STATIC_DRAW);

	points = trian_num * 9;

	delete[] vertices;
	for (int i = 0; i < vert_num; i++)
		delete[] vert[i];
	delete[] vert;
	for (int i = 0; i < vert_num; i++)
		delete[] trian[i];
	delete[] trian;
}

void loadModelOBJ_EBO(int& points, const char* filename, int buffer_vbo, int buffer_ebo) {
	int vert_num = 0;
	int trian_num = 0;

	std::ifstream file;
	file.open(filename);

	std::string output;

	if (file.is_open()) {
		while (!file.eof()) {
			file >> output;
			if (output == "v") vert_num++;
			if (output == "f") trian_num++;
		}
	}

	file.close();
	file.open(filename);

	float* vert = new float[vert_num * 3];
	int* elem = new int[trian_num * 3];


	int vert_counter = 0;
	int elem_counter = 0;
	int tmp = 0;

	while (!file.eof()) {
		file >> output;
		if (output == "v") {
			for (int i = 0; i < 3; i++) {
				file >> vert[vert_counter++];
			}
		}
		if (output == "f") {
			for (int i = 0; i < 3; i++) {
				file >> tmp;
				elem[elem_counter++] = (tmp - 1);
			}
		}
		output.clear();
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vert_num * 3, vert, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * trian_num * 3, elem, GL_STATIC_DRAW);

	points = trian_num * 3;
	std::cout << "number of points: " << points << "\n";
	delete[] vert;
	delete[] elem;
}

void loadModelOBJNormalCoords(int& points, const char* filename, int buffer, std::vector<std::vector<int>>& objects) {
	int vert_num = 0;
	int triangles = 0;
	int normals = 0;
	int coord_num = 0;
	std::ifstream myReadFile;
	myReadFile.open(filename);
	std::string output;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			myReadFile >> output;
			if (output == "v") vert_num++;
			if (output == "f") triangles++;
			if (output == "vn") normals++;
			if (output == "vt") coord_num++;
			if (output == "o") {
				if (objects.size() == 0) {
					objects.push_back({ 0 });
				}
				else {
					objects.back().push_back(triangles * 3 - objects.back()[0]);
					objects.push_back({ triangles * 3 });
				}
			}
		}
		objects.back().push_back(triangles * 3 - objects.back()[0]);
	}

	myReadFile.close();
	myReadFile.open(filename);


	float** vert;
	vert = new float* [vert_num]; //przydzielenie pamiêci na w wierszy

	for (int i = 0; i < vert_num; i++)
		vert[i] = new float[3];


	int** trian;
	trian = new int* [triangles]; //przydzielenie pamiêci na w wierszy

	for (int i = 0; i < triangles; i++)
		trian[i] = new int[9];

	float** norm;
	norm = new float* [normals]; //przydzielenie pamiêci na w wierszy

	for (int i = 0; i < normals; i++)
		norm[i] = new float[3];

	float** coord;
	coord = new float* [coord_num]; //przydzielenie pamiêci na w wierszy

	for (int i = 0; i < coord_num; i++)
		coord[i] = new float[2];

	int licz_vert = 0;
	int licz_triang = 0;
	int licz_normals = 0;
	int licz_coord = 0;


	while (!myReadFile.eof()) {
		output = "";
		myReadFile >> output;
		if (output == "vn") { myReadFile >> norm[licz_normals][0]; myReadFile >> norm[licz_normals][1]; myReadFile >> norm[licz_normals][2]; licz_normals++; }
		if (output == "v") { myReadFile >> vert[licz_vert][0]; myReadFile >> vert[licz_vert][1]; myReadFile >> vert[licz_vert][2]; licz_vert++; }
		if (output == "vt") { myReadFile >> coord[licz_coord][0]; myReadFile >> coord[licz_coord][1]; licz_coord++; }
		if (output == "f") {

			for (int i = 0; i < 9; i += 3)
			{
				std::string s;
				myReadFile >> s;
				std::stringstream ss(s);

				std::vector <std::string> el;
				std::string item;


				while (getline(ss, item, '/')) {
					el.push_back(item);
				}
				trian[licz_triang][i] = std::stoi(el[0]);
				trian[licz_triang][i + 1] = std::stoi(el[1]);
				trian[licz_triang][i + 2] = std::stoi(el[2]);


			}
			licz_triang++;
		}
	}
	GLfloat* vertices = new GLfloat[triangles * 24];

	int vert_current = 0;

	for (int i = 0; i < triangles; i++)
	{
		vertices[vert_current] = vert[trian[i][0] - 1][0];
		vertices[vert_current + 1] = vert[trian[i][0] - 1][1];
		vertices[vert_current + 2] = vert[trian[i][0] - 1][2];
		vertices[vert_current + 3] = norm[trian[i][2] - 1][0];
		vertices[vert_current + 4] = norm[trian[i][2] - 1][1];
		vertices[vert_current + 5] = norm[trian[i][2] - 1][2];
		vertices[vert_current + 6] = coord[trian[i][1] - 1][0];
		vertices[vert_current + 7] = coord[trian[i][1] - 1][1];

		vertices[vert_current + 8] = vert[trian[i][3] - 1][0];
		vertices[vert_current + 9] = vert[trian[i][3] - 1][1];
		vertices[vert_current + 10] = vert[trian[i][3] - 1][2];
		vertices[vert_current + 11] = norm[trian[i][5] - 1][0];
		vertices[vert_current + 12] = norm[trian[i][5] - 1][1];
		vertices[vert_current + 13] = norm[trian[i][5] - 1][2];
		vertices[vert_current + 14] = coord[trian[i][4] - 1][0];
		vertices[vert_current + 15] = coord[trian[i][4] - 1][1];

		vertices[vert_current + 16] = vert[trian[i][6] - 1][0];
		vertices[vert_current + 17] = vert[trian[i][6] - 1][1];
		vertices[vert_current + 18] = vert[trian[i][6] - 1][2];
		vertices[vert_current + 19] = norm[trian[i][8] - 1][0];
		vertices[vert_current + 20] = norm[trian[i][8] - 1][1];
		vertices[vert_current + 21] = norm[trian[i][8] - 1][2];
		vertices[vert_current + 22] = coord[trian[i][7] - 1][0];
		vertices[vert_current + 23] = coord[trian[i][7] - 1][1];

		vert_current += 24;
	}

	for (auto i : objects) {
		for (auto j : i)
			std::cout << j << ' ';
		std::cout << std::endl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * triangles * 24, vertices, GL_STATIC_DRAW);

	points = triangles * 3;

	delete vertices;




	for (int i = 0; i < vert_num; i++)
		delete[] vert[i];
	delete[] vert;

	for (int i = 0; i < triangles; i++)
		delete[] trian[i];
	delete[] trian;

	for (int i = 0; i < normals; i++)
		delete[] norm[i];
	delete[] norm;

	for (int i = 0; i < coord_num; i++)
		delete[] coord[i];
	delete[] coord;
}

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	// Okno renderingu
	sf::Window window(sf::VideoMode(800, 800, 32), "Tomasz_Ligeza_OPEN_GL", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setFramerateLimit(60);
	window.setMouseCursorGrabbed(true);
	window.setMouseCursorVisible(false);
	// Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{
		std::cout << "glew - starting error\n";
		return 1;
	}

	// Utworzenie VAO (Vertex Array Object)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Utworzenie VBO (Vertex Buffer Object)
	// i skopiowanie do niego danych wierzcho³kowych
	GLuint vbo;
	glGenBuffers(1, &vbo);

	int numberOfPoints = 0;
	std::vector<std::vector<int>> objects;
	loadModelOBJNormalCoords(numberOfPoints, "objects/scene.obj", vbo, objects);

	// Utworzenie i skompilowanie shadera wierzcho³ków
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	if (!status) {
		std::cout << "vertexShader - compilation failed\n";
		char buffer[512];

		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		std::cout << buffer << "\n";
		return 1;
	}
	else {
		std::cout << "vertexShader - compilation success\n";
	}

	// Utworzenie i skompilowanie shadera fragmentów
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

	//W ZASADZIE POWTARZAM KOD POWY¯EJ Z DROBNYMI ZMIANAMI 
	if (!status) {
		std::cout << "fragmentShader - compilation failed\n";
		char buffer[512];

		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		std::cout << buffer << "\n";
		return 1;
	}
	else {
		std::cout << "fragmentShader - compilation success\n";
	}

	// Zlinkowanie obu shaderów w jeden wspólny program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specifikacja formatu danych wierzcho³kowych
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	/*GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));*/
	GLint TexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
	glEnableVertexAttribArray(TexCoord);
	glVertexAttribPointer(TexCoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

	//glm::mat4 view;
	//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	//glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.06f, 100.0f);
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");

	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	GLenum primitive = GL_TRIANGLES;
	int mouse_y = 3;
	glEnable(GL_DEPTH_TEST);

	//TUTAJ TEXTURY

	unsigned int metalTex;
	glGenTextures(1, &metalTex);
	glBindTexture(GL_TEXTURE_2D, metalTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("textures/metal.bmp", &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "loaded texture successfully" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(data);

	unsigned int mapTex;
	glGenTextures(1, &mapTex);
	glBindTexture(GL_TEXTURE_2D, mapTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("textures/map.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "loaded texture successfully" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(data);

	unsigned int woodTex;
	glGenTextures(1, &woodTex);
	glBindTexture(GL_TEXTURE_2D, woodTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("textures/wood.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "loaded texture successfully" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(data);

	unsigned int imageTex;
	glGenTextures(1, &imageTex);
	glBindTexture(GL_TEXTURE_2D, imageTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("textures/image.bmp", &width, &height, &nrChannels, 0);
	if (data) {
		std::cout << "loaded texture successfully" << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
	}

	stbi_image_free(data);

	//glUniform1i(glGetUniformLocation(shaderProgram, "metalTex"), 0);
	//glUniform1i(glGetUniformLocation(shaderProgram, "pieselTex"), 1);
	//glUniform1i(glGetUniformLocation(shaderProgram, "woodTex"), 2);
	//glUniform1i(glGetUniformLocation(shaderProgram, "obrazTex"), 3);


	// Rozpoczêcie pêtli zdarzeñ
	bool running = true;
	float dist = 13;
	int mode = 2;

	sf::Clock clock;
	sf::Time time;
	int counter = 0;

	while (running) {
		time = clock.restart();
		counter++;
		float fps = 1 / time.asSeconds();
		if (counter > fps) {
			window.setTitle("Tomasz_Ligeza_OPEN_GL: " + std::to_string(fps));
			counter = 0;
		}
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::KeyPressed:
				
				switch (windowEvent.key.code) {
				case sf::Keyboard::Escape:
					running = false;
				case sf::Keyboard::Num1:
					mode = 0;
					break;
				case sf::Keyboard::Num2:
					mode = 1;
					break;
				case sf::Keyboard::Num3:
					mode = 2;
					break;
				case sf::Keyboard::J:
					dist += 0.01;
					break;
				case sf::Keyboard::K:
					dist -= 0.01;
					break;
				}
				break;
			case sf::Event::MouseMoved:
				moveCamByMouse(uniView, time.asMicroseconds(), window);
				break;

			}
		}
		moveCamByKeyboard(uniView, time.asMicroseconds());

		// Nadanie scenie koloru czarnego
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);


		switch (mode) {
		case 0:
			glViewport(0, 0, window.getSize().x, window.getSize().y);
			glDrawBuffer(GL_BACK_LEFT);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, 0, dist, -0.05);
			glColorMask(true, false, false, false);
			glDrawArrays(primitive, 0, numberOfPoints);

			glClear(GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_BACK_RIGHT);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, 0, dist, 0.05);
			glColorMask(false, false, true, false);
			glDrawArrays(primitive, 0, numberOfPoints);
			glColorMask(true, true, true, true);
			break;
		case 1:
			glViewport(0, 0, window.getSize().x / 2, window.getSize().y);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, 0, 13, -0.05);
			glDrawArrays(primitive, 0, 24);

			glViewport(window.getSize().x / 2, 0, window.getSize().x / 2, window.getSize().y);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, 0, 13, 0.05);

			glDrawArrays(primitive, 0, 24);
			break;
		case 2:
			glActiveTexture(GL_TEXTURE_2D);
			glViewport(0, 0, window.getSize().x, window.getSize().y);

			glBindTexture(GL_TEXTURE_2D, woodTex);
			glDrawArrays(primitive, objects[0][0], objects[0][1]);

			glBindTexture(GL_TEXTURE_2D, metalTex);
			glDrawArrays(primitive, objects[1][0], objects[1][1]);

			glBindTexture(GL_TEXTURE_2D, imageTex);
			glDrawArrays(primitive, objects[2][0], objects[2][1]);

			glBindTexture(GL_TEXTURE_2D, mapTex);
			glDrawArrays(primitive, objects[3][0], objects[3][1]);
			break;
		}
		window.display();
	}
	// Kasowanie programu i czyszczenie buforów
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	// Zamkniêcie okna renderingu
	window.close();
	return 0;
}