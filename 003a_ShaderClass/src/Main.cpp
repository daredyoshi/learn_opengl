// ALWAYS LOAD GLAD FIRST
#include <glad/glad.h>
#include <boost/dll/runtime_symbol_info.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "ShaderProgram.h"

// these are callback functions that we are binding to glfw
// they are defined after the main function, but declared here so we can bind them
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

std::string CWD{ boost::dll::program_location().parent_path().string() };
std::string VERT_SHADER { "shaders/vert/shader.glsl" };
std::string FRAG_SHADER { "shaders/frag/shader.glsl" };

int main()
{
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
    ShaderProgram shader{ vertShaderPath.c_str(), fragShaderPath.c_str() };

    // These coordinates in NDC space will make up the triangle that we
    // want to render
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };
    // float vertices[] = {
    //     // positions        
    //      0.5f, -0.5f, 0.0f, // bottom right
    //     -0.5f, -0.5f, 0.0f, // bottom left
    //      0.0f,  0.5f, 0.0f, // top 
    // };
    // (void)vertices2;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // 0 is the intex of the vertex attrib array we want to activate
    glEnableVertexAttribArray(0);
    // the colors are offset by 3 floats (void*)(3 * sizeof(floats))
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // we don't want the application to draw a single image and then
    // immediately quit and close the window. 
    // this checks if the user has requested to close the window
    // Each iteration of this loop is called a frame
    while(!glfwWindowShouldClose(window)){
        // check to see if the user has escaped
        processInput(window);
        // let's test our rendering by clearing the buffer (black)
        // and adding our own color
        // this is a state-setting function for glClear
        glClearColor(0.4, 0.3, 0.3, 1.0);
        // now clear with the color
        // The possible bits we can set are GL_COLOR_BUFFER_BIT
        // GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);


        // playing around with some fades
        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        // int vertexColorLocation = glGetUniformLocation(shader.getID(), "vertexColor"); 

        // set the value of the green uniform
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        // We can now activate the program object
        // From now one every shader and rendering call will use this program object
        shader.use();
        // Usually when you have multiple objects, you first generate/configure all the 
        // VAOs ( and the required VBO and attribute pointers ) and store those for later use.
        // Once we want to draw one of our objects, we take the corresponding VAO, bind it, and 
        // then draw the object and unbind the VAO again
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // because GLFW binds to this callback I can't change the inputs.
    // I don't like piling on warnings so I'm putting this here to explicity
    // tell the compiler I'm aware that window isn't use
    (void)window;
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    // the bottom two numbers are where the window is located
    glViewport(0, 0, width, height);
}


