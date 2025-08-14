#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "block.h"

#include "shader.h"
#include "camera.h"
#include <filesystem>
#include <iostream>

#include "world.h"


#include "glm-1.0.1/glm/glm.hpp"
#include "glm-1.0.1/glm/gtc/matrix_transform.hpp"
#include "glm-1.0.1/glm/gtc/type_ptr.hpp"

#include "threadPool.h"
//для задач процессора по созданию чанка
ThreadPool pool(4);

GLuint loadTextureArray(const std::vector<std::string>& paths) {
    int width = 0, height = 0, channels = 0;
    unsigned char* first = stbi_load(paths[0].c_str(), &width, &height, &channels, STBI_rgb_alpha);
    if (!first) {
        std::cerr << "Failed to load: " << paths[0] << std::endl;
        return 0;
    }

    GLuint texArrayID;
    glGenTextures(1, &texArrayID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texArrayID);

    // Выделяем память под все слои
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, paths.size() + 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Кладем каждую текстуру в свой слой
    for (int i = 0; i < paths.size(); ++i) {
        unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data) {
            std::cerr << "Failed to load: " << paths[i] << std::endl;
            continue;
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i + 1, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    return texArrayID;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


Camera camera(glm::vec3(0.0f, 100.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed: y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void showFPS(GLFWwindow* window) {
    static double lastTime = glfwGetTime();
    static int frames = 0;

    double currentTime = glfwGetTime();
    double dt = currentTime - lastTime;

    ++frames;

    if (dt >= 1.0f) {
        double fps = frames / dt;
        std::string posX = "X: " + std::to_string(static_cast<int>(camera.position.x));
        std::string posY = "Y: " + std::to_string(static_cast<int>(camera.position.y));
        std::string posZ = "Z: " + std::to_string(static_cast<int>(camera.position.z));
        std::string title = "FPS: " + std::to_string(static_cast<int>(fps))+ " " + posX + ", " + posY + ", " + posZ;

        glfwSetWindowTitle(window, title.c_str());

        frames = 0;
        lastTime = currentTime;
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //msaa
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //set a multisample flag
    glEnable(GL_MULTISAMPLE);
    //set a depth flag
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertex.vs", "fragment.fs");
    Block::init();
    int faceTypeLoc = glGetUniformLocation(ourShader.ID, "faceType");
    std::vector<std::string> texturePaths = {
        "textures/grass_top.jpg",   // blockID = 0
        "textures/dirt.jpg",    // blockID = 1
        "textures/stone.jpg",   // blockID = 2
        "textures/water.jpg"     // blockID = 3
    };
    GLuint textureArrayID = loadTextureArray(texturePaths);

    //setTexture("textures/real_stone.jpg", ourShader);

    World world(pool);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    while (!glfwWindowShouldClose(window))
    {
        showFPS(window);
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);


        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ourShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
        ourShader.setInt("textureArray", 0);
        //block.bindTexture(ourShader);

        // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = camera.GetViewMatrix();
        glm::mat4 projection    = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 2000.0f);
        // retrieve the matrix uniform locations
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        glUniform1i(faceTypeLoc, 1);
        world.setPlayerPosition(camera.position);
        world.update();
        world.draw(ourShader, camera);

        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(CTRL, deltaTime * 2);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}