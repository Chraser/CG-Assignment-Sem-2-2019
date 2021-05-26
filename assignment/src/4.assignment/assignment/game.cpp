/*
Name: Kay Men Yap
Student ID: 192575442
Last modified: 27/10/2019


The assignment used the sample_2 file and the header files from tutorial 1 as the base file and modified
to fit the assignment functionality. Also used light attenuation code from
https://learnopengl.com/Lighting/Light-casters
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>

#include <iostream>
#include <string>
#include <cmath>

#define PI 3.14159265

// Box coordinate with 36 vertices.
// Every 3 coordinates will form 1 triangle.
// The last 2 columns represent texture coordinate for mapping.
float box[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);
glm::vec3 moveObjectToCamera(glm::vec3 object_pos);
unsigned int loadTexture(char const * path);
void update_animations();
void update_torch_animation();
void update_leg_animation();
void update_jumping_animation();
void update_spike_animation();

// settings
const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 1080;
const int MIN_RADIUS = 0;
const int MAX_RADIUS = 8;
const glm::vec3 SHEEP_DIRECTION = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 SHEEP_START_LOCATION = glm::vec3(1.0f, 0.0f,-5.0f);
const glm::vec3 SVEN_START_LOCATION = glm::vec3(1.0f, 0.0f, -2.0f);
const glm::vec3 WIN_FLOOR_LOCATION = glm::vec3(0.0f, 0.0f, 23.0f);
const glm::vec3 TORCH_START_LOCATION = glm::vec3(0.0f, 0.25f, 0.0f);
const glm::vec3 BRIGHT_MODE_AMBIENT = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 NOT_BRIGHT_MODE_AMBIENT = glm::vec3(0.1f, 0.1f, 0.1f);

// camera
Camera camera = Camera();

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//index for the brightness radius array of values for linear and quadratic portion of attenuation
int brightness_radius_index = 4;

//position variables of objects that will move
glm::vec3 sheep_position = SHEEP_START_LOCATION;
glm::vec3 sven_position = SVEN_START_LOCATION;
glm::vec3 torch_position = glm::vec3(0.0f, 0.25f, 0.0f);

// timing
float delta_time = 0.0f;	// time between current frame and last frame
float last_frame = 0.0f;

//Toggle (Animation or states)
bool TORCH_PICKED_UP = false;
bool SVEN_PICKED_UP = false;
int BUTTON_DELAY = 0;
int BRIGHTNESS_RADIUS_DELAY = 0;
int ORTHO_DELAY = 0;
int RESET_DELAY = 0;
int JUMP_DELAY = 0;
int TORCH_PICK_UP_DELAY = 0;
int SVEN_PICK_UP_DELAY = 0;
bool TORCH_CLOSE_ENOUGH = false;
bool SVEN_CLOSE_ENOUGH = false;
bool GAME_LOST = false;
bool GAME_WIN = false;
bool BRIGHT_MODE = false;
bool ORTHOGRAPHIC_MODE = false;
extern bool JUMPING_MODE;
bool SHOW_COORDINATE = false;
int SHOW_DELAY = 0;


//Animation Variables
bool rotate_forward = true;
bool torch_going_up = true;
bool camera_going_up = true;
bool spike_going_up = true;
float leg_angle = 0.0; //used for animating the legs to moving as if the object is walking
float torch_animation_rotation = 0.0f;
float spike_translation_y = -0.5f;
const float MIN_SPIKE_HEIGHT = -0.5f;
const float MAX_SPIKE_HEIGHT = 0.0f;

// Countdown until the button trigger can be pressed again.
// This prevents accidental burst repeat clicking of the key.
void update_delay()
{
	if(BUTTON_DELAY > 0) BUTTON_DELAY -= 1;
	if(BRIGHTNESS_RADIUS_DELAY > 0) BRIGHTNESS_RADIUS_DELAY -= 1;
	if(ORTHO_DELAY > 0) ORTHO_DELAY -= 1;
	if(RESET_DELAY > 0) RESET_DELAY -= 1;
	if(SHOW_DELAY > 0) SHOW_DELAY -= 1;
	if(JUMP_DELAY > 0) JUMP_DELAY -= 1;
	if(TORCH_PICK_UP_DELAY > 0) TORCH_PICK_UP_DELAY -= 1;
	if(SVEN_PICK_UP_DELAY > 0) SVEN_PICK_UP_DELAY -= 1;
}

// Toggle torch pick up only if the camera is close enough.
void toggle_torch_pickup_distance()
{
	if(glm::length(camera.Position - torch_position) <= 2.5f)
		TORCH_CLOSE_ENOUGH = true;
	else
		TORCH_CLOSE_ENOUGH = false;
}

// Toggle picking up sven only if the camera is close enough.
void toggle_sven_pickup_distance()
{
	if(glm::length(camera.Position - sven_position) <= 2.5f)
		SVEN_CLOSE_ENOUGH = true;
	else
		SVEN_CLOSE_ENOUGH = false;
}

//check if camera is too close to an object that will cause the player to lose
void check_lose(glm::vec3 object_pos)
{
	if(glm::length(camera.Position - object_pos) <= 1.3f)
	{
		GAME_LOST = true;
		TORCH_PICKED_UP = false;
		SVEN_PICKED_UP = false;
		std::cout << "Game over. Press R to restart" << std::endl;
	}
}

//check if the player satisfied the objective to win
void check_win()
{
	if(glm::length(camera.Position - WIN_FLOOR_LOCATION) <= 2.5f)
	{
		if(SVEN_PICKED_UP == true)
		{
			GAME_WIN = true;
			TORCH_PICKED_UP = false;
			SVEN_PICKED_UP = false;
			std::cout << "GAME WIN" << std::endl;
		}
	}
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
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement for OS X
	#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CG Assignment", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  	glfwSetCursorPosCallback(window, mouse_callback);
  	glfwSetScrollCallback(window, scroll_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// build and compile our shader zprogram
	// ------------------------------------
	Shader lighting_shader("./game.vs", "./game.fs");
	Shader lamp_shader("./lamp.vs", "./lamp.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	unsigned int VBO_box, VAO_box;

	glGenVertexArrays(1, &VAO_box);
	glGenBuffers(1, &VBO_box);

	glBindVertexArray(VAO_box);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

	//vertex coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal vectors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);



	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int VAO_light;
	glGenVertexArrays(1, &VAO_light);
	glBindVertexArray(VAO_light);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_box);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// load and create a texture
	// -------------------------
	unsigned int tex_wood_diffuse, tex_grass_diffuse, tex_tree_bark_diffuse, tex_tree_leaves_diffuse,
	 			 tex_wool_diffuse, tex_sheep_face_diffuse, tex_sven_body_diffuse, tex_sven_face_diffuse,
				 tex_win_floor_diffuse, tex_lose_screen_diffuse, tex_win_screen_diffuse, tex_spike_diffuse;
	unsigned int tex_wood_specular, tex_grass_specular, tex_tree_bark_specular, tex_tree_leaves_specular,
	     	     tex_wool_specular, tex_sheep_face_specular, tex_sven_body_specular, tex_sven_face_specular,
				 tex_win_floor_specular, tex_lose_screen_specular, tex_win_screen_specular, tex_spike_specular;

	unsigned int tex_red_diffuse, tex_green_diffuse, tex_blue_diffuse;
	unsigned int tex_red_specular, tex_green_specular, tex_blue_specular;

	tex_wood_diffuse = loadTexture(FileSystem::getPath("resources/textures/wood2.jpg").c_str());
	tex_wood_specular = loadTexture(FileSystem::getPath("resources/textures/wood2_specular.jpg").c_str());
	tex_grass_diffuse = loadTexture(FileSystem::getPath("resources/textures/grass.png").c_str());
	tex_grass_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_tree_bark_diffuse = loadTexture(FileSystem::getPath("resources/textures/tree_bark.jpeg").c_str());
	tex_tree_bark_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_tree_leaves_diffuse = loadTexture(FileSystem::getPath("resources/textures/tree_leaf.png").c_str());
	tex_tree_leaves_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_wool_diffuse = loadTexture(FileSystem::getPath("resources/textures/wool.png").c_str());
	tex_wool_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_sheep_face_diffuse = loadTexture(FileSystem::getPath("resources/textures/sheep_face.png").c_str());
	tex_sheep_face_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_sven_body_diffuse = loadTexture(FileSystem::getPath("resources/textures/grey.png").c_str());
	tex_sven_body_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_sven_face_diffuse = loadTexture(FileSystem::getPath("resources/textures/sven_face.png").c_str());
	tex_sven_face_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_win_floor_diffuse = loadTexture(FileSystem::getPath("resources/textures/blue.jpg").c_str());
	tex_win_floor_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_lose_screen_diffuse = loadTexture(FileSystem::getPath("resources/textures/lose_screen.png").c_str());
	tex_lose_screen_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_win_screen_diffuse = loadTexture(FileSystem::getPath("resources/textures/win_screen.png").c_str());
	tex_win_screen_specular = loadTexture(FileSystem::getPath("resources/textures/grass_specular.jpg").c_str());
	tex_spike_diffuse = loadTexture(FileSystem::getPath("resources/textures/metal.png").c_str());
	tex_spike_specular = loadTexture(FileSystem::getPath("resources/textures/marble_specular.jpg").c_str());

	tex_red_diffuse = loadTexture(FileSystem::getPath("resources/textures/red.jpg").c_str());
	tex_red_specular = loadTexture(FileSystem::getPath("resources/textures/red_specular.jpg").c_str());
	tex_green_diffuse = loadTexture(FileSystem::getPath("resources/textures/green.jpg").c_str());
	tex_green_specular = loadTexture(FileSystem::getPath("resources/textures/green_specular.jpg").c_str());
	tex_blue_diffuse = loadTexture(FileSystem::getPath("resources/textures/blue.jpg").c_str());
	tex_blue_specular = loadTexture(FileSystem::getPath("resources/textures/blue_specular.jpg").c_str());




	//shader configuration -------------------------------------------------------------------------------------------
	lighting_shader.use();
	lighting_shader.setInt("material.diffuse", 0);
	lighting_shader.setInt("material.specular", 1);


	float linearArray[] = {0.7f, 0.35f, 0.22f, 0.14f, 0.09f, 0.07f, 0.045f, 0.027f, 0.022f};
	float quadraticArray[] = {1.8f, 0.44f, 0.20f, 0.07f, 0.032f, 0.017f, 0.0075, 0.0028, 0.0019f};


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		delta_time = currentFrame - last_frame;
		last_frame = currentFrame;

		//update delay countdown
		update_delay();
		update_animations();

		// input
		// -----
		process_input(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// activate shader
		lighting_shader.use();

		lighting_shader.setVec3("light.position", torch_position);

		lighting_shader.setVec3("viewPos", camera.Position);

		// light properties
		if(BRIGHT_MODE == false)
		{
			lighting_shader.setVec3("light.ambient", NOT_BRIGHT_MODE_AMBIENT);
		}
		else
		{
			lighting_shader.setVec3("light.ambient", BRIGHT_MODE_AMBIENT);
		}


		lighting_shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
		lighting_shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
    	lighting_shader.setFloat("material.shininess", 65.0f);
		// for now just set the same for every object. But, you can make it dynamic for various objects.

		//attenuation properties obtained from learnopengl's light caster page
		lighting_shader.setFloat("light.constant",  1.0f);
		lighting_shader.setFloat("light.linear", linearArray[brightness_radius_index]);
		lighting_shader.setFloat("light.quadratic", quadraticArray[brightness_radius_index]);

		glm::mat4 projection;
		if(ORTHOGRAPHIC_MODE == true)
		{
			projection = glm::ortho(-5.0f, 5.0f, -4.0f, 4.0f, -100.0f, 100.0f);
		}
		else
		{
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}

		lighting_shader.setMat4("projection", projection);

		if(GAME_LOST == true || GAME_WIN == true)
		{
			camera.reset();
			camera.Position = glm::vec3(24.0f, 0.5, 24.0f);
			camera.Position.y = 0.5f;
			camera.Front = glm::vec3(0.0f, 0.0f, 1.0f);
		}


		// camera/view transformation
		glm::mat4 view = camera.GetViewMatrix();
		lighting_shader.setMat4("view", view);

		//declare transformation matrix
		glm::mat4 model = glm::mat4();
		/*
		//example (remember, it is in column matrix position, so the order is reversed.)
		model = glm::translate(model, glm::vec3(1.0f, 2.0f, 3.0f)); 			// translate by (1.0, 2.0, 3.0)
		model = glm::scale(model, glm::vec3(2.0f, 5.0f, 3.0f)); 			// scale by (2.0, 5.0, 3.0) on X, Y, and Z respectively.
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));	// rotate 45 degree about Y-axis (0,1,0)
		*/


		//Draw objects
		//------------------------------------------------------------------------------------------


		//Coordinate System
		if(SHOW_COORDINATE == true)
		{

			glm::vec3 coord_scales[] = {
				glm::vec3( 100.0f,  0.02f,  0.02f),	//X
				glm::vec3( 0.02f,  100.0f,  0.02f),	//Y
				glm::vec3( 0.02f,  0.02f,  100.0f),	//Z
			};

			glBindVertexArray(VAO_box);
			for(int tab = 0; tab < 3; tab++)
			{
				if(tab == 0) // X
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_red_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_red_specular);
				}
				if(tab == 1) // Y
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_green_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_green_specular);
				}
				if(tab == 2) // Z
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_blue_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_blue_specular);
				}

				model = glm::mat4();
				model = glm::scale(model, coord_scales[tab]);

				lighting_shader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}


		//Grass
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_grass_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_grass_specular);

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, -0.01f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 0.001f, 50.0f));

		lighting_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::vec3 sign_scales[] = {
			glm::vec3( 0.1f,  0.3f,  0.05f),	//stick for the sign
			glm::vec3( 0.9f,  0.7f,  0.03f),//board for the sign
			glm::vec3( 0.8f,  0.6f,  0.01f)	//sign_image
		};

		glm::vec3 sign_positions[] = {
			glm::vec3( 0.0f,  0.15f,  0.0f),//stick for the sign
			glm::vec3( 0.0f,  0.6f,  -0.015f),	//board for the sign
			glm::vec3( 0.0f,  0.61f,  -0.027f),	//sign_image
		};

		glm::vec3 sign_offset = glm::vec3(0.0f, 0.0f, 0.5f);
		glm::vec3 sign_location = camera.Position + camera.Front + sign_offset;
		sign_location.y = 0.0f;

		//render sign if you have lost or won the game
		if(GAME_LOST == true || GAME_WIN == true)
		{
			BRIGHT_MODE = true;
			for(int tab = 0; tab < 3; tab++)
			{
				glBindVertexArray(VAO_box);

				//use wood texture if it isn't the message image
				if(tab != 2)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_wood_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_wood_specular);
				}
				else
				{
					//display win message if win
					if(GAME_WIN == true)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, tex_win_screen_diffuse);
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, tex_win_screen_specular);
					}
					//display lose message if lose
					else
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, tex_lose_screen_diffuse);
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, tex_lose_screen_specular);
					}
				}

				model = glm::mat4();
				model = glm::translate(model, sign_location);
				model = glm::translate(model, sign_positions[tab]);
				model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, sign_scales[tab]);

				lighting_shader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}


		//win floor area
		glBindVertexArray(VAO_box);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_win_floor_diffuse);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex_win_floor_specular);

		model = glm::mat4();
		model = glm::translate(model, WIN_FLOOR_LOCATION);
		model = glm::scale(model, glm::vec3(4.0f, 0.001f, 4.0f));

		lighting_shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Tree (1 bark and 1 leaf block)
		glm::vec3 tree_scales[] = {
			glm::vec3( 2.0f,  2.0f,  2.0f),//leaves
			glm::vec3( 0.5f,  1.5f,  0.5f)//bark
		};
		glm::vec3 tree_part_positions[] = {
			glm::vec3( 0.0f, 1.5f,  0.0f),//leaves
			glm::vec3( 0.0f, 0.0f,  0.0f)//bark
		};

		glBindVertexArray(VAO_box);

		for(int i = -20; i < 20; i += 5)
		{
			for(int j = -20; j < 20; j +=5)
			{
				for(int tab = 0; tab < 2; tab++)
				{
					if(tab == 1)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, tex_tree_bark_diffuse);
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, tex_tree_bark_specular);
					}
					else
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, tex_tree_leaves_diffuse);
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, tex_tree_leaves_specular);
					}
					model = glm::mat4();
					model = glm::translate(model, glm::vec3((float)i, 0.0f, (float)j));
					model = glm::translate(model, tree_part_positions[tab]);
					model = glm::scale(model, tree_scales[tab]);
					model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

					lighting_shader.setMat4("model", model);

					//only draw the trees if the position isnt at spawn
					if(i != 0 || j!=0)
					{
						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
				}
			}
		}

		//Spikes (1 flat box and 9 spikes)
		glm::vec3 spike_scales[] = {
			glm::vec3( 0.5f,  0.001f,  0.5f),//flat box
			glm::vec3( 0.01f,  0.4f,  0.01f),//spikes
		};

		glm::vec3 spike_part_positions[] = {
			glm::vec3( 0.0f, 0.0f, 0.0f),//flat base for the spikes
			glm::vec3( 0.0f, 0.1f, 0.0f),//spike
		};

		//only render and animate the spikes when sven is picked up
		if(SVEN_PICKED_UP == true)
		{
			glBindVertexArray(VAO_box);

			for(float i = -22.5; i <= 22.5; i += 5)
			{
				for(int tab = 0; tab < 2; tab++)
				{
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, tex_spike_diffuse);
					glActiveTexture(GL_TEXTURE1);
					glBindTexture(GL_TEXTURE_2D, tex_spike_specular);

					if(tab == 0)
					{
						model = glm::mat4();
						model = glm::translate(model, spike_part_positions[tab]);
						check_lose(glm::vec3(i, spike_translation_y, 17.0f));
						model = glm::translate(model, glm::vec3(i, 0.0f, 17.0f));
						model = glm::scale(model, spike_scales[tab]);

						lighting_shader.setMat4("model", model);

						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
					else
					{
						for(float j = -0.23f; j <= 0.23f; j += 0.115f)
						{
							for(float k = -0.23f; k <= 0.23f; k += 0.115f)
							{
								model = glm::mat4();
								model = glm::translate(model, glm::vec3(i, spike_translation_y, 17.0f));
								model = glm::translate(model, glm::vec3(j, 0.0f, k));
								model = glm::translate(model, spike_part_positions[tab]);
								model = glm::scale(model, spike_scales[tab]);

								lighting_shader.setMat4("model", model);

								glDrawArrays(GL_TRIANGLES, 0, 36);
							}
						}
					}
				}
			}
		}


		//Sheep (4 legs, 1 head, 1 face, and 1 body)
		glm::vec3 sheep_scales[] = {
			glm::vec3( 0.4f,   0.3f,  0.7f),	//body
			glm::vec3( 0.1f,  -0.45f,  0.1f),	//near left leg
			glm::vec3( 0.1f,  -0.45f,  0.1f),	//near right leg
			glm::vec3( 0.1f,  -0.45f,  0.1f),	//far left leg
			glm::vec3( 0.1f,  -0.45f,  0.1f),	//far right leg
			glm::vec3( 0.2f,   0.2f,  0.2f),	//head
			glm::vec3( 0.15f,  0.15f, 0.001f),	//face
		};
		glm::vec3 sheep_parts_positions[] = {
			glm::vec3(  0.0f,  0.4f, 0.0f),	//body
			glm::vec3( -0.14f, 0.45f, 0.29f),	//near left leg
			glm::vec3(  0.14f, 0.45f, 0.29f),	//near right leg
			glm::vec3( -0.14f, 0.45f, -0.29f),	//far left leg
			glm::vec3(  0.14f, 0.45f, -0.29f),	//far right leg
			glm::vec3(  0.0f, 0.6f, 0.35f),	//head
			glm::vec3(  0.0f, 0.62f, 0.45f)	//face
		};

		glBindVertexArray(VAO_box);

		float facing_angle = 0;
		glm::vec3 temp;

		//make sheep move towards the camera and face the camera
		if(SVEN_PICKED_UP == true)
		{
			sheep_position = moveObjectToCamera(sheep_position);

			facing_angle = 0;
			if(camera.Position.x < sheep_position.x)
			{
				temp =  sheep_position - camera.Position;
				facing_angle = 180;
			}
			else
			{
				temp = camera.Position - sheep_position;
			}
			temp.y = 0;
			facing_angle += (180 * acos(glm::dot(SHEEP_DIRECTION, glm::normalize(temp)))) / PI;
		}

		if(SVEN_PICKED_UP == true)
		{
			check_lose(sheep_position);
		}

		//rendering the sheep
		for(int tab = 0; tab < 7; tab++)
		{
			model = glm::mat4();
			model = glm::translate(model, sheep_position);
			model = glm::rotate(model, glm::radians(facing_angle), glm::vec3(0.0f,1.0f,0.0f));
			model = glm::translate(model, sheep_parts_positions[tab]);
			if(tab != 6)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_wool_diffuse);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tex_wool_specular);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_sheep_face_diffuse);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tex_sheep_face_specular);
			}
			if(tab != 0)
			{
				if(tab == 1 || tab == 4)
				{
					model = glm::rotate(model, glm::radians(leg_angle), glm::vec3(1.0f,0.0f,0.0f));
				}
				else if(tab == 2 || tab == 3)
				{
					model = glm::rotate(model, glm::radians(-leg_angle), glm::vec3(1.0f,0.0f,0.0f));
				}
			}
			model = glm::scale(model, sheep_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			lighting_shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Sven (4 legs, 1 head, 1 face, 1 body, 1 fur coat, 1 tail)
		glm::vec3 sven_scales[] = {
			glm::vec3( 0.25f,   0.15f,  0.4f),	//body
			glm::vec3( 0.27f,   0.2f,  0.17f),	//fur coat
			glm::vec3( 0.05f,  -0.25f,  0.05f),	//near left leg
			glm::vec3( 0.05f,  -0.25f,  0.05f),	//near right leg
			glm::vec3( 0.05f,  -0.25f,  0.05f),	//far left leg
			glm::vec3( 0.05f,  -0.25f,  0.05f),	//far right leg
			glm::vec3( 0.15f,   0.18f,  0.15f),	//head
			glm::vec3( 0.15f,  0.15f, 0.001f),	//face
			glm::vec3( 0.04f,  0.05f,  0.2f)	//tail
		};
		glm::vec3 sven_parts_positions[] = {
			glm::vec3(  0.0f,  0.2f, 0.0f),	    //body
			glm::vec3(  0.0f,  0.18f, 0.17f),   //fur coat
			glm::vec3( -0.1f, 0.25f, 0.14f),	//near left leg
			glm::vec3(  0.1f, 0.25f, 0.14f),	//near right leg
			glm::vec3( -0.1f, 0.25f, -0.14f),	//far left leg
			glm::vec3(  0.1f, 0.25f, -0.14f),   //far right leg
			glm::vec3(  0.0f, 0.18f, 0.27f),    //head
			glm::vec3(  0.0f, 0.18f, 0.35f),    //face
			glm::vec3(  0.0f, 0.28f, -0.3f)	    //tail
		};

		toggle_sven_pickup_distance();

		//update sven's position to be in front of camera if picked up
		if(SVEN_PICKED_UP == true)
		{
			sven_position = camera.Position + camera.Front;
			sven_position.y = 0;
		}

		//rendering sven
		for(int tab = 0; tab < 9; tab++)
		{
			model = glm::mat4();
			model = glm::translate(model, sven_position);
			model = glm::translate(model, sven_parts_positions[tab]);
			if(tab != 7)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_sven_body_diffuse);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tex_sven_body_specular);
			}
			else
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_sven_face_diffuse);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tex_sven_face_specular);
			}
			model = glm::scale(model, sven_scales[tab]);
			model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));

			lighting_shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Torch
		glm::vec3 torch_scales[] = {
			glm::vec3( 0.05f,  0.05f,  0.05f), //tip of torch, light source
			glm::vec3( 0.05f,  0.25f,  0.05f)//body of torch
		};

		glm::vec3 torch_parts_positions[] = {
			glm::vec3( 0.0f,  0.0f,  0.0f),	//tip of torch, light source
			glm::vec3( 0.0f,  -0.15f,  0.0f)//body of torch
		};

		toggle_torch_pickup_distance();

		if(TORCH_PICKED_UP == true)
		{
			torch_position = camera.Position + camera.Front;
			torch_position.y = 0.5f;
		}

		//render your torch
		for(int tab = 0; tab < 2; tab++)
		{
			model = glm::mat4();
			model = glm::translate(model, torch_position);
			model = glm::translate(model, torch_parts_positions[tab]);
			if(TORCH_PICKED_UP == false)
			{
				model = glm::rotate(model, glm::radians(torch_animation_rotation), glm::vec3(0.0f,1.0f,0.0f));
			}
			model = glm::scale(model, torch_scales[tab]);

			if(tab == 0)
			{
				// Draw the light source
				lamp_shader.use();
				lamp_shader.setMat4("projection", projection);
				lamp_shader.setMat4("view", view);
				lamp_shader.setMat4("model", model);

				lamp_shader.setFloat("intensity", 1.0);

				glBindVertexArray(VAO_light);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
			else
			{
				//Draw the body of the torch
				lighting_shader.use();
				glBindVertexArray(VAO_box);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex_wood_diffuse);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, tex_wood_specular);

				lighting_shader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		check_win();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_box);
	glDeleteBuffers(1, &VBO_box);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

