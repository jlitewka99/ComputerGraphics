#include <iostream>
#include <SFML\Graphics.hpp>
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SFML/System/Time.hpp>


// Kody shaderów
const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
out vec3 Color;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 proj;
void main(){
Color = color;
gl_Position = proj * view * model * vec4(position, 1.0);
}
)glsl";

const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;
out vec4 outColor;
 
void main()
{
outColor = vec4(Color, 1.0);
}
)glsl";

void cube(int buffer) {
	int points = 36;

	float vertices[] = {
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

			0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 1.0f,

			-0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f,
			0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
			0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,
			-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.5f
	};

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * points * 6, vertices, GL_STATIC_DRAW);
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

	// Utworzenie VBO (Vertex Buffer Object)
	// i skopiowanie do niego danych wierzcho³kowych
	GLuint vbo;
	glGenBuffers(1, &vbo);

	cube(vbo);

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
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

	GLint uniView = glGetUniformLocation(shaderProgram, "view");

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.06f, 100.0f);
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	glEnable(GL_DEPTH_TEST);

	sf::Clock clock;
	double deltaTime = 0.f;

	int licznik = 0;
	int primitive = GL_TRIANGLE_FAN;
	// Rozpoczêcie pêtli zdarzeñ
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
				}
				}
				break;
			}
		}

		licznik++;
		deltaTime = clock.restart().asSeconds();

		float fps = 1 / deltaTime;
		if (licznik >= fps) {
			window.setTitle(std::to_string(fps));
			licznik = 0;
		}

		setCam(uniView, deltaTime, window);

		// Nadanie scenie koloru czarnego
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//DRAWING
		glDrawArrays(primitive, 0, 36);
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