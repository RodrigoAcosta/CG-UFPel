#ifndef MODELS_H
#define MODELS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <glm/gtc/type_ptr.hpp>
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

#define PI 3.14159265

class Models
{
public:
    
    /*  Functions   */
    void createNewModel()
    {
        vecModels.push_back(glm::mat4(1.0f));  //add new matrix identity at vector of matrix
    
        vecModels.back() = glm::translate(vecModels.back(), glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
        vecModels.back() = glm::scale(vecModels.back(), glm::vec3(0.2f, 0.2f, 0.2f));    // it's a bit too big for our scene, so scale it down
    }
    
    //GET'S PARA X,Y,Z DO MODELO,TAMANHO
    int getVecModelsSize()
    {
        return vecModels.size();
    }
    
    //GET X,Y,Z OF MODEL SELECTED
    float getXModel()
    {
        const float *pSource = (const float*)glm::value_ptr(vecModels[getModelSetToMov()]);
        return pSource[12];
    }
    
    float getYModel()
    {
        const float *pSource = (const float*)glm::value_ptr(vecModels[getModelSetToMov()]);
        return pSource[13];
    }
    
    float getZModel()
    {
        const float *pSource = (const float*)glm::value_ptr(vecModels[getModelSetToMov()]);
        return pSource[14];
    }
    
    void setXModel(float x)
    {
        float *pSource = (float*)glm::value_ptr(vecModels[getModelSetToMov()]);
        pSource[12] = x;
    }
    
    void setYModel(float y)
    {
        float *pSource = (float*)glm::value_ptr(vecModels[getModelSetToMov()]);
        pSource[13] = y;
    }
    
    void setZModel(float z)
    {
        float *pSource = (float*)glm::value_ptr(vecModels[getModelSetToMov()]);
        pSource[14] = z;
    }
    
    
    void setModelSetToMov (int n)
    {
        //SETAR O ID DO OBJETO WITH N
        modelSetToMov = n;
    }
    
    int getModelSetToMov ()
    {
        return modelSetToMov;
    }
    
    void setVecMovModel(float x, float y, float z)
    {
        vecMovModel.x = x;
        vecMovModel.y = y;
        vecMovModel.z = z;
    }
    
    glm::vec3 getVecMovModel()
    {
        return vecMovModel;
    }
    
    
    // draws the model, and thus all its meshes
    void DrawModels(Shader shader, Model ourModel)
    {
        for(int it = 0; it < getVecModelsSize(); it++){
            
            //IF DE MOVIMENTAR APENAS O MODELO QUE ESTA APONTADO NO MOMENTO
            if(it == getModelSetToMov())
            {
                vecModels[it] = glm::translate(vecModels[it], glm::vec3(getVecMovModel().x, getVecMovModel().y, getVecMovModel().z));
            }
            shader.setMat4("model", vecModels[it]);
            ourModel.Draw(shader);
        }
    }
    
    void scaleUpModel()
    {
        vecModels[getModelSetToMov()] = glm::scale(vecModels[getModelSetToMov()], glm::vec3(1.2f, 1.2f, 1.2f));
    }
    
    void scaleDownModel()
    {
        vecModels[getModelSetToMov()] = glm::scale(vecModels[getModelSetToMov()], glm::vec3(0.8f, 0.8f, 0.8f));
    }
    
    void rotateX(float deltaTime)
    {
        vecModels[getModelSetToMov()] = glm::rotate(vecModels[getModelSetToMov()], deltaTime, glm::vec3(0.2f, 0.0f, 0.0f));
    }
    
    void rotateY(float deltaTime)
    {
        vecModels[getModelSetToMov()] = glm::rotate(vecModels[getModelSetToMov()], deltaTime, glm::vec3( 0.0f, 0.2f, 0.0f));
    }
    
    void rotateZ(float deltaTime)
    {
        vecModels[getModelSetToMov()] = glm::rotate(vecModels[getModelSetToMov()], deltaTime, glm::vec3(0.0f, 0.0f, 0.2f));
    }
    
    
    //Linear Translation to Point OF MODELS
    void printPosition()
    {
        std::cout << "X: " << getXModel() << endl;
        std::cout << "Y: " << getYModel() << endl;
        std::cout << "Z: " << getZModel() << endl;
        
    }
    
    
    //Linear Translation to Point OF MODELS
    void linearTranslation(glm::vec3 toPoint, int tempoTotal, Shader shader, Model ourModel, GLFWwindow* window){
        
        //regra de 3 para transições suaves
        //Distancia dos Pontos = sqrt( pow (x2 - x1, 2) + pow (y2 - y1, 2) + pow (z2 - z1, 2))
        //tempoTotal(segs) -- Distancia dos Pontos
        //       DeltaTime -- x
        //(deltaTime*DistP1P2)/tempoTotal
        //Calcula a Distancia entre os pontos
        
        
        float distanciaP1P2X = sqrt( pow (toPoint.x - getXModel(), 2));
        float distanciaP1P2Y = sqrt( pow (toPoint.y - getYModel(), 2));
        float distanciaP1P2Z = sqrt( pow (toPoint.z - getZModel(), 2));
        
        //float tempoIni = glfwGetTime();
        
        currentFrame = glfwGetTime();
        lastFrame = currentFrame;
        do
        {
            deltaTime =  currentFrame - lastFrame;
            
//            std:: cout << "DeltaTime: "<< deltaTime << endl;
        setVecMovModel(((float)deltaTime*distanciaP1P2X)/tempoTotal,((float)deltaTime*distanciaP1P2Y)/tempoTotal,((float)deltaTime*distanciaP1P2Z)/tempoTotal);
            
            DrawModels(shader, ourModel);
           
            glfwSwapBuffers(window);
            glfwPollEvents();
            
            lastFrame = currentFrame;
            currentFrame = glfwGetTime();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
        }while( ((sqrt( pow (toPoint.x - getXModel(), 2)) > 0.01)
                || (sqrt( pow (toPoint.y - getYModel(), 2)) > 0.01)
                 || (sqrt( pow (toPoint.z - getZModel(), 2)) > 0.01)));
//                && ((currentFrame - tempoIni) <= tempoTotal));
        
        
//        std::cout << "FINISH "<< std::endl;
//        std:: cout << "Decorrido: "<<(currentFrame - tempoIni) << endl;
//        std:: cout << "Tempo Total: "<< tempoTotal << endl;
        setVecMovModel(0.0f,0.0f,0.0f); //clear vector movement
    }
    
    
    //Rotation Models at Point
    void RotationPoint(glm::vec3 inPoint, int tempoVolta, int qtLaps, Shader shader, Model ourModel, GLFWwindow* window){
        
        //regra de 3 para transições suaves
        //tempoVolta(segs) -- 360(Graus)
        //       DeltaTime -- x(Graus)
       
//        float distanciaP1P2X = sqrt( pow (inPoint.x - getXModel(), 2));
//        float distanciaP1P2Y = sqrt( pow (inPoint.y - getYModel(), 2));
//        float distanciaP1P2Z = sqrt( pow (inPoint.z - getZModel(), 2));
//
//
//        float raio = distanciaP1P2X +distanciaP1P2Y+distanciaP1P2Z;
        
        
        float tempoIni = 0;
        glm::vec3 savePoint;

        for (int i = 0; i <= qtLaps; i++)
        {
        
            tempoIni = glfwGetTime();
            currentFrame = glfwGetTime();
            lastFrame = currentFrame;

            do
            {
                deltaTime =  currentFrame - lastFrame;
                
                savePoint.x = getXModel();
                savePoint.y = getYModel();
                savePoint.z = getZModel();

                jumpToXYZ(inPoint); //salta para o ponto que esta girando em volta

                //rotaciona
                vecModels[getModelSetToMov()] = glm::rotate(vecModels[getModelSetToMov()], 0.01f, glm::vec3( 0.0f, 1.0f, 0.0f));

                //volta para a posicao inicial
                jumpToXYZ(savePoint);

                //translate
                setVecMovModel(0.1,0.0f,0.0f);
                
                DrawModels(shader, ourModel);

                glfwSwapBuffers(window);
                glfwPollEvents();

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                lastFrame = currentFrame;
                currentFrame = glfwGetTime();
                
            }while((currentFrame - tempoIni) <= tempoVolta);

        }
        
        std::cout << "Tempo Final: " << (currentFrame - tempoIni) << std::endl;
        
        
        
        
        
        
        
        
        
    }
    
    
    //Jump To XYZ
    void jumpToXYZ(glm::vec3 jumpTo){
        
        setXModel(jumpTo.x);
        setYModel(jumpTo.y);
        setZModel(jumpTo.z);
    }
    
    
    
    
    
    
private:
    
    std::vector<glm::mat4> vecModels; //Vector of Matrix of Models
    glm::vec3 vecMovModel = glm::vec3(0.0f,0.0f,0.0f); //vetor to translate objets of scene
    int modelSetToMov = 0; //variable to moviment the models
    
    float deltaTime = 0.0f;
    float currentFrame = 0.0f;
    float lastFrame = 0.0f;
    /*  Functions   */
    
};
#endif