//update all animation that is needed
void update_animations()
{
	if(TORCH_PICKED_UP == false)
	{
		update_torch_animation();
	}

	if(SVEN_PICKED_UP == true)
	{
		update_leg_animation();
		update_spike_animation();
	}

	if(JUMPING_MODE == true)
	{
		update_jumping_animation();
	}
}

void update_torch_animation()
{
	if(torch_going_up == true)
	{
		if(torch_position.y < 0.5f)
		{
			torch_position.y = torch_position.y + 0.005;
		}
		else
		{
			torch_going_up = false;
		}
	}
	else
	{
		if(torch_position.y > 0.25f)
		{
			torch_position.y = torch_position.y - 0.005;
		}
		else
		{
			torch_going_up = true;
		}
	}

	torch_animation_rotation += 5.0f;
	if(torch_animation_rotation >= 360.0f)
	{
		torch_animation_rotation = 0;
	}
}

//update leg rotation animation
void update_leg_animation()
{
	if(rotate_forward == true)
	{
		if(leg_angle < 22.5f)
		{
			leg_angle += 2.25f;
		}
		else
		{
			rotate_forward = false;
		}
	}
	else
	{
		if(leg_angle > -22.5f)
		{
			leg_angle -= 2.25f;
		}
		else
		{
			rotate_forward = true;
		}
	}
}


