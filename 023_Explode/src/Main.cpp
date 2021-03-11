
// ALWAYS LOAD GLAD FIRST
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

std::string const VERT_OBJECT_SHADER { "shaders/vert/main.glsl" };
std::string const GEO_OBJECT_SHADER { "shaders/geo/main.glsl" };
std::string const FRAG_OBJECT_SHADER { "shaders/frag/main.glsl" };

std::string const OBJ_FILE{ "obj/backpack/backpack.obj" };

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
    // std::string name = aux.substr(pos+1);


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


    Model obj { CWD + '/' + OBJ_FILE };

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
        glm::vec3{0.2},
        // spec
        glm::vec3{ 0.0},
        //direction
        glm::normalize(glm::vec3{ 0.0, -0.8, -0.2 })
    };

    glm::vec4 objectPositions[] = {
        glm::vec4( 0.0f,  0.0f,  0.0f , 1.0f), 
        glm::vec4( 2.0f,  5.0f, -15.0f, 1.0f), 
        glm::vec4(-1.5f, -2.2f, -2.5f , 1.0f),  
        glm::vec4(-3.8f, -2.0f, -12.3f, 1.0f),  
        glm::vec4( 2.4f, -0.4f, -3.5f , 1.0f),  
        glm::vec4(-1.7f,  3.0f, -7.5f , 1.0f),  
        glm::vec4( 1.3f, -2.0f, -2.5f , 1.0f),  
        glm::vec4( 1.5f,  2.0f, -2.5f , 1.0f), 
        glm::vec4( 1.5f,  0.2f, -1.5f , 1.0f), 
        glm::vec4(-1.3f,  1.0f, -1.5f , 1.0f)  
    }; 

    std::vector<glm::vec3> pointLightPositions = {
        {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
        }
    };  

    std::vector<PointLight> pointLights {};
    unsigned int count{0};
    for (glm::vec3 pointPos : pointLightPositions){
        pointLights.push_back(
                PointLight{
                    // name
                    "pointLights[" + std::to_string(count) + ']',
                    // ambient
                    glm::vec3{0.1},
                    // diffuse
                    glm::vec3{0.5},
                    // spec
                    glm::vec3{ 0.5},
                    // position 
                    pointPos, 
                    // linear
                    0.06f, 
                    // quadratic
                    0.0175
                }
        );
        ++count;
    }
    

    EmeraldMaterial emeraldMat {"material"};
    ObsidianMaterial obsidianMat {"material"};
    JadeMaterial jadeMat {"material"};
    std::array<Material, 3> materials {{jadeMat, emeraldMat, obsidianMat}};
    glEnable(GL_DEPTH_TEST);
    

    // double textureScale{ 1.0f};
    double textureOpacity{ 1.0f };
    // this will be the transform of object
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(glm::vec3(0.3f));
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 view { cam.getViewMatrix() };
    glm::mat4 projection {cam.getProjectionMatrix()};

    std::string vertShaderPath = (CWD+'/'+ VERT_OBJECT_SHADER);
    std::string geoShaderPath = (CWD+'/'+ GEO_OBJECT_SHADER);
    std::string fragShaderPath = (CWD+'/'+ FRAG_OBJECT_SHADER);
    std::cout << "Vert shader = " << vertShaderPath << '\n';
    std::cout << "Frag shader = " << fragShaderPath << '\n';

    ShaderProgram geometryShader{ vertShaderPath, geoShaderPath, fragShaderPath };
    // use the shader once to be able to set uniforms
    geometryShader.use();
    geometryShader.setInt("flip", 0);
    geometryShader.setFloat("scale", 1.0f);
    geometryShader.setFloat("opacity", 0.3f);
    geometryShader.setMatrix("model", model);
    geometryShader.setMatrix("view", view);
    geometryShader.setMatrix("projection", projection);

    directionalLight.setShaderMaterial(geometryShader);
    spotLight.setShaderMaterial(geometryShader);
    for (PointLight &pointLight : pointLights){
        pointLight.setShaderMaterial(geometryShader);
    }

    
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
        
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            textureOpacity+= 0.2;
            geometryShader.setFloat("opacity", textureOpacity); 
        }
        
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            textureOpacity-= 0.2;
            geometryShader.setFloat("opacity", textureOpacity); 
        }

        // check to see if the user has escaped
        processInput(window, geometryShader);
        // let's test our rendering by clearing the buffer (black)
        // and adding our own color
        // this is a state-setting function for glClear
        glClearColor(0.8, 0.2, 0.2, 1.0);
        // now clear with the color
        // The possible bits we can set are GL_COLOR_BUFFER_BIT
        // GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




        count = 0;
        for( PointLight &light : pointLights ){

            glm::vec3 offsetLightPos{ sin(currentFrame)*0.05, cos(currentFrame)*0.05, 0.0 };
            light.setPosition(light.getPosition() + offsetLightPos);

            // let the light change colors
            light.setDiffuse(glm::vec3{
                    ( sin(+ currentFrame * 8 + count)*0.5) + 1.5 + count, 
                    ( sin(currentFrame * 1 + count)*0.5) + 1.5 - count * -2, 
                    ( sin(currentFrame * 5 + count)*0.5) + 1.5 + count + 1, }
                    ) ;
        }
        

        // also draw the lamp object
        geometryShader.use();
        geometryShader.setMatrix("projection", cam.getProjectionMatrix());
        geometryShader.setMatrix("view", cam.getViewMatrix()); 
        // setting the light pos in here in case we decide to animate it
        geometryShader.setVec3("viewPos", cam.getPos());
        geometryShader.setFloat("time", currentFrame*0.05);

        for (PointLight light : pointLights){
            light.setShaderMaterial(geometryShader);
        }
        directionalLight.setShaderMaterial(geometryShader);

        spotLight.setPosition(cam.getPos());
        spotLight.setDirection(cam.getDir());
        spotLight.setShaderMaterial(geometryShader);

        float rotAmount = 0.01f * deltaTime;
        for (unsigned int i{0}; i<10; ++i){

            model = glm::rotate(model, rotAmount*float(i), glm::vec3(0.2f, 0.2f, 0.6f));
            model[3] = objectPositions[i];
           
            geometryShader.setMatrix("model", model);

            // this could also be calculated on the shader, but that would be slower
            // since it would be done for each vertex and we only need to do it
            // once here
            glm::mat3 normalMatrix{glm::mat3(glm::transpose(glm::inverse(model)))};
            geometryShader.setMatrix3("normalMatrix", normalMatrix);

            obj.draw(geometryShader);
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
