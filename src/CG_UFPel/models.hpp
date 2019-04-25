#ifndef MODELS_H
#define MODELS_H

#include <glad/glad.h>

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
    
    int getVecModelsSize()
    {
        return vecModels.size();
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
    
    
private:
    
    std::vector<glm::mat4> vecModels; //Vector of Matrix of Models
    glm::vec3 vecMovModel = glm::vec3(0.0f,0.0f,0.0f); //vetor to translate objets of scene
    int modelSetToMov = 0; //variable to moviment the models
    
    /*  Functions   */
    
};
#endif
