#include <iostream>
#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/System/Time.hpp>
#include <fstream>

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
Color = color;
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
outColor = vec4(Color, 1.0);
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

float rotation = 0;
sf::Vector2i last{0, 0};
double yaw = -90;
double pitch = 0;

void setCam(GLint _uView, double deltaTime, const sf::Window& window) {
	float keyboardCameraSpeed = 3.f * deltaTime;
	float mouseCameraSpeed = 10.f * deltaTime;
	//PORUSZANIE KLAWIATUR¥
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		cameraPos += keyboardCameraSpeed * cameraFront;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		cameraPos -= keyboardCameraSpeed * cameraFront;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * keyboardCameraSpeed;

		//rotation -= cameraSpeed;
		//cameraFront.x = sin(rotation);
		//cameraFront.z = -cos(rotation);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * keyboardCameraSpeed;
		//rotation += cameraSpeed;
		//cameraFront.x = sin(rotation);
		//cameraFront.z = -cos(rotation);
	}

	//PORUSZANIE MYSZ¥
	//TO JEST MEGA CIULOWE, NIE MO¯NA ZROBIÆ ALT+TAB
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
		position.x = localPosition.x;
		position.y = window.getSize().y;
		relocation = true;
	}
	if (localPosition.y >= window.getSize().y - 1) {
		position.x = localPosition.y;
		position.y = 0;
		relocation = true;
	}

	if (relocation) {
		sf::Mouse::setPosition(position);
	}
	localPosition = sf::Mouse::getPosition(window);
	
	sf::Vector2i offset = localPosition - last;
	if (relocation)
		offset = { 0, 0 };
	last = localPosition;

	double sensitivity = 0.1f;
	//double cameraSpeed = 0.005f * deltaTime;

	offset.x *= sensitivity;
	offset.y *= sensitivity;

	yaw += offset.x * mouseCameraSpeed;
	pitch -= offset.y * mouseCameraSpeed;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glUniformMatrix4fv(_uView, 1, GL_FALSE, glm::value_ptr(view));
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

	delete [] vertices;

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

	float* vert = new float[vert_num*3];
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


