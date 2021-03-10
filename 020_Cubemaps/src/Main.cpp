#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera cam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH  / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

std::string const VERT_OBJECT_SHADER { "shaders/vert/geometry.glsl" };
std::string const FRAG_OBJECT_SHADER { "shaders/frag/geometry.glsl" };
std::string const VERT_SKYBOX_SHADER { "shaders/vert/skybox.glsl" };
std::string const FRAG_SKYBOX_SHADER { "shaders/frag/skybox.glsl" };
std::string const VERT_TESTMODEL_SHADER { "shaders/vert/testModel.glsl" };
std::string const FRAG_TESTMODEL_SHADER { "shaders/frag/testModel.glsl" };
std::string const CUBE_TEXTURE{ "textures/marble.jpg" };
std::string const FLOOR_TEXTURE{ "textures/rock.jpg" };
std::string const TEST_MODEL{ "obj/backpack/backpack.obj" };
std::vector<std::string> SKYBOX_TEXTURES{{
    "textures/skybox/right.jpg",
    "textures/skybox/left.jpg",
    "textures/skybox/top.jpg",
    "textures/skybox/bottom.jpg",
    "textures/skybox/front.jpg",
    "textures/skybox/back.jpg"
}};

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
 
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    std::string vertexShaderPath = (CWD + VERT_OBJECT_SHADER);
    std::string fragShaderPath = (CWD + FRAG_OBJECT_SHADER);
    ShaderProgram shader(vertexShaderPath, fragShaderPath);

    std::string vertexSkyboxShaderPath = (CWD + VERT_SKYBOX_SHADER);
    std::string fragSkyboxShaderPath = (CWD + FRAG_SKYBOX_SHADER);
    ShaderProgram skyboxShader(vertexSkyboxShaderPath, fragSkyboxShaderPath);

    std::string vertexTestModelShaderPath= (CWD + VERT_TESTMODEL_SHADER);
    std::string fragTestModelShaderPath= (CWD + FRAG_TESTMODEL_SHADER);
    ShaderProgram testModelShader(vertexTestModelShaderPath, fragTestModelShaderPath);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  2.0f, 2.0f								
    };
    float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
    // model 
    // testModelShader.use();
    std::string testModelFile = CWD + TEST_MODEL; 
    AssimpModel testModel{ testModelFile };

    // cube VAO
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // plane VAO
    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // load textures
    // -------------
    std::string cubeTexturePath= (CWD + CUBE_TEXTURE);
    std::string floorTexturePath= (CWD + FLOOR_TEXTURE);
    Texture cubeTexture {cubeTexturePath};
    Texture floorTexture {floorTexturePath}; 

    // load cubemap
    // --------------
    // cubemaps are loaded just like other textures
    unsigned int skyboxID;
    glGenTextures(1, &skyboxID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID); // you need to load each image manually though
    int width, height, nrChannels;
    unsigned char *data;  
    unsigned int count{0};
    // load each side of the cube map
    for(std::string skyboxTexture : SKYBOX_TEXTURES){
        std::string skyboxTexturePath = CWD + skyboxTexture;
        stbi_set_flip_vertically_on_load(false);
        data = stbi_load(skyboxTexturePath.c_str(), &width, &height, &nrChannels, 0);
        if(data){
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + count,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
        }
        else{
            std::cout << "Cubemap tex failed to load at path " << skyboxTexture << '\n';
        }
        stbi_image_free(data);
        ++count;
    }
    // specify wrapping and fitering methods
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // this is the textures wrapping method in the r coordinate (like z for positions)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  

    

    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("texture1", 0);
    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    testModelShader.use();
    testModel.draw(testModelShader);
    // move the test model up a bit
    glm::mat4 testModelMatrix {1.0};
    testModelMatrix[3] = glm::vec4{0.0, 1.0, 0.0, 1.0};
    testModelMatrix = glm::scale(testModelMatrix, glm::vec3{0.3});
    testModelShader.setMatrix("model", testModelMatrix);
    testModelShader.setInt("skybox", 5);

    // render loop
    // -----------
    while(!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = cam.getViewMatrix();
        glm::mat4 projection = cam.getProjectionMatrix();
        shader.setMatrix("view", view);
        shader.setMatrix("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture); 	
        glm::mat3 cubeNormalMatrix{glm::mat3(glm::transpose(glm::inverse(model)))};
        shader.setMatrix3("normalMatrix", cubeNormalMatrix);
        shader.setVec3("cameraPos", cam.getPos());
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMatrix("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
        shader.setMatrix("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        shader.setMatrix("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // test model
        testModelShader.use();
        testModelShader.setMatrix("projection", projection);
        testModelShader.setMatrix("view", view); 
        float rotAmount = 0.11f * deltaTime;
        testModelMatrix = glm::rotate(testModelMatrix, rotAmount, glm::vec3(0.2f, 0.2f, 0.6f));
        testModelShader.setMatrix("model", testModelMatrix); 
        // setting the light pos in here in case we decide to animate it
        testModelShader.setVec3("cameraPos", cam.getPos());
        glm::mat3 normalMatrix{glm::mat3(glm::transpose(glm::inverse(testModelMatrix)))};
        testModelShader.setMatrix3("normalMatrix", normalMatrix);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
        testModel.draw(testModelShader);

        // skybox
        // Draw the skybox after everything else
        // It is set to maximum depth in the vertex
        // shader, so it will only draw where nothing else
        // is present.
        // This is also why the depth function is GL_LEQUAL
        // since the skybux is at maximum 1.0 deth
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMatrix("view", glm::mat4(glm::mat3(cam.getViewMatrix())));
        skyboxShader.setMatrix("projection", cam.getProjectionMatrix());
        // bind the skybox
        glBindVertexArray(skyboxVAO);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
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
