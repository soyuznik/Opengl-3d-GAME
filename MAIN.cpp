


#include <iostream>
#include "utility.h"
#include "src/GLFW_WINDOW_SLOTS_IMPLEMENTATION.h"
#include "src/FMOD_SOUND_IMPLEMENTATION.h"
#include "src/TEXT.h"



Text* logger;
std::vector<std::string> log_text;

inline void LOG(std::string text) {
	log_text.push_back(text);
}
inline void WRITE_LOG() {
	int log_offset__ = 0;
	for (int i = 0; i < log_text.size(); i++) {
		logger->drawText(log_text[i], 5, (1050 - log_offset__), 0.4f, vec3(0.0f, 0.0f, 0.0f));
		log_offset__ += 20;
	}
	log_text.clear();
	
}


int main()
{

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// infinite frames
	glfwSwapInterval(0);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	

	Shader ourShader("resources/shaders/7.4.camera_vertex.glsl", "resources/shaders/7.4.camera_frag.glsl");
	Shader modelShader("resources/shaders/1.model_vertex.glsl", "resources/shaders/1.model_frag.glsl");
	logger = new Text(window, "resources/fonts/OpenSans-Light.ttf");

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	unsigned int boxVAO , planeVAO , sqVAO , wallVAO;
	AABB* playerbox;
	AABB* plane;
	AABB* wall;
/* ENTERING SCOPE ----------------------------------------------------------*/ {
		// set up vertex data (and buffer(s)) and configure vertex attributes
		// ------------------------------------------------------------------
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};
		float cube_vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,///
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,/// oppsite of  face you see on spawn
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,///

			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,//
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,///

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//-
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,//  face you see on spawn
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//--
											///
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,//--
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,//
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//-

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,///
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//
											// left face of spawn
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//
										   //// right face of spawn
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,//
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,// under face of spawn
										 //////
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,//
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,//
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,//

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,//
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,//
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//
										   //// upper face of spawn
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,//
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,//
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f///
		};

		std::vector<vec3> Player_points;
		for (unsigned int a = 0; a < sizeof(cube_vertices) / sizeof(cube_vertices[0]); a = a + 5) {
			Player_points.push_back(vec3(cube_vertices[a], cube_vertices[a + 1], cube_vertices[a + 2]));
		}
		playerbox = new AABB(Player_points[0], Player_points[0]);
		for (size_t i = 1; i < Player_points.size(); i++)
		{
			playerbox->UpdateMinMax(Player_points[i]);
		}
		//create box
		unsigned int VBO;
		glGenVertexArrays(1, &boxVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(boxVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//create plane
		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			// 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		   // -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		   // -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		   //  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		   // -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		   //  5.0f, -0.5f, -5.0f,  2.0f, 2.0f

			-5.5f, -0.1f, -5.5f,  0.0f, 0.0f,
			 5.5f, -0.1f, -5.5f,  1.0f, 0.0f,
			 5.5f,  0.0f, -5.5f,  1.0f, 1.0f,
			 5.5f,  0.0f, -5.5f,  1.0f, 1.0f,
			-5.5f,  0.0f, -5.5f,  0.0f, 1.0f,
			-5.5f, -0.2f, -5.5f,  0.0f, 0.0f,

			-5.5f, -0.1f,  5.5f,  0.0f, 0.0f,
			 5.5f, -0.1f,  5.5f,  1.0f, 0.0f,
			 5.5f,  0.0f,  5.5f,  1.0f, 1.0f,
			 5.5f,  0.0f,  5.5f,  1.0f, 1.0f,
			-5.5f,  0.0f,  5.5f,  0.0f, 1.0f,
			-5.5f, -0.1f,  5.5f,  0.0f, 0.0f,

			-5.5f,  0.0f,  5.5f,  1.0f, 0.0f,
			-5.5f,  0.0f, -5.5f,  1.0f, 1.0f,
			-5.5f, -0.1f, -5.5f,  0.0f, 1.0f,
			-5.5f, -0.1f, -5.5f,  0.0f, 1.0f,
			-5.5f, -0.1f,  5.5f,  0.0f, 0.0f,
			-5.5f,  0.0f,  5.5f,  1.0f, 0.0f,

			 5.5f,  0.0f,  5.5f,  1.0f, 0.0f,
			 5.5f,  0.0f, -5.5f,  1.0f, 1.0f,
			 5.5f, -0.1f, -5.5f,  0.0f, 1.0f,
			 5.5f, -0.1f, -5.5f,  0.0f, 1.0f,
			 5.5f, -0.1f,  5.5f,  0.0f, 0.0f,
			 5.5f,  0.0f,  5.5f,  1.0f, 0.0f,

			-5.5f, -0.1f, -5.5f,  0.0f, 1.0f,
			 5.5f, -0.1f, -5.5f,  1.0f, 1.0f,
			 5.5f, -0.1f,  5.5f,  1.0f, 0.0f,
			 5.5f, -0.1f,  5.5f,  1.0f, 0.0f,
			-5.5f, -0.1f,  5.5f,  0.0f, 0.0f,
			-5.5f, -0.1f, -5.5f,  0.0f, 1.0f,

			-5.5f,  0.0f, -5.5f,  0.0f, 1.0f,
			 5.5f,  0.0f, -5.5f,  1.0f, 1.0f,
			 5.5f,  0.0f,  5.5f,  1.0f, 0.0f,
			 5.5f,  0.0f,  5.5f,  1.0f, 0.0f,
			-5.5f,  0.0f,  5.5f,  0.0f, 0.0f,
			-5.5f,  0.0f, -5.5f,  0.0f, 1.0f
        };
		float wallVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			// 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		   // -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		   // -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		   //  5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		   // -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		   //  5.0f, -0.5f, -5.0f,  2.0f, 2.0f

			-5.5f, -5.5f, -0.2f,  0.0f, 0.0f,
			 5.5f, -5.5f, -0.2f,  1.0f, 0.0f,
			 5.5f,  5.5f, -0.2f,  1.0f, 1.0f,
			 5.5f,  5.5f, -0.2f,  1.0f, 1.0f,
			-5.5f,  5.5f, -0.2f,  0.0f, 1.0f,
			-5.5f, -5.5f, -0.2f,  0.0f, 0.0f,
				 			 
			-5.5f, -5.5f,  0.2f,  0.0f, 0.0f,
			 5.5f, -5.5f,  0.2f,  1.0f, 0.0f,
			 5.5f,  5.5f,  0.2f,  1.0f, 1.0f,
			 5.5f,  5.5f,  0.2f,  1.0f, 1.0f,
			-5.5f,  5.5f,  0.2f,  0.0f, 1.0f,
			-5.5f, -5.5f,  0.2f,  0.0f, 0.0f,
				   			 
			-5.5f,  5.5f,  0.2f,  1.0f, 0.0f,
			-5.5f,  5.5f, -0.2f,  1.0f, 1.0f,
			-5.5f, -5.5f, -0.2f,  0.0f, 1.0f,
			-5.5f, -5.5f, -0.2f,  0.0f, 1.0f,
			-5.5f, -5.5f,  0.2f,  0.0f, 0.0f,
			-5.5f,  5.5f,  0.2f,  1.0f, 0.0f,
					   		
			 5.5f,  5.5f,  0.2f,  1.0f, 0.0f,
			 5.5f,  5.5f, -0.2f,  1.0f, 1.0f,
			 5.5f, -5.5f, -0.2f,  0.0f, 1.0f,
			 5.5f, -5.5f, -0.2f,  0.0f, 1.0f,
			 5.5f, -5.5f,  0.2f,  0.0f, 0.0f,
			 5.5f,  5.5f,  0.2f,  1.0f, 0.0f,
				  			 
			-5.5f, -5.5f, -0.2f,  0.0f, 1.0f,
			 5.5f, -5.5f, -0.2f,  1.0f, 1.0f,
			 5.5f, -5.5f,  0.2f,  1.0f, 0.0f,
			 5.5f, -5.5f,  0.2f,  1.0f, 0.0f,
			-5.5f, -5.5f,  0.2f,  0.0f, 0.0f,
			-5.5f, -5.5f, -0.2f,  0.0f, 1.0f,
			 		   		 
			-5.5f,  5.5f, -0.2f,  0.0f, 1.0f,
			 5.5f,  5.5f, -0.2f,  1.0f, 1.0f,
			 5.5f,  5.5f,  0.2f,  1.0f, 0.0f,
			 5.5f,  5.5f,  0.2f,  1.0f, 0.0f,
			-5.5f,  5.5f,  0.2f,  0.0f, 0.0f,
			-5.5f,  5.5f, -0.2f,  0.0f, 1.0f
		};	 




		// AABB collision
		std::vector<vec3> ppoints;

		for (unsigned int a = 0; a < sizeof(planeVertices) / sizeof(planeVertices[0]); a = a + 5) {
			ppoints.push_back(vec3(planeVertices[a], planeVertices[a + 1], planeVertices[a + 2]));
		}
		// create a plane with 10x10 smaller planes
		// update the Min/Max points accordingly
		plane = new AABB(ppoints[0], ppoints[0]);
		for (size_t i = 1; i < ppoints.size(); i++)
		{
			for (int i = 1; i < 10; i++) {
				for (int j = 1; j < 10; j++) {
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(11.0f * i, -0.51f, 11.0f * j));
					glm::vec4 point = (model * glm::vec4(ppoints[i].x, ppoints[i].y, ppoints[i].z, 1.0f));
					plane->UpdateMinMax(glm::vec3(point.x, point.y, point.z));
				}
			}
		}
		// AABB collision
		ppoints.clear();

		for (unsigned int a = 0; a < sizeof(wallVertices) / sizeof(wallVertices[0]); a = a + 5) {
			ppoints.push_back(vec3(wallVertices[a], wallVertices[a + 1], wallVertices[a + 2]));
		}
		// create a plane with 10x10 smaller planes
		// update the Min/Max points accordingly
		wall = new AABB(ppoints[0], ppoints[0]);
		for (size_t i = 1; i < ppoints.size(); i++)
		{
		   glm::vec4 point = (glm::vec4(ppoints[i].x, ppoints[i].y, ppoints[i].z, 1.0f));
		   wall->UpdateMinMax(glm::vec3(point.x, point.y, point.z));	
		}




		//create box
		unsigned int VBO1;
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &VBO1);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);


		//square to draw shoot effect
		float sqVertices[] = {
			 -0.5f, -0.5f,     0.0f,    0.0f,  0.0f,
			  0.5f, -0.5f,     0.0f,    1.0f,  0.0f,
			  0.5f,  0.5f,     0.0f,    1.0f,  1.0f,
			  0.5f,  0.5f,     0.0f,    1.0f,  1.0f,
			 -0.5f,  0.5f,     0.0f,    0.0f,  1.0f,
			 -0.5f, -0.5f,     0.0f,    0.0f,  0.0
		};
		//create square
		unsigned int VBO2;
		glGenVertexArrays(1, &sqVAO);
		glGenBuffers(1, &VBO2);
		glBindVertexArray(sqVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sqVertices), sqVertices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);


		//create wall
		unsigned int VBO3;
		glGenVertexArrays(1, &wallVAO);
		glGenBuffers(1, &VBO3);
		glBindVertexArray(wallVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO3);
		glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coord attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

