
// ALWAYS LOAD GLAD FIRST
#include <glad/glad.h>

#include <boost/dll/runtime_symbol_info.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#define GLM_SWIZZLE
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/trigonometric.hpp"

#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"

// these are callback functions that we are binding to glfw
// they are defined after the main function, but declared here so we can bind them
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, ShaderProgram &shader);

std::string VERT_SHADER { "shaders/vert/shader.glsl" };
std::string FRAG_SHADER { "shaders/frag/shader.glsl" };
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

    std::string vertShaderPath = (CWD+'/'+ VERT_SHADER);
    std::string fragShaderPath = (CWD+'/'+ FRAG_SHADER);
    std::cout << "Vert shader = " << vertShaderPath << '\n';
    std::cout << "Frag shader = " << fragShaderPath << '\n';
    ShaderProgram shader{ vertShaderPath, fragShaderPath };

    std::string brickTexturePath = (CWD + '/' + BRICK_TEXTURE);
    Texture brickTexture{ brickTexturePath };

    std::string happyTexturePath = (CWD + '/' + HAPPY_TEXTURE);
    Texture happyTexture{ happyTexturePath };

    float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
        -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

        -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
         0.05f, -0.05f,  0.0f, 1.0f, 0.0f,   
         0.05f,  0.05f,  0.0f, 1.0f, 1.0f		    		
    };
    glm::vec2 translations[100];
    int idx = 0;
    float offset = 0.1f;
    for(int y { -10} ;y < 10; y+=2){
        for(int x{-10}; x < 10; x+=2){
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[idx++] = translation;
        }
    }
    // so we need to store the data in a vertex array object (VAO)
    unsigned int VAO;
    // Create a VAO, the first parameter is the amount of arrays that will be bound
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // create our vertex buffer object. This will pass the data to the gpu
    // all objects in openGL are access via an id. 
    unsigned int VBO;
    // create vertex buffers for this id
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    // create a new vertex buffer object for the offsets
    unsigned int instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // this functino tells OpenGL when to update the content
    // of a vertex attribute to the next element of the instance array.
    // By default this is zero which means on each vertex. 
    // But we want it to update on each instance draw 
    // by setting it to a higher numer we would be sayin update on x instance draws. 
    glVertexAttribDivisor(2, 1);
   

    
    glEnable(GL_DEPTH_TEST);

    // this will be the transform of object
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

    // use the shader once to be able to set uniforms
    shader.use();

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
    while(!glfwWindowShouldClose(window)){

        glClearColor(0.4, 0.3, 0.3, 1.0);
        // now clear with the color
        // The possible bits we can set are GL_COLOR_BUFFER_BIT
        // GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // glm::vec3 camX = glm::normalize(glm::cross(up, camZ));
        // now get the up vector by crossing the x and z
        // glm::vec3 camY = glm::normalize(glm::cross(camZ, camX));
        

        shader.use();

        // Usually when you have multiple objects, you first generate/configure all the 
        // VAOs ( and the required VBO and attribute pointers ) and store those for later use.
        // Once we want to draw one of our objects, we take the corresponding VAO, bind it, and 
        // then draw the object and unbind the VAO again

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

        // this will swap the color buffer that is used to render 
        // and show the output on the screen
        glfwSwapBuffers(window);
        // thi checks if any events are triggered (like keyboard/mouse)
        glfwPollEvents();
    }

    // cleanup (optional)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

        
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
