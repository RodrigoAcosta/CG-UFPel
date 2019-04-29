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
#include <GLFW/glfw3.h>

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
    void processInput(GLFWwindow *window, Camera *camera, Models *modelos, Shader *shader, Model *ourModel)
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
        if ((glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1.25))
        {
            if(modelos->getVecModelsSize() > 0)
            {
                std::cout << "Play/Stop Animation" << std::endl;
                lastButtonTime = currentButtonTime;
            }else{
                std::cout << "Não há Modelos para aplicar Animação." << std::endl;
            }
        
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
        
        //Linear Translation
        if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1.25))
        {
            if(modelos->getVecModelsSize() > 0)
            {
                std::cout << "Linear Translation Function" << std::endl;
                std::cout << "Inform X:" << std::endl;
                std::cin >> coordLinearTranslation.x;
                std::cout << "Inform Y:" << std::endl;
                std::cin >> coordLinearTranslation.y;
                std::cout << "Inform Z:" << std::endl;
                std::cin >> coordLinearTranslation.z;
                std::cout << "Inform Time of Execution:" << std::endl;
                std::cin >> tempoTotal;
                
                
                //PLAY LINEAR TRANSLATION
                modelos->linearTranslation(coordLinearTranslation, tempoTotal, *shader, *ourModel, window);
                
                coordLinearTranslation.x = 0.0f;
                coordLinearTranslation.y = 0.0f;
                coordLinearTranslation.z = 0.0f;
                
                lastButtonTime = currentButtonTime;
            }else{
                std::cout << "Não há Modelos para aplicar Translação." << std::endl;
            }
        
        }
        
        //Rotation at Point
        if ((glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) && (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1.25))
        {
            
            if(modelos->getVecModelsSize() > 0)
            {
                std::cout << "Rotation At Point Function" << std::endl;
                std::cout << "Inform X:" << std::endl;
                std::cin >> coordRotationPoint.x;
                std::cout << "Inform Y:" << std::endl;
                std::cin >> coordRotationPoint.y;
                std::cout << "Inform Z:" << std::endl;
                std::cin >> coordRotationPoint.z;
                std::cout << "Inform Lap Time:" << std::endl;
                std::cin >> tempoVolta;
                std::cout << "Inform qt Laps:" << std::endl;
                std::cin >> qtLaps;
                
                
                //PLAY LINEAR TRANSLATION
                modelos->RotationPoint(coordRotationPoint, tempoVolta, qtLaps, *shader, *ourModel, window);
                
                coordRotationPoint.x = 0.0f;
                coordRotationPoint.y = 0.0f;
                coordRotationPoint.z = 0.0f;
                qtLaps = 0;
                
                lastButtonTime = currentButtonTime;
            }else{
                std::cout << "Não há Modelos para aplicar Translação." << std::endl;
            }
            
        }
        
        
        //Jump To X,Y,Z
        if ((glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1.25))
        {
            
            if(modelos->getVecModelsSize() > 0)
            {
                std::cout << "Jump To:" << std::endl;
                std::cout << "Inform X:" << std::endl;
                std::cin >> jumpToXYZ.x;
                std::cout << "Inform Y:" << std::endl;
                std::cin >> jumpToXYZ.y;
                std::cout << "Inform Z:" << std::endl;
                std::cin >> jumpToXYZ.z;
                
                
                //PLAY LINEAR TRANSLATION
                modelos->jumpToXYZ(jumpToXYZ);
                
                jumpToXYZ.x = 0.0f;
                jumpToXYZ.y = 0.0f;
                jumpToXYZ.z = 0.0f;
                
                lastButtonTime = currentButtonTime;
            }else{
                std::cout << "Não há Modelos para aplicar Translação." << std::endl;
            }
            
        }
        
        
        
        
        
        
        
        
        
        //printa posicao do objeto
        if ((glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) && ((currentButtonTime - lastButtonTime) > 1.25))
        {
            
            modelos->printPosition();
            
        }
        
    }// End Capture Keys
    
    float getLinearTranslationX(){
        return coordLinearTranslation.x;
    }
    
    float getLinearTranslationY(){
        return coordLinearTranslation.y;
    }
    
    float getLinearTranslationZ(){
        return coordLinearTranslation.z;
    }
    
    
private:
   
    float currentButtonTime = 0.0f;
    float lastButtonTime = 0.0f;
    float deltaTime = 0.0f;
    float currentFrame = 0.0f;
    float lastFrame = 0.0f;
    
    //Variables to Linear Translation
    glm::vec3 coordLinearTranslation;
    int tempoTotal;
    
    //Variable to Rotation at Point
    glm::vec3 coordRotationPoint;
    int tempoVolta;
    int qtLaps;
    
    //Variable to JumpTo
    glm::vec3 jumpToXYZ;
    
};
#endif
