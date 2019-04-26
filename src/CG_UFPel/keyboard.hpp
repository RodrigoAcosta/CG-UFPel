//
//  keyboard.hpp
//  CG_UFPel
//
//  Created by Rodrigo Acosta on 25/04/19.
//

#ifndef keyboard_hpp
#define keyboard_hpp

#include <stdio.h>
#include <glad/glad.h>

#include "models.hpp"
#include <learnopengl/camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <learnopengl/mesh.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
using namespace glm;

class Keyboard
{
public:
    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------
    void processInput(GLFWwindow *window, Camera *camera, Models *modelos)
    {
        
        currentFrame = glfwGetTime();  //controle suave translation
        currentButtonTime = glfwGetTime(); //control double click keys
        
        deltaTime =  currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        //EXIT PROGRAM
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
        
        //MOVE CAMERA
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            camera->ProcessKeyboard(FORWARD, 0.012f);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camera->ProcessKeyboard(BACKWARD, 0.012f);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camera->ProcessKeyboard(LEFT, 0.012f);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camera->ProcessKeyboard(RIGHT, 0.012f);
        
        //MOVE REFERENCED OBJECT
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            modelos->setVecMovModel(0.0f,((float)deltaTime*50.0f)/10,0.0f);
        
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            modelos->setVecMovModel(0.0f,((float)deltaTime*-50.0f)/10,0.0f);
        
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            modelos->setVecMovModel(((float)deltaTime*-50.0f)/10,0.0f,0.0f);
        
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            modelos->setVecMovModel(((float)deltaTime*50.0f)/10,0.0f,0.0f);
        
        
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
            modelos->createNewModel(); //add new model of object in the scene
            modelos->setModelSetToMov(modelos->getVecModelsSize() - 1); //SETAR O ID DO OBJETO PARA MOVIMENTAR O ULTIMO CRIADO
            
            std::cout << "Add New Object" << std::endl;
            lastButtonTime = currentButtonTime;
        }
        
        
        //Setar qual objeto modificar
        if ((glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
        {
            if( modelos->getModelSetToMov() > 0)
            {
                modelos->setModelSetToMov(modelos->getModelSetToMov() - 1);
            }
            std::cout << "modelSetToMov = " << modelos->getModelSetToMov() << std::endl;
            lastButtonTime = currentButtonTime;
        }
        
        if ((glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
        {
            if( modelos->getModelSetToMov() < (modelos->getVecModelsSize() -1))
            {
                modelos->setModelSetToMov(modelos->getModelSetToMov() + 1);
            }
            std::cout << "modelSetToMov = " << modelos->getModelSetToMov() << std::endl;
            lastButtonTime = currentButtonTime;
        }
        
        //scale up the model slected
        if ((glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
        {
            if(modelos->getVecModelsSize() > 0)
            {
                modelos->scaleUpModel();
                lastButtonTime = currentButtonTime;
                std::cout << "Model Scale Up" << std::endl;
            }else{
                std::cout << "Não há Modelos para aplicar a propriedade de Escala." << std::endl;
            }
        }
        
        //scale up the model slected
        if ((glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
        {
            if(modelos->getVecModelsSize() > 0)
            {
                modelos->scaleDownModel();
                lastButtonTime = currentButtonTime;
                std::cout << "Model Scale Down" << std::endl;
            }else{
                std::cout << "Não há Modelos para aplicar a propriedade de Escala." << std::endl;
            }
        }
        
        //rotate on axis X
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
        {
            if(modelos->getVecModelsSize() > 0)
            {
                modelos->rotateX(deltaTime);
            }else{
                std::cout << "Não há Modelos para aplicar a propriedade de Rotação." << std::endl;
            }
        }
        
        //rotate on axis Y
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
        {
            if(modelos->getVecModelsSize() > 0)
            {
                modelos->rotateY(deltaTime);
            }else{
                std::cout << "Não há Modelos para aplicar a propriedade de Rotação." << std::endl;
            }
        }
        
        //rotate on axis Z
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 0.25))
        {
            if(modelos->getVecModelsSize() > 0)
            {
                modelos->rotateZ(deltaTime);
            }else{
                std::cout << "Não há Modelos para aplicar a propriedade de Rotação." << std::endl;
            }
        }
        
        
        
        
        
        
        
        
        
        
        
        
        
        
    }
    
private:
   
    float currentButtonTime = 0.0f;
    float lastButtonTime = 0.0f;
    float deltaTime = 0.0f;
    float currentFrame = 0.0f;
    float lastFrame = 0.0f;
    
};
#endif
