#pragma once
// Some Code samples from learnopengl.com , modified by me.
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include "MATH_MACRO_IMPLEMENTATION.h"
#include <deque>



// build and compile our shader zprogram
// ------------------------------------

GLFWwindow* window;
// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;	
float lastFrame = 0.0f;
bool shoot_cooldown = true;
bool Shooting = false;
bool ADS = false;
bool should_jump = false;
bool display_gun = true;
bool display_deagle = false;
unsigned int bullets_shot = 0;
bool ShootingFinished = true;
// save Last Position where the camera didint collide anything
vec3 saveLastPostion = camera.Position;
vec3 bulletDir = camera.Front;
vec3 bulletPos = saveLastPostion;
std::deque<vec3> bulletPositions;
std::deque<vec3> bulletFronts;
std::deque<Quaternion> bulletQua;
// Automatic gun boolean , means if the gun should move Back or Forth>..XD
bool should_move = true;
// if gravity is enabled
bool should_fall = true;
// jump_cooldown so you cant fly;;;;
float jump_cooldown = 0.0f;

//--------------------------
double _ShootingDelay = 0;
int _AnimationIndex = 0;
bool should_deagle_shoot = true;
double time_passed = 0;