int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;

	// Okno renderingu
	sf::Window window(sf::VideoMode(800, 800, 32), "Tomasz_Ligeza_GK", sf::Style::Titlebar | sf::Style::Close, settings);
	//sf::Window window(sf::VideoMode(1920, 1080, 32), "Tomasz_Ligeza_GK", sf::Style::Fullscreen | sf::Style::Close, settings);
	window.setFramerateLimit(60);
	window.setMouseCursorGrabbed(true);
	window.setMouseCursorVisible(false);

	// Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Utworzenie VAO (Vertex Array Object)
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	int points = 0;

	GLuint ebo;
	glGenBuffers(1, &ebo);
	


	// Utworzenie VBO (Vertex Buffer Object)
	// i skopiowanie do niego danych wierzcho³kowych
	GLuint vbo;
	glGenBuffers(1, &vbo);

	loadModelOBJ(points, "object/scene.obj", vbo);
	//cube(vbo);

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

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);


	// Specifikacja formatu danych wierzcho³kowych

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	GLint TexCoords = glGetAttribLocation(shaderProgram, "aTexCoord");
	glEnableVertexAttribArray(TexCoords);
	glVertexAttribPointer(TexCoords, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));


	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

	GLint uniView = glGetUniformLocation(shaderProgram, "view");

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.06f, 100.0f);
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	glEnable(GL_DEPTH_TEST);

	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width;
	int height;
	int nrChannel;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load("textures/1.jpg", &width, &height, &nrChannel, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "something went wrong\n";
	}
	stbi_image_free(data);	

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	data = stbi_load("textures/2.jpg", &width, &height, &nrChannel, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "something went wrong\n";
	}
	stbi_image_free(data);

	glUniform1f(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1f(glGetUniformLocation(shaderProgram, "texture2"), 1);


	sf::Clock clock;
	double deltaTime = 0.f;

	int counter = 0;
	int primitive = GL_POINTS;
	// Rozpoczêcie pêtli zdarzeñ

	int mode = 3;
	float dist = 13;
	float zeroPlane = 0;
	float eye = 0.05;


	bool running = true;
	while (running) {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::KeyPressed:
				switch (windowEvent.key.code) {
				case sf::Keyboard::Escape: {
					running = false;
					break;
				}
				//	F1-F10
				{
				case sf::Keyboard::F1: {
					primitive = GL_POINTS;
					break;
				}
				case sf::Keyboard::F2: {
					primitive = GL_LINES;
					break;
				}
				case sf::Keyboard::F3: {
					primitive = GL_LINE_STRIP;
					break;
				}
				case sf::Keyboard::F4: {
					primitive = GL_LINE_LOOP;
					break;
				}
				case sf::Keyboard::F5: {
					primitive = GL_TRIANGLES;
					break;
				}
				case sf::Keyboard::F6: {
					primitive = GL_TRIANGLE_STRIP;
					break;
				}
				case sf::Keyboard::F7: {
					primitive = GL_TRIANGLE_FAN;
					break;
				}
				case sf::Keyboard::F8: {
					primitive = GL_QUADS;
					break;
				}
				case sf::Keyboard::F9: {
					primitive = GL_QUAD_STRIP;
					break;
				}
				case sf::Keyboard::F10: {
					primitive = GL_POLYGON;
					break;
				}
				case sf::Keyboard::Q:
					dist += 0.1;
					break;
				case sf::Keyboard::W:
					dist -= 0.1;
					break;
				case sf::Keyboard::A:
					zeroPlane += 0.1;
					break;
				case sf::Keyboard::S:
					zeroPlane -= 0.1;
					break;
				case sf::Keyboard::Z:
					eye += 0.005;
					break;
				case sf::Keyboard::X:
					eye -= 0.005;
					break;
				case sf::Keyboard::Num1:
					mode = 1;
					break;
				case sf::Keyboard::Num2:
					mode = 2;
					break;
				case sf::Keyboard::Num3:
					mode = 3;
					break;
				}
				}
				break;
			}
		}

		counter++;
		deltaTime = clock.restart().asSeconds();

		float fps = 1 / deltaTime;
		if (counter >= fps) {
			window.setTitle(std::to_string(fps));
			counter = 0;
		}

		setCam(uniView, deltaTime, window);

		// Nadanie scenie koloru czarnego
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (mode) {
		case 1:
			glViewport(0, 0, window.getSize().x, window.getSize().y);
			glDrawBuffer(GL_BACK_LEFT);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zeroPlane, dist, eye);
			
			glColorMask(true, false, false, false);

			glBindTexture(GL_TEXTURE_2D, texture1);
			glDrawArrays(primitive, 0, 12);

			glBindTexture(GL_TEXTURE_2D, texture2);
			glDrawArrays(primitive, 12, 36);

			glClear(GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_BACK_RIGHT);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zeroPlane, dist, -eye);
			
			glColorMask(false, false, true, false);

			glBindTexture(GL_TEXTURE_2D, texture1);
			glDrawArrays(primitive, 0, 12);

			glBindTexture(GL_TEXTURE_2D, texture2);
			glDrawArrays(primitive, 12, 36);
			glColorMask(true, true, true, true);
			break;
		case 2:
			glViewport(0, 0, window.getSize().x / 2, window.getSize().y);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zeroPlane, dist, eye);

			glBindTexture(GL_TEXTURE_2D, texture1);
			glDrawArrays(primitive, 0, 12);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glDrawArrays(primitive, 12, 36);

			glViewport(window.getSize().x / 2, 0, window.getSize().x / 2, window.getSize().y);
			StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zeroPlane, dist, -eye);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glDrawArrays(primitive, 0, 12);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glDrawArrays(primitive, 12, 36);
			break;
		case 3:
			glViewport(0, 0, window.getSize().x, window.getSize().y);

			glDrawArrays(primitive, 0, points);
			glDrawElements(primitive, points, GL_UNSIGNED_INT, 0);
			break;
		}

		// Wymiana buforów tylni/przedni
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