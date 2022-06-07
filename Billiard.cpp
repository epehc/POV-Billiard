/* ---------------------------------------------------------------
   name:           Beleg.cpp
   purpose:        Beleg Template
   version:	   STARTER CODE
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream> 

#include "Billiard.hpp"
#include "Context.hpp"
#include "Input.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;


// field of view
GLfloat Billiard::fov= 45.0;
GLfloat Billiard::cameraZ= 3;

static vec3 cameraPos = vec3(0,0,3);

mat4 Billiard::projectionMatrix, Billiard::viewMatrix, Billiard::modelMatrix(1);

static mat4 rotationMatrix = mat4(2);

vec4 Billiard::lightPosition= vec4(3.0, 3.0, 3.0, 1.0);

TriangleMesh Billiard::mesh;
TriangleMesh Billiard::mesh2;
glsl::Shader Billiard::diffuseShader;

static bool ani = false;
//.....................


//......................


LightSource Billiard::lightSource={
    // position

  glm::vec4(0, 2, 3, 1),
  // ambient color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // diffuse color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // specular color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
};

void Billiard::init(){

mesh.load("meshes/cube.obj");
mesh2.load("meshes/teapot.obj");

const std::string version= "#version 120\n";


diffuseShader.addVertexShader(version);
  diffuseShader.loadVertexShader("shaders/diffuse.vert");
  diffuseShader.compileVertexShader();
  diffuseShader.addFragmentShader(version);
  diffuseShader.loadFragmentShader("shaders/diffuse.frag");
  diffuseShader.compileFragmentShader();
  diffuseShader.bindVertexAttrib("position", TriangleMesh::attribPosition);
  diffuseShader.bindVertexAttrib("normal", TriangleMesh::attribNormal);
  diffuseShader.link();


//...............


//.............

  
  // set background color to black
  glClearColor(0.0,0.0,0.0,1.0);
  glPolygonMode(GL_FRONT, GL_FILL);
    
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // enable antialiasing
  glEnable(GL_MULTISAMPLE);
}

// adjust to new window size
void Billiard::reshape(){
    
  // viewport
  glViewport(0, 0, (GLsizei) window->width(), (GLsizei) window->height());

  computeViewMatrix();
  computeProjectionMatrix();
}

void Billiard::computeViewMatrix(void){


  viewMatrix= glm::lookAt(cameraPos, vec3(0), vec3(0,1,0));
}

void Billiard::computeProjectionMatrix(void){

  // reshaped window aspect ratio
  float aspect= (float) window->width() / (float) window->height();
  
  // compute near and far plane
  float nearPlane=cameraZ/10.0f; 
  float farPlane= cameraZ*10.0f;
  
  projectionMatrix= glm::perspective(radians(fov), aspect, nearPlane, farPlane);
}

// this is where the drawing happens

void Billiard::display(void){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 
 diffuseShader.bind();
 diffuseShader.setUniform("transformation", projectionMatrix*viewMatrix*modelMatrix);
 diffuseShader.setUniform("color", vec3(1,1,1));
 diffuseShader.setUniform("lightPosition", inverse(modelMatrix)*lightPosition);
 mesh.draw();
 diffuseShader.unbind();

mat4 modelMatrix = glm::translate(mat4(1),vec3(1,0,0));
 diffuseShader.bind();
 diffuseShader.setUniform("transformation", projectionMatrix*viewMatrix*modelMatrix);
 diffuseShader.setUniform("color", vec3(122,1,1));
 diffuseShader.setUniform("lightPosition", inverse(modelMatrix)*lightPosition);
 mesh2.draw();
 diffuseShader.unbind();
  


//.............



//.............
  
  // display back buffer
  window->swapBuffers();
}

// keyboard control
void Billiard::keyPressed(){
  
  // rotate selected node around 
  // x,y and z axes with keypresses

  switch(keyboard->key){
    
 case 'q':
 case 'Q':
   exit(0);

 case 'c':
    cameraPos *= 0.9;
computeViewMatrix();
window->redisplay();
break;

case 'v':
 cameraPos *= 1.1;
computeViewMatrix();

    window->redisplay();
break;

case 'm':
ani = true;

break;
case 'x':

       cameraPos = mat3( glm::rotate(mat4(1),radians(10.0f),vec3(1,0,0)) ) * cameraPos;

        computeViewMatrix();
        window->redisplay();
    
    break;
case 'y':

       cameraPos = mat3( glm::rotate(mat4(1),radians(10.0f),vec3(0,1,0)) ) * cameraPos;

        computeViewMatrix();
        window->redisplay();
    
    break;
case 'w':
        modelMatrix *= glm::translate(rotationMatrix,vec3(-1,0,0));
        computeViewMatrix();
        window->redisplay();
break;
case 's':
        modelMatrix *= glm::translate(rotationMatrix,vec3(1,0,0));
        computeViewMatrix();
        window->redisplay();
break;
case 'd':
        modelMatrix *= glm::translate(rotationMatrix,vec3(0,0,1));
        computeViewMatrix();
        window->redisplay();
break;
case 'a':
        modelMatrix *= glm::translate(rotationMatrix,vec3(0,0,-1));
        computeViewMatrix();
        window->redisplay();
break;


  default:
    break;
  }
}



void Billiard::idle(){

static int time= INT_MAX;

if(ani == true ) {
modelMatrix *= glm::rotate(mat4(1),radians(10.0f),vec3(0,1,0)) ;
window->redisplay();
}


if(time++ > 25){
ani = false;
time =0;
}



}

// keyboard callback for special keys
// (arrow keys for node selection)
void Billiard::specialKey(){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(keyboard->code) {

  case Keyboard::Code::UP:

    
    break;
  case Keyboard::Code::DOWN:
    
    break;
  case Keyboard::Code::LEFT:
    
    break;
  case Keyboard::Code::RIGHT:
    
    break;
  default:
    break;
  }
}

vector< pair < int, string > > Billiard::menuEntries{{Menu::QUIT, "quit"}};

// mouse menu control
void Billiard::menu(int id){
  
  switch (id) {
  case Menu::QUIT: 
    exit(0);

  default:
    break;
  }
}

int main(int argc, char** argv){

  // initialize OpenGL context
  OpenGLContext<Billiard>::init(&argc, argv);

  // some diagnostic output
  std::cout << "GPU: " << glGetString(GL_RENDERER) << ", OpenGL version: " << glGetString(GL_VERSION) << ", GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
  //start event loop
  InputManager<Billiard>::startEventLoop();
  
  return 0;
}