//updates the camera's y coordinate when jumping
void update_jumping_animation()
{
	if(camera_going_up == true)
	{
		if(camera.Position.y <  MAX_JUMP_HEIGHT)
		{
			camera.Position.y += 0.1f;
		}
		else
		{
			camera_going_up = false;
		}
	}
	else
	{
		if(camera.Position.y >  MIN_JUMP_HEIGHT)
		{
			camera.Position.y -= 0.1f;
		}
		else
		{
			camera_going_up = true;
			JUMPING_MODE = false;
		}
	}
}

void update_spike_animation()
{
	if(spike_going_up == true)
	{
		if(spike_translation_y <  MAX_SPIKE_HEIGHT)
		{
			spike_translation_y += 0.01f;
		}
		else
		{
			spike_going_up = false;
		}
	}
	else
	{
		if(spike_translation_y >  MIN_SPIKE_HEIGHT)
		{
			spike_translation_y -= 0.01f;
		}
		else
		{
			spike_going_up = true;
		}
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
		camera.MovementSpeed = SPEED;
	else
		camera.MovementSpeed = SPEED * 2;	// double speed with "Shift" pressed

	//toggle coordinate visibility
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && SHOW_DELAY == 0)
	{
		SHOW_DELAY = 20;
		if(SHOW_COORDINATE == false)
			SHOW_COORDINATE = true;
		else
			SHOW_COORDINATE = false;
	}

	//toggles bright mode
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && BUTTON_DELAY == 0)
	{
		BUTTON_DELAY = 20;
		if(BRIGHT_MODE == false)
			BRIGHT_MODE = true;
		else
			BRIGHT_MODE = false;
	}

	//toggle orthographic mode
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && ORTHO_DELAY == 0)
	{
		ORTHO_DELAY = 20;
		if(ORTHOGRAPHIC_MODE == false)
			ORTHOGRAPHIC_MODE = true;
		else
			ORTHOGRAPHIC_MODE = false;
	}

	//lower brigtness radius
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && BRIGHTNESS_RADIUS_DELAY == 0)
	{
		BRIGHTNESS_RADIUS_DELAY = 20;
		if(brightness_radius_index > MIN_RADIUS)
			brightness_radius_index -= 1;
	}

	//increase brightness radius
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && BRIGHTNESS_RADIUS_DELAY == 0)
	{
		BRIGHTNESS_RADIUS_DELAY = 20;
		if(brightness_radius_index < MAX_RADIUS)
			brightness_radius_index += 1;
	}

	//reset button
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && RESET_DELAY == 0)
	{
		RESET_DELAY = 20;
		sheep_position = SHEEP_START_LOCATION;
		sven_position = SVEN_START_LOCATION;
		torch_position = TORCH_START_LOCATION;
		camera.reset();
		GAME_LOST = false;
		GAME_WIN = false;
		TORCH_PICKED_UP = false;
		SVEN_PICKED_UP = false;
		BRIGHT_MODE = false;
		ORTHOGRAPHIC_MODE = false;
	}

	//only allow movement and pick up when the game is still running
	if(GAME_LOST == false && GAME_WIN == false)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	    camera.ProcessKeyboard(FORWARD, delta_time);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	    camera.ProcessKeyboard(BACKWARD, delta_time);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	    camera.ProcessKeyboard(LEFT, delta_time);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	    camera.ProcessKeyboard(RIGHT, delta_time);


		//pick up torch
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && TORCH_PICK_UP_DELAY == 0 && TORCH_CLOSE_ENOUGH == true)
		{
			TORCH_PICK_UP_DELAY = 20;
			if(TORCH_PICKED_UP == false)
				TORCH_PICKED_UP = true;
			else
				TORCH_PICKED_UP = false;
		}

		//pick up sven
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && SVEN_PICK_UP_DELAY == 0 && SVEN_CLOSE_ENOUGH == true)
		{
			SVEN_PICK_UP_DELAY = 20;
			if(SVEN_PICKED_UP == false)
				SVEN_PICKED_UP = true;
			else
				SVEN_PICKED_UP = false;
		}

		//jump button
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && JUMP_DELAY == 0)
		{
			JUMP_DELAY = 20;
			if(JUMPING_MODE == false)
				JUMPING_MODE = true;
		}
	}
}

//increase/decrease the x and z coordinate of an object's position to be closer
//to the camera
glm::vec3 moveObjectToCamera(glm::vec3 object_pos)
{
	if(camera.Position.x < object_pos.x)
	{
		object_pos.x = object_pos.x - 0.01f;
	}
	else if(camera.Position.x > object_pos.x)
	{
		object_pos.x += 0.01f;
	}

	if(camera.Position.z < object_pos.z)
	{
		object_pos.z -= 0.01f;
	}
	else if(camera.Position.z > object_pos.z)
	{
		object_pos.z += 0.01f;
	}
	return object_pos;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

	//only allow mouse movement when the game hasn't been won or lost yet
	if(GAME_WIN == false  && GAME_LOST == false)
	{
    	camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
