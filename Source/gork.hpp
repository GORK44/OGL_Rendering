//
//  gork.hpp
//  OpenGLtest1
//
//  Created by apple on 2019/12/8.
//  Copyright © 2019 apple. All rights reserved.
//

#ifndef gork_hpp
#define gork_hpp

#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>


#include "Shader.hpp"
#include "Camera.hpp"
#include "Model.hpp"

#include "gork.hpp"




// settings 窗口宽和长
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const bool Flip = true;//y轴翻转
const bool NotFlip = false;
const bool Repeat = true;//边缘重复
const bool notRepeat = false;
const bool sRGB = true;//是sRGB纹理，已经gammaCorrection
const bool RGB = false;




static struct Effect{
    bool IBL = 1;
    bool BLOOM = 0;
}Effect;

typedef enum {
    nearest = 0,          // Material shader
    mipmap,
    linear
} TextureFiltering;


#ifndef PI
#define PI 3.14159265358979323846f
#endif


#define LIGHTGRAY  CLITERAL(Color){ 200, 200, 200, 255 }   // Light Gray
#define GRAY       CLITERAL(Color){ 130, 130, 130, 255 }   // Gray
#define DARKGRAY   CLITERAL(Color){ 80, 80, 80, 255 }      // Dark Gray
#define YELLOW     CLITERAL(Color){ 253, 249, 0, 255 }     // Yellow
#define GOLD       CLITERAL(Color){ 255, 203, 0, 255 }     // Gold
#define ORANGE     CLITERAL(Color){ 255, 161, 0, 255 }     // Orange
#define PINK       CLITERAL(Color){ 255, 109, 194, 255 }   // Pink
#define RED        CLITERAL(Color){ 230, 41, 55, 255 }     // Red
#define MAROON     CLITERAL(Color){ 190, 33, 55, 255 }     // Maroon
#define GREEN      CLITERAL(Color){ 0, 228, 48, 255 }      // Green
#define LIME       CLITERAL(Color){ 0, 158, 47, 255 }      // Lime
#define DARKGREEN  CLITERAL(Color){ 0, 117, 44, 255 }      // Dark Green
#define SKYBLUE    CLITERAL(Color){ 102, 191, 255, 255 }   // Sky Blue
#define BLUE       CLITERAL(Color){ 0, 121, 241, 255 }     // Blue
#define DARKBLUE   CLITERAL(Color){ 0, 82, 172, 255 }      // Dark Blue
#define PURPLE     CLITERAL(Color){ 200, 122, 255, 255 }   // Purple
#define VIOLET     CLITERAL(Color){ 135, 60, 190, 255 }    // Violet
#define DARKPURPLE CLITERAL(Color){ 112, 31, 126, 255 }    // Dark Purple
#define BEIGE      CLITERAL(Color){ 211, 176, 131, 255 }   // Beige
#define BROWN      CLITERAL(Color){ 127, 106, 79, 255 }    // Brown
#define DARKBROWN  CLITERAL(Color){ 76, 63, 47, 255 }      // Dark Brown

#define WHITE      CLITERAL(Color){ 255, 255, 255, 255 }   // White
#define BLACK      CLITERAL(Color){ 0, 0, 0, 255 }         // Black
#define BLANK      CLITERAL(Color){ 0, 0, 0, 0 }           // Blank (Transparent)
#define MAGENTA    CLITERAL(Color){ 255, 0, 255, 255 }     // Magenta
#define RAYWHITE   CLITERAL(Color){ 245, 245, 245, 255 }   // My own White (raylib logo)


class GORK {
    
public:
    // 相机 camera
//    Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));
    static Camera camera;

    static float lastX;
    static float lastY;
    static bool firstMouse;
    static float deltaTime; // 当前帧与上一帧的时间差
    static float lastFrame; // 上一帧的时间
    
    
    
//    //HDR
//    static bool hdr;
//    static bool hdrKeyPressed;
//    static float exposure;
    
    static bool bloom;  //bloom
    static bool bloomKeyPressed;


    


    
    static void Framebuffer_size_callback(GLFWwindow* window, int width, int height); // 在每次窗口大小被调整的时候被调用
    static void ProcessInput(GLFWwindow *window); //输入控制
    static void Mouse_callback(GLFWwindow* window, double xpos, double ypos);//鼠标移动控制方向
    static void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//滚轮控制FOV
    
    
    
