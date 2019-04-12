#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

//Vector of Matrix of Models
std::vector<glm::mat4> modelos;
glm::vec3 vecMovModel(glm::vec3(0.0f)); //vetor to translate objets of scene
int modelSetToMov = 0; //variable to moviment the models


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
    
    //Models
    // render the loaded model
    //std::vector<glm::mat4> modelos;
    
    modelos.push_back(glm::mat4(1.0f));  //objeto 1
    //modelos.push_back(glm::mat4(1.0f));  //objeto 2
    
    //percorre todos os modelos setando para escala adequada e translada para a origem certa
    for(int it = 0; it < modelos.size(); it++){
        modelos[it] = glm::translate(modelos[it], glm::vec3(0.0f + it, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        modelos[it] = glm::scale(modelos[it], glm::vec3(0.2f, 0.2f, 0.2f));    // it's a bit too big for our scene, so scale it down
    }
    
    
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

        
        //seta todas as matrizes de modelos para poder renderizar
        for(int it = 0; it < modelos.size(); it++){
            
            //IF DE MOVIMENTAR APENAS O MODELO QUE ESTA APONTADO NO MOMENTO
            if(it == modelSetToMov)
            {
            modelos[it] = glm::translate(modelos[it], glm::vec3(vecMovModel.x, vecMovModel.y, vecMovModel.z));
            }
            ourShader.setMat4("model", modelos[it]);
            ourModel.Draw(ourShader);
        }
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        vecMovModel = glm::vec3(0.0f); //clear vector movement
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
    {
        vecMovModel.x = 0.0f;
        vecMovModel.y = ((float)deltaTime*50.0f)/10;
        vecMovModel.z = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vecMovModel.x = 0.0f;
        vecMovModel.y = ((float)deltaTime*-50.0f)/10;
        vecMovModel.z = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vecMovModel.x = ((float)deltaTime*-50.0f)/10;
        vecMovModel.y = 0.0f;
        vecMovModel.z = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vecMovModel.x = ((float)deltaTime*50.0f)/10;
        vecMovModel.y = 0.0f;
        vecMovModel.z = 0.0f;
    }
    
    
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
    if ((glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1))
    {
        modelos.push_back(glm::mat4(1.0f));  //objeto com matriz identidade
        modelos.back() = glm::translate(modelos.back(), glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        modelos.back() = glm::scale(modelos.back(), glm::vec3(0.2f, 0.2f, 0.2f));    // it's a bit too big for our scene, so scale it down
        
        modelSetToMov = modelos.size() - 1;
        //SETAR O ID DO OBJETO PARA MOVIMENTAR O ULTIMO CRIADO
        
        
        std::cout << "Add New Object" << std::endl;
        lastButtonTime = currentButtonTime;
    }
    
    
    //Setar qual objeto modificar
    if ((glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1))
    {
        if( modelSetToMov > 0)
        {
             modelSetToMov -= 1;
        }
        std::cout << "modelSetToMov = " << modelSetToMov << std::endl;
        lastButtonTime = currentButtonTime;
    }
    
    if ((glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1))
    {
        if( modelSetToMov < (modelos.size() -1))
        {
            modelSetToMov += 1;
        }
        std::cout << "modelSetToMov = " << modelSetToMov << std::endl;
        lastButtonTime = currentButtonTime;
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
