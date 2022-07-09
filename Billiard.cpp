/* ---------------------------------------------------------------
   name:           Beleg.cpp
   purpose:        Beleg Template
   version:	   STARTER CODE
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>

#include "Billiard.hpp"
#include "Context.hpp"
#include "Input.hpp"
#include "Shaders.hpp"
#include "LightSource.h"
#include "Material.h"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// use this with care
// might cause name collisions
using namespace glm;
using namespace std;
using namespace glsl;

static float xAngle = 0;
static float yAngle = 0;
static float zAngle = 0;
static float radiant = 5.0f;
static float radiantY = 270.0f;
static float radiantX = 5.0f;
static float cameraPosY = 1.4348907f;
static float cameraPosZ = 1.4348907f;

// field of view
GLfloat Billiard::fov= 45.0;
GLfloat Billiard::cameraZ= 3;

static clock_t now = clock();

static vec3 cameraPos = vec3(0, cameraPosY, cameraPosZ);

mat4 Billiard::projectionMatrix, Billiard::viewMatrix, Billiard::modelMatrix(1);



vec4 Billiard::lightPosition= vec4(0, 3.0, 3.0, 1.0);

TriangleMesh Billiard::table;

Billiard::Ball Billiard::balls[] = {                                                        Billiard::Ball(vec3(-0.5 , 0.353, 0 )) ,
                                                                                            Billiard::Ball(vec3(0.35, 0.353, 0)), 
                                                                            Billiard::Ball(vec3(0.4, 0.353, -0.025)), Billiard::Ball(vec3(0.4, 0.353, 0.025)),
                                                                Billiard::Ball(vec3(0.45, 0.353, -0.05)), Billiard::Ball(vec3(0.5, 0.353, -0.025)),  Billiard::Ball(vec3(0.45, 0.353, 0.05)),
                                                    Billiard::Ball(vec3(0.5, 0.353, -0.075)), Billiard::Ball(vec3(0.45, 0.353, 0)), Billiard::Ball(vec3(0.5, 0.353, 0.025)), Billiard::Ball(vec3(0.5, 0.353, 0.075)),
                                     Billiard::Ball(vec3(0.55, 0.353, -0.1)), Billiard::Ball(vec3(0.55, 0.353, -0.05)), Billiard::Ball(vec3(0.55, 0.353, 0)), Billiard::Ball(vec3(0.55, 0.353, 0.05)), Billiard::Ball(vec3(0.55, 0.353, 0.1)) };
int Billiard::size = sizeof(balls) / sizeof(balls[0]);


//Billiard::Ball Billiard::white(vec3(-0.5, 0.353, 0));
//Billiard::Ball Billiard::b1(vec3(0.5, 0.353, 0));
//Billiard::Ball Billiard::b2(vec3(0.55, 0.353, 0));

glsl::Shader Billiard::diffuseShader;


Window* Billiard::window;
Mouse* Billiard::mouse;
Keyboard* Billiard::keyboard;

static float lightDistance = 10;

// light source
LightSource Billiard::lightSource{
    // light position in world space
    vec4(3, 8, lightDistance, 1),
    vec4(1, 1, 1, 1),
    vec4(1, 1, 1, 1),
    vec4(1, 1, 1, 1),
};
//Material
//Material Billiard::material{
//    "material",
//    vec4(0.2, 0.2, 0.2, 1),
//    vec4(0.3, 0.3, 0.3, 1),
//    vec4(1, 1, 1, 1),
//    30,
//};


Shader Billiard::phongShader;

static bool ani = false;
static bool rol = false;

Billiard::Transformation Billiard::drag;
static mat4 rotationMatrix = mat4(1);
vec3 Billiard::shift = vec3(0); // offset
float Billiard::scaling = 1.0; // scale
//.....................

//Billiard::Ball Billiard::balls[16];

//......................
float perpDotProduct(vec2 v1, vec2 v2) {
    return v1.x * v2.y - v1.y + v2.x;
}

float dotProduct(vec2 v1, vec2 v2) {
    return v1.x * v2.x + v1.y + v2.y;
}


//bool isProjectedPointOnLineSegment(vec2 p, vec2 v1, vec2 v2) {
//    //get dotProduct |e1| |e2|
//    vec2 e1 = vec2(v2.x - v1.x, v2.y - v1.y);
//    float rectArea = perpDotProduct(e1, e1);
//
//    vec2 e2 = vec2(p.x - v1.x, p.y - v1.y);
//
//    double val = perpDotProduct(e1, e2);
//    return (val > 0 && val < rectArea);
//
//}
//

vec2 getProjectedPointOnLine(vec2 p, vec2 v1, vec2 v2) {
    vec2 e1 = vec2(v2.x - v1.x, v2.y - v1.y);
    vec2 e2 = vec2(p.x - v1.x, p.y - v1.y);

    double dp = dotProduct(e1, e2);

    //get length of vectors
    double lenLineE1 = sqrt(e1.x * e1.x + e1.y * e1.y);
    double lenLineE2 = sqrt(e2.x * e2.x + e2.y * e2.y);
    double cos = dp / (lenLineE1 * lenLineE2);

    // length of v1P'
    double projLenOfLine = cos * lenLineE2;
    vec2 result = vec2((int)(v1.x + (projLenOfLine * e1.x) / lenLineE1),
        (int)(v1.y + (projLenOfLine * e1.y) / lenLineE1));
    cout << "collision" << endl;
    return result;
}

void Billiard::init(){

    //load the table mesh
  table.load("meshes/pool-table.obj"); 

  //load the white ball mesh
  balls[0].load("meshes/white.obj");

  //load every other ball
  for (int i = 1; i < size; i++) {
      cout << i << endl;
      balls[i].load("meshes/" + to_string(i) + ".obj");
  }
  
  //white.load("meshes/white.obj");
  //b1.load("meshes/" + to_string(1) + ".obj");
  //b2.load("meshes/2.obj");


const std::string version= "#version 120\n";


//...............
  phongShader.addVertexShader(version);
  phongShader.addVertexShader(phongVertexShader);
  phongShader.compileVertexShader();
  phongShader.addFragmentShader(version);
  phongShader.loadFragmentShader("LightSource.h");
  phongShader.loadFragmentShader("Material.h");
  phongShader.addFragmentShader(blinnPhongReflectionShader);
  phongShader.addFragmentShader(phongFragmentShader);
  phongShader.compileFragmentShader();
  phongShader.bindVertexAttrib("position", TriangleMesh::attribPosition);
  phongShader.bindVertexAttrib("normal", TriangleMesh::attribNormal);
  phongShader.link();

//.............

  
  // set background color to black
  glClearColor(0.0,0.0,0.0,1.0);
  glPolygonMode(GL_FRONT, GL_FILL);
    
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // enable antialiasing
  glEnable(GL_MULTISAMPLE);

  rotationMatrix = glm::rotate(mat4(1), radians(radiantY), vec3(0.0, -1.0, 0.0)) * rotationMatrix;
  rotationMatrix = glm::rotate(mat4(1), radians(radiantX), vec3(1.0, 0.0, 0.0)) * rotationMatrix;

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
  float nearPlane=cameraZ/25.0f; 
  float farPlane= cameraZ*10.0f ;
  
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

  //render the table mesh using Phong Shaders
  phongShader.bind();
  phongShader.setUniform("modelViewProjectionMatrix", projectionMatrix * viewMatrix * modelMatrix);
  phongShader.setUniform("modelViewMatrix", viewMatrix * modelMatrix);
  phongShader.setUniform("normalMatrix", mat3(transpose(inverse(viewMatrix * modelMatrix))));
  phongShader.setUniform("lightSource.position", viewMatrix * lightSource.position);
  phongShader.setUniform("lightSource.ambient", lightSource.ambient);
  phongShader.setUniform("lightSource.diffuse", lightSource.diffuse);
  phongShader.setUniform("lightSource.specular", lightSource.specular);

  vector<TriangleMesh::Group>::iterator group = table.begin();
  while (group != table.end()) {
      vector<TriangleMesh::Segment>::iterator segment = group->begin();
      while (segment != group->end()) {
          Material material = table.getMaterial(segment->material);
          phongShader.setUniform("material.ambient", vec4(0.1, 0.1, 0.1, 1));
          phongShader.setUniform("material.diffuse", material.diffuse);
          phongShader.setUniform("material.specular", material.specular);
          phongShader.setUniform("material.shininess", material.shininess);
          segment->draw();
          segment++;
      }
      group++;
  }

  phongShader.unbind();


  for (int i = 0; i < size; i++) {
      balls[i].print(modelMatrix);
  }

  //white.print(modelMatrix);
  //b1.print(modelMatrix);
  //b2.print(modelMatrix);

 

//.............

 //drawParameters();

//.............
  

  // display back buffer
  window->swapBuffers();
}

void Billiard::reset(void) {

    rotationMatrix = mat4(1); // current rotation of object                                                                                                                                 
    shift = vec3(0, 0, 0); // offset                                                                                                                                                    
    scaling = 1;

    for (int i = 0; i < size; i++) {
        balls[i].resetBallPosition();
    }
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

//case 'w':
//        modelMatrix *= glm::translate(rotationMatrix, vec3(0,0,1));
//        computeViewMatrix();
//        window->redisplay();
//        break;
//case 's':
//        modelMatrix *= glm::translate(rotationMatrix, vec3(0,0,-1));
//        computeViewMatrix();
//        window->redisplay();
//        break;
//case 'd':
//        modelMatrix *= glm::translate(rotationMatrix, vec3(1,0,0));
//        computeViewMatrix();
//        window->redisplay();
//        break;
//case 'a':
//        modelMatrix *= glm::translate(rotationMatrix,vec3(-1,0,0));
//        computeViewMatrix();
//        window->redisplay();
//        break;

case 'r':
    reset();
    window->redisplay();
    break;


  default:
    break;
  }
}

void Billiard::mouseReleased(void) {

    if (keyboard->isActive(Keyboard::SHIFT)) {
        //cout << " released" << endl;
        
        clock() - now;
        long click = clock() - now;
        float erg = (click * 0.0000234);
        cout << erg << endl;
        vec3 temp = vec3(balls[0].ballPosition - vec3(rotationMatrix * vec4(cameraPos, 1)));
        balls[0].push(vec2(-temp.x, temp.z), erg);
        window->redisplay();
    }

}

void Billiard::mousePressed(void) {

    now = clock();

    if (keyboard->isActive(Keyboard::CTRL))
        drag = SHIFT_Z;
    else if (keyboard->isActive(Keyboard::ALT))
        drag = SCALE;
    else
        drag = ROTATE;
}




void Billiard::mouseDragged(void) {

    vec2 v = vec2(mouse->movement) / vec2(window->size());
    vec2 pos = vec2(mouse->position);

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
        //std::cout << pos.x << " " << pos.y << endl;
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


void Billiard::idle() {
    static int time = INT_MAX;

    if (ani == true) {
        modelMatrix *= glm::rotate(mat4(1), radians(10.0f), vec3(0, 1, 0));
        window->redisplay();
    }


    if (time++ > 25) {
        ani = false;
        time = 0;
    }

    if (balls[0].isRolling()) {
        
        balls[0].roll();
        //getProjectedPointOnLine(vec2 p, vec2 v1, vec2 v2)
        getProjectedPointOnLine(vec2(balls[1].ballPosition.x, balls[1].ballPosition.z), vec2(balls[0].ballPosition.x, balls[0].ballPosition.z), vec2(balls[0].ballPosition.x + 0.5, balls[1].ballPosition.z + 0.5));
        window->redisplay();
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