    static GLFWwindow* InitWindow();// 初始化和配置
    
    
    static unsigned int LoadTexture(char const *pathFolder, const char *fileName, char const *suffix, bool isFlipped, bool isRepeated, bool gammaCorrection, TextureFiltering toSmall);//加载纹理贴图（反转，边缘重复，是sRGB纹理）(suffix后缀)
//    static unsigned int LoadTexture(char const *pathFolder, const char *fileName, char const *suffix, bool isFlipped, bool isRepeated, bool gammaCorrection, bool mip);//加载纹理贴图（反转，边缘重复，是sRGB纹理）(suffix后缀)
    static unsigned int LoadCubemap(std::vector<std::string> faces);//加载立方体贴图
    
    
    //绘制几何体
    static void RenderCube();
    static void RenderQuad();
    static void RenderSphere();
    
    
    //插值
    static float Lerp(float a, float b, float f);

    
    //======================
    //pbr
    static void SetupFramebuffer(unsigned int &captureFBO, unsigned int &captureRBO); // IBL 漫反射辐照度
    // pbr: setup framebuffer 设定帧缓冲（记录HDR转换为立方体的六个面）
    
    static void HdrToCubemap(unsigned int &envCubemap, glm::mat4 &captureProjection, glm::mat4 * captureViews);//HDR转换成立方体贴图
    
    static void CreateIrradianceMap(unsigned int &irradianceMap, unsigned int &envCubemap, unsigned int &captureFBO, unsigned int &captureRBO, glm::mat4 captureProjection, glm::mat4 captureViews[6]);//创建漫反射辐照度贴图
    
    static void CreatePrefilterMap(unsigned int &prefilterMap, unsigned int &envCubemap, unsigned int &captureFBO, unsigned int &captureRBO, glm::mat4 captureProjection, glm::mat4 captureViews[6]);//创建预滤波HDR环境贴图(粗糙度用mipmapw解决)
    
    static void CreateBrdfLUTTexture(unsigned int &brdfLUTTexture, unsigned int &captureFBO, unsigned int &captureRBO);//根据所使用的BRDF方程生成2D LUT
    
    static void BindTexture_IBL(unsigned int irradianceMap, unsigned int prefilterMap, unsigned int brdfLUTTexture);
    //======================
    
    
    static void ShaderSet_PBR_Ball(Shader &pbrShader_redBall);
    
    static void ShaderSet_PBR_Model(Shader &pbrShader);

    
    static glm::mat4 * CubeCapture();
    
    static void SetLights(glm::vec3 * lightPositions, glm::vec3 * lightColors, Shader &pbrShader, Shader &pbrShader_redBall);

    static void MaterialBalls(Shader &pbrShader_redBall, int nrRows = 7, int nrColumns = 7, float spacing = 2.5);
    
    static void ShaderSet(Shader &shader, glm::mat4 &view, glm::vec3 &camPos, glm::mat4 &projection);
    
    
    //================================================
    //deferred
    static void TextureAttachments(unsigned int &gAttachment, int attachmentNumber);//纹理附件
    
    static void RenderBufferObject(unsigned int &rboDepth);
    
    static void SetDeferredLightInfo(const unsigned int NR_LIGHTS, std::vector<glm::vec3> &lightPositions, std::vector<glm::vec3> &lightColors);
    
    static std::vector<glm::vec3> DeferredObjectPositions();
    
    static void gBufferBind(unsigned int gBuffer);
    static void gBufferUnBind();
    
    static void DrawNanosuits(std::vector<glm::vec3> &objectPositions, Shader &shader_GeometryPass, Model &nanosuit);
    
    static void ShaderBindTextures_gBuffer(Shader &shader_LightingPass, unsigned int gPosition, unsigned int gNormal, unsigned int gAlbedoSpec);
    
    static void ShaderSetLights_deferred(Shader &shader_LightingPass, std::vector<glm::vec3> &lightPositions, std::vector<glm::vec3> &lightColors);
    
    static void SetDepthBuffer_gBuffer(unsigned int gBuffer);
    
    static void DrawLights_deferred(Shader &shader_LightBox, std::vector<glm::vec3> &lightPositions, std::vector<glm::vec3> &lightColors);
    
    
    static void TextureAttachments_SSAO(unsigned int ssaoFBO, unsigned int &ssaoColorBuffer);//纹理附件
    //================================================

    
    
};




#endif /* gork_hpp */
