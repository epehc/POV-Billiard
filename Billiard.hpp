#pragma once

/* ----------------------------------------------------------------
   name:           Control.hpp
   purpose:        robot control
   version:	   SKELETON CODE
   TODO:           add more callbacks (see Application.hpp)
   author:         katrin lang
   computer graphics
   tu berlin
   ------------------------------------------------------------- */

#include <vector>
// for pair
#include <utility>

#include "glm/glm.hpp"

#include "glm/gtx/exterior_product.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "Context.hpp"
#include "Application.hpp"

#include "LightSource.h"
#include "Material.h"
#include "TriangleMesh.hpp"
#include "GLSL.hpp"
#include<ctime>

using namespace std;

 OpenGLConfiguration config(glm::uvec2(2, 1),
		      OpenGLConfiguration::Profile::COMPATIBILITY,
		      (Window::DOUBLE | Window::DEPTH | Window::RGB | Window::MULTISAMPLE),
		      8,
		      glm::uvec2(50, 50),
		      glm::uvec2(512, 512),
		      "Computer Graphics - Beleg");


class Billiard : public OpenGLApplication<config>{

public:

	static int size;
  
  // initialization
  static void init();

  // display scene
  static void display(void);
 
  // called after window rehape
  static void reshape();
 
  // called when menu item was selected          
  static void menu(int id);
  
  // keyboard callback
  static void keyPressed();

  // mouse pressed
  static void mousePressed(void);

  //mouse Released
  static void mouseReleased(void);

  // mouse dragged                                                                        
  static void mouseDragged(void);

  // keyboard callback for special keys 
  static void specialKey();

  //static void keyReleased();

  static void idle();
    
  // menu entries
  static std::vector< std::pair< int, std::string > >  menuEntries;
  
  static Window* window;
  static Mouse* mouse;
  static Keyboard* keyboard;

  

private:


	//reset transformations
	static void reset(void);

  static void computeViewMatrix(void);
  static void computeProjectionMatrix(void);

  // field of view (in degrees)                                               
  static GLfloat fov; //field of view
  static glm::vec3 shift; // offset
  static float scaling; // scale
  static float pan; 

	static void setLighting();
	static void setMaterial();

	static void drawPhongShading(void);

	static glsl::Shader phongShader;

  
	// camera position                                                           
	static float cameraZ;

	// near and far plane                                                        
	static float nearPlane, farPlane;

	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 modelMatrix;
	static glm::vec4 lightPosition; 

	static TriangleMesh table;

	static glsl::Shader diffuseShader;

	static LightSource lightSource;
	static Material material;
  

	struct Menu{
	enum Item{QUIT};
	};

  static enum Transformation {
	  SCALE, ROTATE, SHIFT_XY, SHIFT_Z
  } drag;

public: class Ball {

  public:

	  Ball(glm::vec3 pos) {
		  ballPosition = pos;
		  OGballPos = pos;
	  }

	  void load(std::string modelName) {
		  mesh.load(modelName);
	  }

	  glm::mat4 rotateMatrix = glm::mat4(1);
	  glm::vec3 ballPosition;
	  glm::vec3 OGballPos;
	  float velocity;
	  glm::vec2 direction;
	  glm::vec3 axis;

	  TriangleMesh mesh;
	 
	  bool isRolling() {
		  return velocity > 0;
	  }

	  void push(glm::vec2 dir, float v) {
		  velocity = v;
		  direction = dir;
		  axis = glm::vec3(dir.y, 0, -dir.x);

	  }

	  void roll() {
		  
		  if (velocity < 0) {
			  return;
		  }

		  rotateMatrix = glm::rotate(rotateMatrix, glm::radians(1000.0f * velocity), axis);
		  ballPosition += velocity * glm::vec3(direction.x, 0, direction.y);
		  velocity -= 0.0001;

	  }

	  void resetBallPosition() {
		  ballPosition = OGballPos;
	  }

	  void print(glm::mat4 modelMatrix) {
		  
		  modelMatrix = translate(modelMatrix, ballPosition);

		  modelMatrix *= rotateMatrix;

		  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.0275));


		  phongShader.bind();
		  phongShader.setUniform("modelViewProjectionMatrix", projectionMatrix * viewMatrix * modelMatrix);
		  phongShader.setUniform("modelViewMatrix", viewMatrix * modelMatrix);
		  phongShader.setUniform("normalMatrix", glm::mat3(transpose(inverse(viewMatrix * modelMatrix))));
		  phongShader.setUniform("lightSource.position", viewMatrix * lightSource.position);
		  phongShader.setUniform("lightSource.ambient", lightSource.ambient);
		  phongShader.setUniform("lightSource.diffuse", lightSource.diffuse);
		  phongShader.setUniform("lightSource.specular", lightSource.specular);

		  std::vector<TriangleMesh::Group>::iterator group = mesh.begin();
		  while (group != mesh.end()) {
			  std::vector<TriangleMesh::Segment>::iterator segment = group->begin();
			  while (segment != group->end()) {
				  Material material = mesh.getMaterial(segment->material);
				  phongShader.setUniform("material.ambient", glm::vec4(0.1, 0.1, 0.1, 1));
				  phongShader.setUniform("material.diffuse", material.diffuse);
				  phongShader.setUniform("material.specular", material.specular);
				  phongShader.setUniform("material.shininess", material.shininess);
				  segment->draw();
				  segment++;
			  }
			  group++;
		  }
		  
		  phongShader.unbind();

	  }

  };
  
	  static Ball white;
	  static Ball b1;
	  static Ball b2;

	  static Ball balls[];
};
