#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "models.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;
float currentButtonTime = 0.0f;
float lastButtonTime = 0.0f;


//Class Models
Models modelos;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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
    Shader ourShader(FileSystem::getPath("resources/cg_ufpel.vs").c_str(), FileSystem::getPath("resources/cg_ufpel.fs").c_str());

    // load models
    // -----------
    Model ourModel(FileSystem::getPath("resources/objects/nanosuit/nanosuit.obj"));
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        lastFrame = glfwGetTime();

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        
        modelos.DrawModels(ourShader, ourModel);
    

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        modelos.setVecMovModel(0.0f,0.0f,0.0f); //clear vector movement
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    //control double click keys
    currentButtonTime = glfwGetTime();
    
    
    //EXIT PROGRAM
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    
    //MOVE CAMERA
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, 0.012f);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, 0.012f);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, 0.012f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, 0.012f);
    
    //MOVE REFERENCED OBJECT
    deltaTime = lastFrame - currentFrame;
    currentFrame = lastFrame;
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        modelos.setVecMovModel(0.0f,((float)deltaTime*50.0f)/10,0.0f);
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        modelos.setVecMovModel(0.0f,((float)deltaTime*-50.0f)/10,0.0f);
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        modelos.setVecMovModel(((float)deltaTime*-50.0f)/10,0.0f,0.0f);
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        modelos.setVecMovModel(((float)deltaTime*50.0f)/10,0.0f,0.0f);
    
    
    // Draw or Not in wireframe
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    // Play/Stop Animation
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        std::cout << "Play/Stop Animation" << std::endl;
    }
    
    //Add New Object in the scene
    if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
    {
        modelos.createNewModel(); //add new model of object in the scene
        modelos.setModelSetToMov(modelos.getVecModelsSize() - 1); //SETAR O ID DO OBJETO PARA MOVIMENTAR O ULTIMO CRIADO
        
        std::cout << "Add New Object" << std::endl;
        lastButtonTime = currentButtonTime;
    }
    
    
    //Setar qual objeto modificar
    if ((glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
    {
        if( modelos.getModelSetToMov() > 0)
        {
             modelos.setModelSetToMov(modelos.getModelSetToMov() - 1);
        }
        std::cout << "modelSetToMov = " << modelos.getModelSetToMov() << std::endl;
        lastButtonTime = currentButtonTime;
    }
    
    if ((glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
    {
        if( modelos.getModelSetToMov() < (modelos.getVecModelsSize() -1))
        {
            modelos.setModelSetToMov(modelos.getModelSetToMov() + 1);
        }
        std::cout << "modelSetToMov = " << modelos.getModelSetToMov() << std::endl;
        lastButtonTime = currentButtonTime;
    }
    
    //scale up the model slected
    if ((glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
    {
        if(modelos.getVecModelsSize() > 0)
        {
            modelos.scaleUpModel();
            lastButtonTime = currentButtonTime;
            std::cout << "Model Scale Up" << std::endl;
        }else{
             std::cout << "Não há Modelos para aplicar a propriedade de Escala." << std::endl;
        }
    }
    
    //scale up the model slected
    if ((glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
    {
        if(modelos.getVecModelsSize() > 0)
        {
            modelos.scaleDownModel();
            lastButtonTime = currentButtonTime;
            std::cout << "Model Scale Down" << std::endl;
        }else{
            std::cout << "Não há Modelos para aplicar a propriedade de Escala." << std::endl;
        }
        
    }
    
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
