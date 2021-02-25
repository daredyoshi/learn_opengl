
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

    // These coordinates in NDC space will make up the rectabnel that we
    // want to render
   float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
         0.5f, -0.5f, 0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,    // top left 
         // back vertices   
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    }; 
    unsigned int indices[] = {
        0, 1, 2, // first triangle
        2, 3, 0,  // second triangle
        0, 3, 7,
        7, 4, 0, 
        7, 4, 5,
        5, 6, 7, 
        1, 2, 6,
        6, 5, 1,
        1, 0, 4,
        4, 5, 1,
        2, 3, 7,
        7, 6, 2
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
    // so we need to store the data in a vertex array object (VAO)
    unsigned int VAO;
    // Create a VAO, the first parameter is the amount of arrays that will be bound
    glGenVertexArrays(1, &VAO);

    // create our vertex buffer object. This will pass the data to the gpu
    // all objects in openGL are access via an id. 
    unsigned int VBO;
    // create vertex buffers for this id
    glGenBuffers(1, &VBO);

    // bind the vertex array object 
    glBindVertexArray(VAO);

    // to edit this object we need to bind the id to openGL
    // you can bind one object of each type at a time before having to unbind
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // now any buffer calls we make (on the GL_ARRAY_BUFFER target) will
    // be used to configure the currently bound buffer
    // now bind our vertices to the array buffer
    // This function allocates memory and stores the data within the 
    // curerntly bound buffer object
    // There are three ways that this function can treat the data
    //      GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times
    //      GL_STATIC_DRAW: the data is set only once and used many times
    //      GL_DYNAMIC_DRAW: the data is changed a lot and used manytimes
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
   
    // create the element buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // We will now tell OpenGL how it should interpret the vertex data (per attribute)
    //
    //
    // The first param specifies which vertex attribute we want to configure. (location = 0)
    // The next argument specifies the size of the attribute
    // The next is the type of the attribute
    // The next is if we want to force normalize the data. 
    // The next is how large our stride is. This is the space between consecutive vertex attributes
    // (we know that the array is githly packed so this could also have been 0)
    // The last parameter sets the offset of where the data begins in the buffer
    // Now we are using every 6th float
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // 0 is the intex of the vertex attrib array we want to activate
    glEnableVertexAttribArray(0);
    // the colors are offset by 3 floats (void*)(3 * sizeof(floats))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // for the uv coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glEnable(GL_DEPTH_TEST);

    // double textureScale{ 1.0f};
    double textureOpacity{ 1.0f };
    // this will be the transform of object
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

    glm::mat4 view { cam.getViewMatrix() };
    // manually creating a camera here
   
    glm::mat4 projection {cam.getProjectionMatrix()};





    // use the shader once to be able to set uniforms
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setInt("flip", 0);
    shader.setFloat("scale", 1.0f);
    shader.setFloat("opacity", 0.3f);
    shader.setMatrix("model", model);
    shader.setMatrix("view", view);
    shader.setMatrix("projection", projection);

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
        // calculate the new deltatime
        double currentFrame {glfwGetTime()};
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
            textureOpacity+= 0.2;
            shader.setFloat("opacity", textureOpacity); 
        }
        
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
            textureOpacity-= 0.2;
            shader.setFloat("opacity", textureOpacity); 
        }

        // check to see if the user has escaped
        processInput(window, shader);
        // let's test our rendering by clearing the buffer (black)
        // and adding our own color
        // this is a state-setting function for glClear
        glClearColor(0.4, 0.3, 0.3, 1.0);
        // now clear with the color
        // The possible bits we can set are GL_COLOR_BUFFER_BIT
        // GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // glm::vec3 camX = glm::normalize(glm::cross(up, camZ));
        // now get the up vector by crossing the x and z
        // glm::vec3 camY = glm::normalize(glm::cross(camZ, camX));
        

        // the above is equivalent to 
        shader.setMatrix("view", cam.getViewMatrix()); 
        shader.setMatrix("projection", cam.getProjectionMatrix());
        // We can now activate the program object
        // From now one every shader and rendering call will use this program object
        shader.use();

        // Usually when you have multiple objects, you first generate/configure all the 
        // VAOs ( and the required VBO and attribute pointers ) and store those for later use.
        // Once we want to draw one of our objects, we take the corresponding VAO, bind it, and 
        // then draw the object and unbind the VAO again

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickTexture);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, happyTexture);

        for (unsigned int i{0}; i<10; ++i){
            model[3] = cubePositions[i];
            shader.setMatrix("model", model);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 40, GL_UNSIGNED_INT, 0);
        }

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
