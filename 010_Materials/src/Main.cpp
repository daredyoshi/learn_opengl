
// ALWAYS LOAD GLAD FIRST
#include <glad/glad.h>

#include <boost/dll/runtime_symbol_info.hpp>
#include <GLFW/glfw3.h>
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

std::string VERT_OBJECT_SHADER { "shaders/vert/geometry.glsl" };
std::string FRAG_OBJECT_SHADER { "shaders/frag/geometry.glsl" };
std::string VERT_LIGHT_SHADER{ "shaders/vert/light.glsl" };
std::string FRAG_LIGHT_SHADER { "shaders/frag/light.glsl" };
std::string BRICK_TEXTURE { "textures/wall.jpg" };
std::string HAPPY_TEXTURE { "textures/awesomeface.png" };

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



    Light light {
        glm::vec3{ 0.2, 0.2, 0.2 },
        glm::vec3{ 0.5, 0.5, 0.5 },
        glm::vec3{ 1.0, 1.0, 1.0 },
        glm::vec3{ 0.0, 5.0, -5.0 }         
    };



    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
                                                             
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
                                                             
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,-1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,-1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0f,  0.0f,  0.0f,
                                                             
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
                                                             
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
                                                             
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
    };

    glm::vec4 cubePositions[] = {
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

    EmeraldMaterial emeraldMat {};
    ObsidianMaterial obsidianMat {};
    JadeMaterial jadeMat {};
    std::array<Material, 3> materials {{emeraldMat, obsidianMat, jadeMat}};
    glEnable(GL_DEPTH_TEST);

    // double textureScale{ 1.0f};
    double textureOpacity{ 1.0f };
    // this will be the transform of object
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 view { cam.getViewMatrix() };
    glm::mat4 projection {cam.getProjectionMatrix()};

    std::string vertShaderPath = (CWD+'/'+ VERT_OBJECT_SHADER);
    std::string fragShaderPath = (CWD+'/'+ FRAG_OBJECT_SHADER);
    std::cout << "Vert shader = " << vertShaderPath << '\n';
    std::cout << "Frag shader = " << fragShaderPath << '\n';

    ShaderProgram geometryShader{ vertShaderPath, fragShaderPath };
    // use the shader once to be able to set uniforms
    geometryShader.use();
    geometryShader.setInt("texture1", 0);
    geometryShader.setInt("texture2", 1);
    geometryShader.setInt("flip", 0);
    geometryShader.setFloat("scale", 1.0f);
    geometryShader.setFloat("opacity", 0.3f);
    geometryShader.setMatrix("model", model);
    geometryShader.setMatrix("view", view);
    geometryShader.setMatrix("projection", projection);

    light.setShaderMaterial(geometryShader);

    std::string brickTexturePath = (CWD + '/' + BRICK_TEXTURE);
    Texture brickTexture{ brickTexturePath };

    std::string happyTexturePath = (CWD + '/' + HAPPY_TEXTURE);
    Texture happyTexture{ happyTexturePath };

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::string vertLightShaderPath = (CWD+'/'+ VERT_LIGHT_SHADER);


    glm::mat4 lightModel {1.0f};


    lightModel = glm::translate(lightModel, light.getPosition());
    // lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    std::string fragLightShaderPath = (CWD+'/'+ FRAG_LIGHT_SHADER);
    std::cout << "Vert Light shader = " << vertLightShaderPath << '\n';
    std::cout << "Frag Light shader = " << fragLightShaderPath << '\n';
    ShaderProgram lightShader{ vertLightShaderPath, fragLightShaderPath};
    lightShader.use();
    lightShader.setMatrix("model", lightModel);
    lightShader.setMatrix("view", view);
    lightShader.setMatrix("projection", projection);

    // Geometry<36> lightBox(lightShader, vertices ); 
    // Box lightBox(lightShader);

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
        glClearColor(0.4, 0.3, 0.3, 1.0);
        // now clear with the color
        // The possible bits we can set are GL_COLOR_BUFFER_BIT
        // GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightShader.use();
        glm::vec3 offsetLightPos{ sin(currentFrame)*0.05, cos(currentFrame)*0.05, 0.0 };
        light.setPosition(light.getPosition() + offsetLightPos);
        lightModel = glm::translate(lightModel, light.getPosition());

        // let the light change colors
        light.setDiffuse(glm::vec3{
                ( sin(currentFrame * 8)*0.5) + 0.5, 
                ( sin(currentFrame * 1)*0.5) + 0.5, 
                ( sin(currentFrame * 5)*0.5) + 0.5, }
                ) ;

        light.setShaderMaterial(lightShader);
        // view/projection transformations
        projection = cam.getProjectionMatrix();
        view = cam.getViewMatrix();
        lightShader.setMatrix("projection", projection);
        lightShader.setMatrix("view", view);
        lightShader.setMatrix("model", lightModel);

        // render the light 
        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, happyTexture);

        // also draw the lamp object
        geometryShader.use();
        geometryShader.setMatrix("projection", projection);
        geometryShader.setMatrix("view", view); 
        // setting the light pos in here in case we decide to animate it
        geometryShader.setVec3("viewPos", cam.getPos());
        light.setShaderMaterial(geometryShader); 
        Material mat = materials[static_cast<int>(totalTime * 0.5) % 3];
        mat.setShaderMaterial(geometryShader);

        // world transformation
        
        // uncomment the below for fun
        // model = glm::mat4(1.0f);
        float rotAmount = 0.01f * deltaTime;
        for (unsigned int i{0}; i<10; ++i){
            model[3] = cubePositions[i];
            
            model = glm::rotate(model, rotAmount*float(i), glm::vec3(0.2f, 0.2f, 0.6f));

            geometryShader.setMatrix("model", model);

            // this could also be calculated on the shader, but that would be slower
            // since it would be done for each vertex and we only need to do it
            // once here
            glm::mat3 normalMatrix{glm::mat3(glm::transpose(glm::inverse(view * model)))};
            geometryShader.setMatrix3("normalMatrix", normalMatrix);
            glBindVertexArray(cubeVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
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

    // if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    //     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    program.use();
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        program.setBool("flip", 1); 
    }
    else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE){
        program.setBool("flip", 0); 
    }

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
