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
//#include "Model.hpp"

#include "gork.hpp"






// 相机 camera
//Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));

// settings 窗口宽和长
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const bool Flip = true;//y轴翻转
const bool NotFlip = false;
const bool Repeat = true;//边缘重复
const bool notRepeat = false;
const bool sRGB = true;//是sRGB纹理，已经gammaCorrection
const bool RGB = false;


//typedef struct{
//    ...
//    ...
//
//} Material;
//https://github.com/raysan5/raylib/blob/ea5cd42e6ccc3c0e520b98a2eb9ebc2c6ce62431/src/raylib.h
//typedef enum {
//    UNIFORM_FLOAT = 0,
//    UNIFORM_VEC2,
//    UNIFORM_VEC3,
//    UNIFORM_VEC4,
//    UNIFORM_INT,
//    UNIFORM_IVEC2,
//    UNIFORM_IVEC3,
//    UNIFORM_IVEC4,
//    UNIFORM_SAMPLER2D
//} ShaderUniformDataType;
//https://github.com/GORK44/raylib/blob/master/examples/models/models_material_pbr.c
// Material type (generic)
//typedef struct Material {
//    Shader shader;          // Material shader
//    MaterialMap *maps;      // Material maps array (MAX_MATERIAL_MAPS)
//    float *params;          // Material generic parameters (if required)
//} Material;
//https://github.com/raysan5/raylib/blob/ea5cd42e6ccc3c0e520b98a2eb9ebc2c6ce62431/src/raylib.h



static struct Effect{
    bool IBL = 1;
    bool BLOOM = 0;
}Effect;

typedef enum {
    nearest = 0,          // Material shader
    mipmap,
    linear
} TextureFiltering;

//virtual void Fangfa(){
//
//}
//多态性

//析构函数？



//类{
//    get();
//    set();
//}




//template<typename T>




//#if defined(PLATFORM_DESKTOP)
//mat.shader = LoadShader("resources/shaders/glsl330/pbr.vs", "resources/shaders/glsl330/pbr.fs");
//#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
//mat.shader = LoadShader("resources/shaders/glsl100/pbr.vs", "resources/shaders/glsl100/pbr.fs");
//#endif

#ifndef PI
#define PI 3.14159265358979323846f
#endif

//#define DEG2RAD (PI/180.0f)
//#define RAD2DEG (180.0f/PI)
//
//#define MAX_TOUCH_POINTS        10      // Maximum number of touch points supported
//
//// Allow custom memory allocators
//#ifndef RL_MALLOC
//#define RL_MALLOC(sz)       malloc(sz)
//#endif
//#ifndef RL_CALLOC
//#define RL_CALLOC(n,sz)     calloc(n,sz)
//#endif
//#ifndef RL_FREE
//#define RL_FREE(p)          free(p)
//#endif



//#pragma



//内联(inline)函数



//智能指针



//浅拷贝与深拷贝



// Some Basic Colors
// NOTE: Custom raylib color palette for amazing visuals on WHITE background
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

    
    //======================
    //pbr
    static void SetupFramebuffer(unsigned int &captureFBO, unsigned int &captureRBO); // IBL 漫反射辐照度
    // pbr: setup framebuffer 设定帧缓冲（记录HDR转换为立方体的六个面）
    
    static void HdrToCubemap(unsigned int &envCubemap, glm::mat4 captureProjection, glm::mat4 captureViews[6]);//HDR转换成立方体贴图
    
    static void CreateIrradianceMap(unsigned int &irradianceMap, unsigned int &envCubemap, unsigned int &captureFBO, unsigned int &captureRBO, glm::mat4 captureProjection, glm::mat4 captureViews[6]);//创建漫反射辐照度贴图
    
    static void CreatePrefilterMap(unsigned int &prefilterMap, unsigned int &envCubemap, unsigned int &captureFBO, unsigned int &captureRBO, glm::mat4 captureProjection, glm::mat4 captureViews[6]);//创建预滤波HDR环境贴图(粗糙度用mipmapw解决)
    
    static void CreateBrdfLUTTexture(unsigned int &brdfLUTTexture, unsigned int &captureFBO, unsigned int &captureRBO);//根据所使用的BRDF方程生成2D LUT
    
    //======================

};








#endif /* gork_hpp */