/* LEAVING SCOPE ----------------------------------------------------------*/ }




	//create gun
	Model gun("resources/m4a1.obj" , "resources/gray.png");
	Model deagle("resources/deagle.obj" , "resources/gray.png");
	Model bullet("resources/sphere.obj", "resources/cooper.jpg");
	//create building
	Model cottage("resources/models/cottage.obj" , "resources/models/cottage.png");
	Model church("resources/models/untitled.obj", "resources/models/church.jpg");


	//load gun texture
	unsigned int texture1 = loadTextureX("resources/gray.png");
	//load box texture
	unsigned int texture2 = loadTextureX("resources/textures/container.jpg");
	//load plane texture
	unsigned int texture3 = loadTextureX("resources/textures/brickwall.jpg");
	//load explosion animation
	std::vector<unsigned int> texture4anim;
	for (int i = 0; i < 8; i++) {
		std::string path = std::string("resources/MUZZLE/muzzle") + std::to_string(i) + std::string(".png");
		texture4anim.push_back(loadTextureY(path));
	}
	//load crosshair texture
	unsigned int texture5 = loadTextureY("resources/crosshair.png");



	




	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	//load sounds
	FMOD_RESULT result;
	FMOD::System* system;
	FMOD::Sound* m4_sound;
	FMOD::Sound* deag_sound;
	FMOD::Sound* mainmusic;
	FMOD::ChannelGroup* musicGroup = nullptr;
	FMOD::Channel* mchannel;
	FMOD::ChannelGroup* channelGroup = nullptr;

	result = FMOD::System_Create(&system);		// Create the main system object.
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}

	result = system->init(100, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.

	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
	// create FMOD sounds
	
	result = system->createSound("resources/m4a1_ff.mp3", FMOD_DEFAULT, FMOD_DEFAULT, &m4_sound);		// FMOD_DEFAULT uses the defaults.  These are the same as FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE.
	result = system->createSound("resources/desert_s.mp3", FMOD_DEFAULT, FMOD_DEFAULT, &deag_sound);
	result = system->createSound("resources/music.mp3", FMOD_DEFAULT, FMOD_DEFAULT, &mainmusic);
	result = system->createChannelGroup("Shooting", &channelGroup);
	result = system->createChannelGroup("Music", &musicGroup);


	
	//system->playSound(mainmusic, musicGroup, false, &mchannel);



	// render loop
	// -----------
	vec3 houseWorldMat4;
	while (!glfwWindowShouldClose(window))
	{
		
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// translate camera box
		playerbox->mMin = +camera.Position;
		playerbox->mMax = +camera.Position;

		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if the camera should jump anymore...
		if (camera.should_jump && jump_cooldown > 0.0f) {
			jump_cooldown = 0.0f;
			camera.should_jump = false;
			camera.Position.y += 0.1f;
		}
		else {
			jump_cooldown += deltaTime;
		}


		camera.Position.y -= 0.01f;
		// Do we collide with this PlaneActor?
		const AABB& planeBox = *plane;
		if (Intersect(*playerbox, planeBox))
		{
			vec3 pos = camera.Position;
			// Calculate all our differences
			float dx1 = planeBox.mMax.x - playerbox->mMin.x;
			float dx2 = planeBox.mMin.x - playerbox->mMax.x;
			float dy1 = planeBox.mMax.y - playerbox->mMin.y;
			float dy2 = planeBox.mMin.y - playerbox->mMax.y;
			float dz1 = planeBox.mMax.z - playerbox->mMin.z;
			float dz2 = planeBox.mMin.z - playerbox->mMax.z;

			// Set dx to whichever of dx1/dx2 have a lower abs
			float dx = Math::Abs(dx1) < Math::Abs(dx2) ?
				dx1 : dx2;
			// Ditto for dy
			float dy = Math::Abs(dy1) < Math::Abs(dy2) ?
				dy1 : dy2;
			// Ditto for dz
			float dz = Math::Abs(dz1) < Math::Abs(dz2) ?
				dz1 : dz2;

			// Whichever is closest, adjust x/y position
			if (Math::Abs(dx) <= Math::Abs(dy) && Math::Abs(dx) <= Math::Abs(dz))
			{
				pos.x += dx;
			}
			else if (Math::Abs(dy) <= Math::Abs(dx) && Math::Abs(dy) <= Math::Abs(dz))
			{
				pos.y += dy;
			}
			else
			{
				pos.z += dz;
			}

			// Need to set position and update box component
			camera.Position = pos;





		}

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // clear viewport with a blue color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear depth
		glEnable(GL_DEPTH_TEST); // enable depth testing
//
		
		// activate shader
		ourShader.use();


		//draw crosshair
		glm::mat4 projection = glm::mat4(1.0f);
		ourShader.setMat4("projection", projection);
		// camera/view transformation
		glm::mat4 view = glm::mat4(1.0f);
		ourShader.setMat4("view", view);

		// CAST RAY TO 0,0,0
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		ourShader.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glBindVertexArray(sqVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);




		/*
		-
		 -          USE MODEL SHADER FOR RENDERING MODELS
		-
		*/

		modelShader.use();
		   
		// pass projection matrix to shader (note that in this case it could change every frame)
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		modelShader.setMat4("projection", projection);

		// camera/view transformation
		view = camera.GetViewMatrix();
		modelShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), houseWorldMat4);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelShader.setMat4("model", model);
		cottage.Draw(modelShader);


		// pass projection matrix to shader (note that in this case it could change every frame)
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		modelShader.setMat4("projection", projection);

		// camera/view transformation
		view = camera.GetViewMatrix();
		modelShader.setMat4("view", view);

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -0.5f, 20.0f));





		model = glm::translate(glm::mat4(1.0f), glm::vec3(50.0f, 6.5f, 20.0f));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//rot = glm::rotate(rot, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		modelShader.setMat4("model", model);

		church.Draw(modelShader);


		// back to old shader

		ourShader.use();
		// pass projection matrix to shader (note that in this case it could change every frame)
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		// camera/view transformation
		view = camera.GetViewMatrix();
		ourShader.setMat4("view", view);
		
		// Construct segment in direction of travel

		_ShootingDelay += deltaTime;
		if ((Shooting) && (_ShootingDelay > 0.1f) && (display_gun)) {
			// Create line segment
			bulletPositions.push_back(camera.Position);	
			bulletFronts.push_back(camera.Front);
			bulletQua.push_back(Quaternion::Identity);
			_ShootingDelay = 0;
		}
		
		if ((Shooting) && (display_deagle) && should_deagle_shoot) {
			// Create line segment
			bulletPositions.push_back(camera.Position);
			bulletFronts.push_back(camera.Front);
			bulletQua.push_back(Quaternion::Identity);
			_ShootingDelay = 0;
			should_deagle_shoot = false;
		}
		for (int i = 0; i < bulletPositions.size(); i++) {
			
			////////////////////////
			const float segmentLength = 10.0f;
			vec3 start = bulletPositions[i];
			vec3 dir = bulletFronts[i];
			vec3 end = start + dir * segmentLength;
			LineSegment l(start, end);

			bool collided = false;
			// Initialize closestT to infinity, so first
			// intersection will always update closestT
			float closestT = Math::Infinity;
			vec3 __norm;
			float t;
			// here is made to check for more planes instead of just one ,
			// so read again Chapter 11
			// check for planes
			if (Intersect(l, *plane, t, __norm)) {
				if (t < closestT)
				{
					std::stringstream ss;
					ss << "Touched at -- " << bulletPositions[i].x << " / " << bulletPositions[i].y <<  " / " << bulletPositions[i].z << std::endl;
					//houseWorldMat4 = bulletPositions[i];
					LOG(ss.str());
					ss.clear();
					vec3 bullNorm = glm::normalize(bulletFronts[i]);
					vec3 norm = glm::normalize(__norm);
					// equation for reflection
					dir = bullNorm - 2.0f * glm::dot(bullNorm, norm) * norm;
				}
			}
			//check for walls
			if (Intersect(l, *wall, t, __norm)) {
				if (t < closestT)
				{
					std::stringstream ss;
					ss << "Touched at -- " << bulletPositions[i].x << " / " << bulletPositions[i].y << " / " << bulletPositions[i].z << std::endl;
					//houseWorldMat4 = bulletPositions[i];
					LOG(ss.str());
					ss.clear();
					vec3 bullNorm = glm::normalize(bulletFronts[i]);
					vec3 norm = glm::normalize(__norm);
					// equation for reflection
					dir = bullNorm - 2.0f * glm::dot(bullNorm, norm) * norm;
				}
			}
			// Figure out difference between original (unit x) and new
			float dot = glm::dot(vec3(1 , 0 , 0), dir);
			float angle = Math::Acos(dot);
			// Facing down X
			if (dot > 0.9999f)
			{
				bulletQua[i] = Quaternion::Identity;
			}
			// Facing down -X
			else if (dot < -0.9999f)
			{
				bulletQua[i] = Quaternion(Vector3::UnitZ, Math::Pi);
			}
			else
			{
				// Rotate about axis from cross product
				Vector3 axis = Vector3::Cross(Vector3::UnitX, Vector3(dir.x ,dir.y , dir.z));
				axis.Normalize();
				bulletQua[i] = Quaternion(axis, angle);
			}
			bulletPositions[i] += dir * (50.0f * deltaTime);// changed speed

			

			model = glm::translate(glm::mat4(1.0f), bulletPositions[i]);
			model *= Matrix4::glm_CreateFromQuaternion(bulletQua[i]);
			model = glm::scale(model, vec3(0.01f, 0.01f, 0.01f));
			ourShader.setMat4("model", model);
			bullet.Draw(ourShader);
		}
		if (!Shooting) {
			if (!bulletPositions.empty() && 
				(((bulletPositions[0].x - camera.Position.x) > 50) ||
				((bulletPositions[0].y - camera.Position.y) > 50) ||
				((bulletPositions[0].z - camera.Position.z) > 50)))
			{
				bulletPositions.pop_front();
				bulletFronts.pop_front();
				bulletQua.pop_front();
			}
			should_deagle_shoot = true;
		}
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(20.0f, -0.0f, 20.0f));
		model = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
		ourShader.setMat4("model", model);
		
		//draw plane 10x10 small planes
		for (int i = 1; i < 10; i++) {
			for (int j = 1; j < 10; j++) {
				model = glm::translate(glm::mat4(1.0f), glm::vec3(11.0f * i, -0.51f, 11.0f * j));
				ourShader.setMat4("model", model);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture3);
				glBindVertexArray(planeVAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				
			}
		}
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		ourShader.setMat4("model", model);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(wallVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// update FMOD system
		system->update();

		//check if should apply effect.
		glClear(GL_DEPTH_BUFFER_BIT);
		
		if (Shooting && ADS && display_deagle && _AnimationIndex < 8) {
			glEnable(GL_BLEND);
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			// pass projection matrix to shader 
			glm::mat4 projection = glm::mat4(1.0f);
			ourShader.setMat4("projection", projection);
			// camera/view transformation
			glm::mat4 view = glm::mat4(1.0f);
			ourShader.setMat4("view", view);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			ourShader.setMat4("model", model);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture4anim[_AnimationIndex]);
			
			time_passed += deltaTime;
			if (time_passed > 0.02f) {
				_AnimationIndex++;
				time_passed = 0;
			}
			glBindVertexArray(sqVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisable(GL_BLEND); 
		}
		if (Shooting && !ADS && display_deagle && _AnimationIndex < 8) {
			glEnable(GL_BLEND);
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			// pass projection matrix to shader 
			glm::mat4 projection = glm::mat4(1.0f);
			ourShader.setMat4("projection", projection);
			// camera/view transformation
			glm::mat4 view = glm::mat4(1.0f);
			ourShader.setMat4("view", view);

			glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.241f, 0.3f, 0.4f));
			ourShader.setMat4("model", model);

			glBindTexture(GL_TEXTURE_2D, texture4anim[_AnimationIndex]);
			
			time_passed += deltaTime;
			if (time_passed > 0.03f) {
				_AnimationIndex++;
				time_passed = 0;
			}
			glBindVertexArray(sqVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glDisable(GL_BLEND);
		}
		if (!Shooting) {
			//reset animations
			if (_AnimationIndex >= 8) { _AnimationIndex = 0; }
		}




		//check which gun to dispaly
		if (display_gun) {
			//update state
			if (Shooting && ShootingFinished) {
				std::thread ShootingPlayback(&playShooting, channelGroup, m4_sound, system);
				ShootingFinished = false;
				ShootingPlayback.detach();
			}
			else if (!Shooting) {
				channelGroup->stop();
			}

			// draw fps gun
			// bind textures on corresponding texture units
			//glDisable(GL_DEPTH_TEST);
			glClear(GL_DEPTH_BUFFER_BIT);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);

			// here just a bunch of view , projection & model transformation so the GunModel appears on screen...
			glm::mat4 gunView = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
			gunView = glm::rotate(gunView, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::mat4(1.0f);

			// AUTO FIRE
			if (ADS) {// if ADS enabled move gun to the middle instead
				glm::mat4 gunView = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
				gunView = glm::rotate(gunView, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(2.0f, -2.5f, 0.0f));
			}
			// Automatic fire , move gun back and forth , randomly/.....
			if (Shooting && should_move) {
				model = glm::translate(model, glm::vec3(0.0f, 0.1f, s_rand(0.0f, 1.0f)));
				should_move = false;
			}
			else if (!should_move && Shooting) {
				model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.1f));
				should_move = true;
			}
			// set the matrices and draw the model...
			ourShader.setMat4("model", model);
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", gunView);
			gun.Draw(ourShader);
		}
		else if (display_deagle) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glClear(GL_DEPTH_BUFFER_BIT);

			//update state
			// the deagle has SINGLE fire so it should fire just when clicked
			if (Shooting && ShootingFinished && !shoot_cooldown) {
				shoot_cooldown = true;
				std::thread ShootingPlayback(&playShooting, channelGroup, deag_sound, system);
				ShootingFinished = false;
				ShootingPlayback.detach();
			}
			// move gun in the right corener
			glm::mat4 gunView = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
			model = glm::mat4(1.0f);
			model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::translate(model, glm::vec3(-1.0f, 4.5f, 0.0f));
			// SINGLE FIRE
			if (ADS) {// if ADS move gun in the middle insead
				glm::mat4 gunView = glm::lookAt(glm::vec3{ 0 }, glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
				model = glm::mat4(1.0f);
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				model = glm::translate(model, glm::vec3(1.0f, 5.5f, 0.0f));
			}
			if (Shooting && _AnimationIndex < 8) { // if shooting move gun up ...
				model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.1f, 0.0f));
				should_move = false;
			}
			// Draw gun Model
			ourShader.setMat4("model", model);
			ourShader.setMat4("projection", projection);
			ourShader.setMat4("view", gunView);
			deagle.Draw(ourShader);
		}
		
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		WRITE_LOG();
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
