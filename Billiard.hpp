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

#include "Context.hpp"
#include "Application.hpp"

#include "LightSource.h"
#include "Material.h"
#include "TriangleMesh.hpp"
#include "GLSL.hpp"

 OpenGLConfiguration config(glm::uvec2(2, 1),
		      OpenGLConfiguration::Profile::COMPATIBILITY,
		      (Window::DOUBLE | Window::DEPTH | Window::RGB | Window::MULTISAMPLE),
		      8,
		      glm::uvec2(50, 50),
		      glm::uvec2(512, 512),
		      "Computer Graphics - Beleg");


class Billiard : public OpenGLApplication<config>{

public:

  
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

  // keyboard callback for special keys 
  static void specialKey();

  static void idle();
    
  // menu entries
  static std::vector< std::pair< int, std::string > >  menuEntries;
  
  static Window* window;
  static Mouse* mouse;
  static Keyboard* keyboard;



private:

	static void computeViewMatrix(void);
	static void computeProjectionMatrix(void);

	static void setLighting();
	static void setMaterial();

	static void drawPhongShading(void);

	static glsl::Shader phongShader;

	// field of view (in degrees)                                               
	static GLfloat fov;

	static float pan;
  
	// camera position                                                           
	static float cameraZ;

	// near and far plane                                                        
	static float nearPlane, farPlane;

	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 modelMatrix;
	static glm::vec4 lightPosition; 

	static TriangleMesh mesh;
	static TriangleMesh mesh2;
	static glsl::Shader diffuseShader;

	static LightSource lightSource;
	/*static Material material;*/
  
	struct Menu{
	enum Item{QUIT};
	};

};
