
// ALWAYS LOAD GLAD FIRST
#include <cstdlib>
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#define GLM_SWIZZLE
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/trigonometric.hpp"

#include "ShaderProgram.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"
#include "Materials.h"
#include "Light.h"




// these are callback functions that we are binding to glfw
// they are defined after the main function, but declared here so we can bind them
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, ShaderProgram &shader);

std::string const VERT_OBJECT_SHADER { "shaders/vert/geometry.glsl" };
std::string const FRAG_OBJECT_SHADER { "shaders/frag/geometry.glsl" };
std::string const VERT_INSTANCE_SHADER{ "shaders/vert/instancing.glsl" };

std::string const PLANET_OBJ_FILE{ "obj/planet/planet.obj" };
std::string const ASTERIOD_OBJ_FILE{ "obj/rock/rock.obj" };
std::string const ASTEROID_TEX_FILE{ "obj/rock/rock.png" };

double deltaTime = 0.0f;
double lastFrame = 0.0f;

float lastX { 400 };
float lastY { 300 };

Camera cam{};
int main(int numArgs, char *args[])
{
    (void)numArgs; 
        // Get the last position of '/'
    std::string aux(args[0]);
    // we aren't using numArgs

    // get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
    int pos = aux.rfind('\\');
#else
    int pos = aux.rfind('/');
#endif

    // Get the path and the name
    std::string CWD = aux.substr(0,pos+1);


    glfwInit();
    // Set glfw to use open gl version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create our window object 
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if ( window == NULL ){
        std::cout << "Failed to create GLFW window" << '\n';
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //the moment a user resizes the window the viewport should be 
//adjusted as well this is a callback to do that
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // initilize glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << '\n';
        return -1;
    }


    Model planetObj { CWD + '/' + PLANET_OBJ_FILE};
    Model asteroidObj { CWD + '/' + ASTERIOD_OBJ_FILE};
    // for some reason the asteroid material does not load correctly so manually load it
    asteroidObj.getMeshes()[0].getMat().setDiffuseTex1(Texture{ CWD + '/' + ASTEROID_TEX_FILE });

    // generate transforms for all the asteroids
    unsigned int amount = 80000;
    glm::mat4 *modelMatrices;
    modelMatrices = new glm::mat4[amount];
    std::srand(glfwGetTime());
    float radius = 50.0;
    float offset = 18.5f;
    float minScale = 0.05f;
    float maxScale = 100.0f;
    for(unsigned int i{0}; i<amount; ++i){
        glm::mat4 model = glm::mat4(1.0f);
        // translation: displace along circle with radiu in rang [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        // add some random displacement along the angle in x
        float displacement = (rand() & (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        // in y
        displacement = (rand() & (int)(2 * offset * 100)) / 100.0f - offset;
        // keep the height of the field smaller compared to x and y to build a ring
        float y = displacement * 0.2f;
        // in z 
        displacement = (rand() & (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // scale: between min scale and max scale
        float scale = (rand() % 20) / maxScale + minScale;
        model = glm::scale(model, glm::vec3(scale));
        
        // rotation: rotate about a random axis by random amount
        float rotX {  ( rand() % 100 / 50.0f  ) - 1.0f};
        float rotY {  ( rand() % 100 / 50.0f  ) - 1.0f};
        float rotZ {  ( rand() % 100 / 50.0f  ) - 1.0f};
        glm::vec3 rotDir { rotX, rotY, rotZ};
        float rotAngle ( rand() % 360 );
        model = glm::rotate(model, rotAngle, rotDir);
        modelMatrices[i] = model;
    }
    // create a buffer for the matrices of asteroid instances 
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
    for (Mesh m : asteroidObj.getMeshes()){
        glBindVertexArray(m.getVAO());
        // vertex attributes
        // because the maximum amount of data for a vertex attribute is a vec4, we need to make 4 vec4s
        std::size_t vec4Size = sizeof(glm::vec4);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    SpotLight spotLight {
        "spotLight",
        // ambient
        glm::vec3{0.2},
        // diffuseCan be 0, in which case the mBones array is NULL.
        glm::vec3{0.8},
        // spec
        glm::vec3{ 1.0},
        // position 
        glm::vec3{ 2.0, 1.0, 1.0}, 
        // direction
        cam.getDir(),
        // linear
        0.06f, 
        // quadratic
        0.0175,
        // cutoff
        10.0f,
        // outer cutoff
        12.0f
    };

    DirectionalLight directionalLight { 
        "directionLight",
        // ambient
        glm::vec3{0.2},
        // diffuse
        glm::vec3{1.0},
        // spec
        glm::vec3{ 1.0},
        //direction
        glm::normalize(glm::vec3{ 0.4, -0.2, 0.4 })
    };


    glEnable(GL_DEPTH_TEST);
    

    // this will be the transform of the planet 
    glm::mat4 planetModel= glm::mat4(1.0f);
    planetModel = glm::translate(planetModel, glm::vec3(0.0f, -3.0f, 0.0f));
    planetModel = glm::scale(planetModel, glm::vec3(4.0f, 4.0f, 4.0f));

    glm::mat4 view { cam.getViewMatrix() };
    glm::mat4 projection {cam.getProjectionMatrix()};

    std::string vertShaderPath = (CWD+'/'+ VERT_OBJECT_SHADER);
    std::string vertInstanceShaderPath = (CWD+'/'+ VERT_INSTANCE_SHADER);
    std::string fragShaderPath = (CWD+'/'+ FRAG_OBJECT_SHADER);

    ShaderProgram geometryShader{ vertShaderPath, fragShaderPath };
    ShaderProgram instanceShader{ vertInstanceShaderPath, fragShaderPath };
    // use the shader once to be able to set uniforms
    geometryShader.use();
    geometryShader.setMatrix("model", planetModel);
    geometryShader.setMatrix("view", view);
    geometryShader.setMatrix("projection", projection);

    directionalLight.setShaderMaterial(geometryShader);
    spotLight.setShaderMaterial(geometryShader);
    planetObj.draw(geometryShader);
    instanceShader.use();
    instanceShader.setMatrix("model", planetModel);
    instanceShader.setMatrix("view", view);
    instanceShader.setMatrix("projection", projection);
    asteroidObj.draw(instanceShader);

    // this makes the mouse input captuerd but not visible
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // tell GLFW to listen to mouse-movement events
    glfwSetCursorPosCallback(window, mouse_callback);
    // tell GLFW to listen to scroll
    glfwSetScrollCallback(window, scroll_callback);


    // we don't want the application to draw a single image and then
    // immediately quit and close the window. 
    // this checks if the user has requested to close the window
    // Each iteration of this loop is called a frame
    double totalTime{0.0};
    while(!glfwWindowShouldClose(window)){
        // calculate the new deltatime
        double currentFrame {glfwGetTime()};
        deltaTime = currentFrame - lastFrame;
        totalTime += deltaTime;
        if ( totalTime > 1000)
            totalTime = 0;
        lastFrame = currentFrame;
        

        // check to see if the user has escaped
        processInput(window, geometryShader);
        // let's test our rendering by clearing the buffer (black)
        // and adding our own color
        // this is a state-setting function for glClear
        glClearColor(0.0, 0.0, 0.0, 1.0);
        // now clear with the color
        // The possible bits we can set are GL_COLOR_BUFFER_BIT
        // GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // draw the planet object
        geometryShader.use();
        directionalLight.setShaderMaterial(geometryShader);
        spotLight.setPosition(cam.getPos());
        spotLight.setDirection(cam.getDir());
        spotLight.setShaderMaterial(geometryShader);
        geometryShader.setMatrix("projection", cam.getProjectionMatrix());
        geometryShader.setMatrix("view", cam.getViewMatrix()); 
        // setting the planet pos in here in case we decide to animate it
        geometryShader.setVec3("viewPos", cam.getPos());
        geometryShader.setMatrix("model", planetModel);
        planetObj.draw(geometryShader);

        // draw the asteroids as instances
        instanceShader.use();
        directionalLight.setShaderMaterial(instanceShader);
        spotLight.setPosition(cam.getPos());
        spotLight.setDirection(cam.getDir());
        spotLight.setShaderMaterial(instanceShader);
        instanceShader.setMatrix("projection", cam.getProjectionMatrix());
        instanceShader.setMatrix("view", cam.getViewMatrix()); 
        
        for(Mesh m : asteroidObj.getMeshes()){
            glBindVertexArray(m.getVAO()); 
            m.getMat().setShaderMaterial(instanceShader);
            m.getMat().setShaderTextures();
            glDrawElementsInstanced(GL_TRIANGLES, m.getIndices().size(), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }

        // this will swap the color buffer that is used to render 
        // and show the output on the screen
        glfwSwapBuffers(window);
        // thi checks if any events are triggered (like keyboard/mouse)
        glfwPollEvents();
    }
    // clean up/delete GLFW's resources that were allocated
    glfwTerminate();
    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, ShaderProgram &program)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    program.use();

    // navigate with esdf
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.incrementPosition(Camera::CameraDirection::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.incrementPosition(Camera::CameraDirection::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.incrementPosition(Camera::CameraDirection::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        cam.incrementPosition(Camera::CameraDirection::RIGHT, deltaTime);
   
    // navigate up and down with wr
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.incrementPosition(Camera::CameraDirection::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        cam.incrementPosition(Camera::CameraDirection::UP, deltaTime);
   
    // you can look around with ijkl
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        cam.incrementDirection(Camera::CameraDirection::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        cam.incrementDirection(Camera::CameraDirection::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) 
        cam.incrementDirection(Camera::CameraDirection::LEFT, deltaTime); 
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) 
        cam.incrementDirection(Camera::CameraDirection::RIGHT, deltaTime);
    // roll with uo
    
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        cam.incrementDirection(Camera::CameraDirection::LEFT_ROLL, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        cam.incrementDirection(Camera::CameraDirection::RIGHT_ROLL, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // because GLFW binds to this callback I can't change the inputs.
    // I don't like piling on warnings so I'm putting this here to explicity
    // tell the compiler I'm aware that window isn't use
    (void)window;
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    // the bottom two numbers are where the window is located
    glViewport(0, 0, width, height);
    cam.setSize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    (void)window;
    cam.setDirection(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    (void)window;
    (void)xoffset;
    cam.changeFov(yoffset);        
}
