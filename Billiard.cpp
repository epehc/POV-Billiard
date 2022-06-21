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

static float xAngle = 0;
static float yAngle = 0;
static float zAngle = 0;
static float radiant = 5.0f;

// field of view
GLfloat Billiard::fov= 45.0;
GLfloat Billiard::cameraZ= 3;

static vec3 cameraPos = vec3(0,0,3);

mat4 Billiard::projectionMatrix, Billiard::viewMatrix, Billiard::modelMatrix(1);



vec4 Billiard::lightPosition= vec4(3.0, 3.0, 3.0, 1.0);

TriangleMesh Billiard::mesh;
TriangleMesh Billiard::mesh2;
glsl::Shader Billiard::diffuseShader;

static bool ani = false;

Billiard::Transformation Billiard::drag;
static mat4 rotationMatrix = mat4(1);
vec3 Billiard::shift = vec3(0); // offset
float Billiard::scaling = 1.0; // scale
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

mesh.load("meshes/pool-table.obj");

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
//MER
void Billiard::display(void){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mat4 modelMatrix = mat4(1);
  modelMatrix = translate(modelMatrix, shift);
  modelMatrix *= rotationMatrix;
  modelMatrix = scale(modelMatrix, vec3(scaling));

  // display back buffer
  window->swapBuffers();
}

void Billiard::reset(void) {

    rotationMatrix = mat4(1); // current rotation of object                                                                                                                                 
    shift = vec3(0, 0, 0); // offset                                                                                                                                                    
    scaling = 1;
}

// keyboard control
//MER
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
    rotationMatrix = glm::rotate(mat4(1), radians(radiant), vec3(1.0, 0.0, 0.0)) * rotationMatrix;
    window->redisplay();
     
    break;

case 'X':
    rotationMatrix = glm::rotate(mat4(1), radians(radiant), vec3(-1.0, 0.0, 0.0)) * rotationMatrix;
    window->redisplay();
    break;

case 'y':
    rotationMatrix = glm::rotate(mat4(1), radians(radiant), vec3(0.0, 1.0, 0.0)) * rotationMatrix;
    window->redisplay();
    break;

case 'Y':
    rotationMatrix = glm::rotate(mat4(1), radians(radiant), vec3(0.0, -1.0, 0.0)) * rotationMatrix;
    window->redisplay();
    break;

case 'z':
    rotationMatrix = glm::rotate(mat4(1), radians(radiant), vec3(0.0, 0.0, 1.0)) * rotationMatrix;
    window->redisplay();
    break;
case 'Z':
    rotationMatrix = glm::rotate(mat4(1), radians(radiant), vec3(0.0, 0.0, -1.0)) * rotationMatrix;
    window->redisplay();
    break;
case 'w':
        modelMatrix *= glm::translate(rotationMatrix, vec3(0,0,1));
        computeViewMatrix();
        window->redisplay();
        break;
case 's':
        modelMatrix *= glm::translate(rotationMatrix, vec3(0,0,-1));
        computeViewMatrix();
        window->redisplay();
        break;
case 'd':
        modelMatrix *= glm::translate(rotationMatrix, vec3(1,0,0));
        computeViewMatrix();
        window->redisplay();
        break;
case 'a':
        modelMatrix *= glm::translate(rotationMatrix,vec3(-1,0,0));
        computeViewMatrix();
        window->redisplay();
        break;

case 'r':
    reset();
    window->redisplay();
    break;


  default:
    break;
  }
}

void Billiard::mousePressed(void) {

    if (keyboard->isActive(Keyboard::SHIFT))
        drag = SHIFT_XY;
    else if (keyboard->isActive(Keyboard::CTRL))
        drag = SHIFT_Z;
    else if (keyboard->isActive(Keyboard::ALT))
        drag = SCALE;
    else
        drag = ROTATE;
}



void Billiard::mouseDragged(void) {

    vec2 v = vec2(mouse->movement) / vec2(window->size());

    switch (drag) {
    case SCALE:
        scaling += 0.1 * length(vec2(mouse->movement));
        scaling = glm::clamp(scaling, 0.01f, 2.0f);
        break;
    case ROTATE:
        if (length(v) == 0) break;
        rotationMatrix =  rotate(mat4(1), radians(180 * length(v)), normalize(vec3(v.y, v.x, 0))) * rotationMatrix; // left multiply
        break;
    case SHIFT_XY:
        shift.x += 3.3 * v.x;
        shift.y -= 3.3 * v.y;
        break;
    case SHIFT_Z:
        shift.z += -10.0 * sign(dot(v, vec2(-1, 1))) * length(v);
        shift.z = clamp(shift.z, -25.0f, 2.0f);
        break;
    default:
        break;
    }
    window->redisplay();
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
