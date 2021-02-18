// ALWAYS LOAD GLAD FIRST
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// these are callback functions that we are binding to glfw
// they are defined after the main function, but declared here so we can bind them
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// the syntax of GLSL is very similar to C
// usually the input data for a shader is not already in NDC, so
// the shader will need to do a bit more work than it's doing here.
// Our shader is verrrry simple though, and is just forwarding the position
// of our vertices which we are conveniently giving in NDC.

// We are declaring all the input vertex attributes with the in keyword.
// the default vector datatype has 4 floats, so we are using a vec3
// for the position.

// to set the output of the vertex shader we need to assign a vector4 value
// to each vertex to a predefined variable called gl_Position. 
// gl_position will be used as the output of the vertex shader. 

// for simplicity we are not using a seperate shader file but just
// adding this as a string array.
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// The fragment shader only requires one output variable similar
// to the gl_Position for the vertx shader. In this case it's named
// more nicely as FragColor. 
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
    "    FragColor = vec4(1.0f, 0.8f, 0.2f, 1.0f);\n"
    "}\0";

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

    // for OpenGL to use a shader it has to dynamically compile it at run-time from 
    // source code. To do this we need to first create a shader object
    // this creates a shader object of vertex type an returns the ID
    unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
    
    // attach the shader source code to the shader object and compile the shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // let's check if compilation was successful.
    int success;
    // this is the extact length of the shader info log
    char infoLog[512];
    // This allows the developer to query a shader for information given an object 
    // parameter. 
    // You can also use GL_SHADER_TYPE, GL_DELETE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILES\n" << infoLog << '\n';
    }
    
    // the next required shader is the frag shader
    unsigned int fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // let's check again for successful compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAG::COMPILATIONS_FAILOMENTO\n" << infoLog << '\n';
    }

    // To use the compiled shader we have to link them to a shader program object
    // The ative shader program's shader will be used when we issue render calls.
    // This is also where you'll get linking errors if your outputs and inputs don't match
    unsigned int shaderProgram{glCreateProgram()};

    // not attach the compiled shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // This links all the attached shaders in one final shader program object
    glLinkProgram(shaderProgram);

    // now let's test if the linking was successful
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::LINKING_FAILED\n" << infoLog << '\n';
    }
    

    // don't forget to delete the shader objects now that we've linked them
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    /* We've now sent the input vertex data to the GPU and instructed the GPU 
     * how it should process the vertex data in a vertex and fragment shader.
     * OpenGL doesn't know how it should connect this data with the vertex attributes
     */

    // These coordinates in NDC space will make up the triangle that we
    // want to render
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };  

    // create our vertex buffer object. This will pass the data to the gpu
    // all objects in openGL are access via an id. 
    unsigned int VBO;
    // create vertex buffers for this id
    glGenBuffers(1, &VBO);

    // so we need to store the data in a vertex array object (VAO)
    unsigned int VAO {};
    // Create a VAO, the first parameter is the amount of arrays that will be bound
    glGenVertexArrays(1, &VAO);

    // to edit this object we need to bind the id to openGL
    // you can bind one object of each type at a time before having to unbind
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // bind the vertex array object
    glBindVertexArray(VAO);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // 0 is the intex of the vertex attrib array we want to activate
    glEnableVertexAttribArray(0);




    // we don't want the application to draw a single image and then
    // immediately quit and close the window. 
    // this checks if the user has requested to close the window
    // Each iteration of this loop is called a frame
    while(!glfwWindowShouldClose(window)){
        // check to see if the user has escaped
        processInput(window);
        // thi checks if any events are triggered (like keyboard/mouse)
        glfwPollEvents();
        // let's test our rendering by clearing the buffer (black)
        // and adding our own color
        // this is a state-setting function for glClear
        glClearColor(0.2, 0.3, 0.3, 1.0);
        // now clear with the color
        // The possible bits we can set are GL_COLOR_BUFFER_BIT
        // GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
        glClear(GL_COLOR_BUFFER_BIT);

        // We can now activate the program object
        // From now one every shader and rendering call will use this program object
        glUseProgram(shaderProgram);
        // Usually when you have multiple objects, you first generate/configure all the 
        // VAOs ( and the required VBO and attribute pointers ) and store those for later use.
        // Once we want to draw one of our objects, we take the corresponding VAO, bind it, and 
        // then draw the object and unbind the VAO again
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // this will swap the color buffer that is used to render 
        // and show the output on the screen
        glfwSwapBuffers(window);
    }

    // cleanup (optional)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

        
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


