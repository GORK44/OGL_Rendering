//
//  Material.hpp
//  OpenGLtest1
//
//  Created by apple on 2019/12/11.
//  Copyright © 2019 apple. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include <iostream>

#include "gork.hpp"


typedef enum {
    common = 0,          // Material shader
    pbr
} MaterialType;





//=========================================================
//改成虚基类，实现多态！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！/
//pbr 和 普通 的 material
//=========================================================

class Material
{
private:
    
    char folderName[100];
    
    unsigned int albedoMap    = 0;
    unsigned int normalMap    = 0;
    unsigned int metallicMap  = 0;
    unsigned int roughnessMap = 0;
    unsigned int aoMap        = 0;
    
    
    
    MaterialType materialType;
    
    
public:
  
//    bool IBL = false;
    
    Material(MaterialType maT, char const *pathFolder, char const *suffix)
    {
        materialType = maT;
        switch (materialType) {
            case pbr:
                std::strcpy(folderName, pathFolder);
                albedoMap    = GORK::LoadTexture(folderName, "/basecolor", suffix, NotFlip,Repeat,RGB,mipmap);
                normalMap    = GORK::LoadTexture(folderName, "/normal", suffix, NotFlip,Repeat,RGB,mipmap);
                metallicMap  = GORK::LoadTexture(folderName, "/metallic", suffix, NotFlip,Repeat,RGB,mipmap);
                roughnessMap = GORK::LoadTexture(folderName, "/roughness", suffix, NotFlip,Repeat,RGB,mipmap);
                aoMap        = GORK::LoadTexture(folderName, "/ao", suffix, NotFlip,Repeat,RGB,mipmap);
                break;
                
            case common:
                
                break;
                
            default:
                break;
        }
        
    }
    
    
    void BindShaderTexture()
    {
        if (Effect.IBL) {
            // 材质球
            // 绑定PBR材质贴图（shader之后会调用）//前三张图是irradianceMap，prefilterMap，brdfLUTTexture
            
            //    GORK::ShaderSet_PBR_Ball(pbrShader_redBall);
            ////    pbrShader_redBall.use();
            ////    pbrShader_redBall.setInt("irradianceMap", 0);
            ////    pbrShader_redBall.setInt("prefilterMap", 1);
            ////    pbrShader_redBall.setInt("brdfLUT", 2);
            ////    pbrShader_redBall.setVec3("albedo", 1.0f, 1.0f, 1.0f);
            ////    pbrShader_redBall.setFloat("ao", 1.0f);
            ////
            ////    pbrShader_redBall.setInt("noiseMap", 3);
            //
            //
            //
            //
            //    GORK::ShaderSet_PBR_Model(pbrShader);
            ////    pbrShader.use();
            ////    pbrShader.setInt("irradianceMap", 0);// shader中的 irradianceMap 对应 GL_TEXTURE0 绑定的纹理
            ////    pbrShader.setInt("prefilterMap", 1);// 预滤波贴图
            ////    pbrShader.setInt("brdfLUT", 2); // LUT贴图
            ////    pbrShader.setInt("albedoMap", 3); // 反照率（基础颜色）贴图
            ////    pbrShader.setInt("normalMap", 4); // 法线贴图
            ////    pbrShader.setInt("metallicMap", 5); // 金属度贴图
            ////    pbrShader.setInt("roughnessMap", 6); // 粗糙度贴图
            ////    pbrShader.setInt("aoMap", 7); // 环境光遮蔽贴图
            ////
            ////    pbrShader.setInt("noiseMap", 8);
            //
            
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, albedoMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            glActiveTexture(GL_TEXTURE5);
            glBindTexture(GL_TEXTURE_2D, metallicMap);
            glActiveTexture(GL_TEXTURE6);
            glBindTexture(GL_TEXTURE_2D, roughnessMap);
            glActiveTexture(GL_TEXTURE7);
            glBindTexture(GL_TEXTURE_2D, aoMap);
        } else {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, albedoMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, normalMap);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, metallicMap);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, roughnessMap);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, aoMap);
        }
    }
    
    
    
    
    
    
};









#endif /* Material_hpp */
