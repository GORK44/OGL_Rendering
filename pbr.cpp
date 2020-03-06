//
//  main.cpp
//  OpenGLtest1
//
//  Created by apple on 2019/5/3.
//  Copyright © 2019 apple. All rights reserved.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Model.hpp"

#include "gork.hpp"
#include "Material.hpp"



//摄像机
Camera GORK::camera(glm::vec3(0.0f, 0.0f, 2.0f));

glm::vec3 cam = glm::vec3(0.0,0.0,50.0);

float scale = 1000.0f; //细，粗粒表面一个片元的微法线m的数量之比（微表面数量之比）(scale越大，粗粒度越粗，一个片元内微表面越少)
float average = 0.0f; //正态分布平均数
float standardDeviation = 0.1f; //标准差
//float standardDeviation = 1.0f; //标准差
float timeNoiseMap = 0.0f;


int main()
{


    // 创建一个窗口对象 glfw window creation
    GLFWwindow* window = GORK::InitWindow();
    if (!window) return -1;


    // ----------------------------------
//     //鼠标控制
    glfwSetCursorPosCallback(window, GORK::Mouse_callback);//鼠标一移动mouse_callback函数就会被调用
    glfwSetScrollCallback(window, GORK::Scroll_callback);//滚轮一滚动scroll_callback函数就会被调用

    //// 捕捉鼠标 tell GLFW to capture our mouse
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // ----------------------------------













    //配置全局opengl状态
    //========================================================================

    //启用深度测试
    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
    glDepthFunc(GL_LEQUAL);//如果目标像素z值<＝当前像素z值，则绘制目标像素(天空盒z=1)

    // 在立方体贴图的面之间进行正确过滤
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);//pre-filter map中高粗糙度(低Mip级别)的立方体贴图接缝处过滤


    //启用模版测试
//    glEnable(GL_STENCIL_TEST);
//    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    //启用混合
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //源因子 等于C¯source的alpha分量，目标因子 等于1− C¯source的alpha分量

    // 启用面剔除
//    glEnable(GL_CULL_FACE); // 启用面剔除，默认剔除背向面（默认 正向面：逆时针，背向面：顺时针）
//    glCullFace(GL_FRONT); //剔除正向面（默认剔除背向面）
//    glFrontFace(GL_CCW); //将 逆时针 的面定义为正向面【默认 正向面：逆时针，背向面：顺时针】
    //========================================================================






    //构建并编译着色器程序
    //========================================================================

    Shader whiteShader("/书/OGL_Test/Shader/singleColor.vs", "/书/OGL_Test/Shader/singleColor.fs");

    Shader screenShader("/书/OGL_Test/Shader/screen.vs", "/书/OGL_Test/Shader/screen.fs");//帧缓冲
    screenShader.use();
    screenShader.setInt("screenTexture", 0);

// ------------------------------------------------------------------------------
    Shader pbrShader_redBall("/书/OGL_Test/Shader/pbr_redBall.vs", "/书/OGL_Test/Shader/pbr_redBall_specularIBL.fs");
    Shader pbrShader("/书/OGL_Test/Shader/pbr.vs", "/书/OGL_Test/Shader/pbr_IBL.fs");
   
    Shader backgroundShader("/书/OGL_Test/Shader/background.vs", "/书/OGL_Test/Shader/background.fs");


    GORK::ShaderSet_PBR_Ball(pbrShader_redBall);
    
    GORK::ShaderSet_PBR_Model(pbrShader);
    
    backgroundShader.use();
    backgroundShader.setInt("environmentMap", 0);



    // ------------------------------------------------------------------------------
    Material material_gold(pbr, "/书/OGL_Test/材质/gold-scuffed-Unreal-Engine", ".png");
    Material material_greasy(pbr, "/书/OGL_Test/材质/greasy-metal-pan1-Unreal-Engine 2", ".png");
    Material material_iron(pbr, "/书/OGL_Test/材质/Iron-Scuffed_Unreal-Engine", ".png");
    Material material_graybricks(pbr, "/书/OGL_Test/材质/gray-bricks1-ue4", ".png");

    Material material_cerberus(pbr, "/书/OGL_Test/枪Cerberus_by_Andrew_Maximov", ".tga");
    
    
    Model gun("/书/OGL_Test/枪Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX");
    Model bunny("/书/OGL_Test/模型/bunny.obj");
    Model armour("/书/OGL_Test/模型/Soi_Armour_A.fbx");
    Model cabinet("/书/OGL_Test/模型/cabinet.FBX");
    Model spoon("/书/OGL_Test/模型/SpoonHD.fbx");
    Model dragon("/书/OGL_Test/模型/stanford-dragon.fbx");
    // ------------------------------------------------------------------------------




    
    
    
    //======================
    unsigned int noiseMap        = GORK::LoadTexture("/书/OGL_Test/", "/z1000",".jpg",NotFlip,Repeat,RGB,mipmap);
    //    unsigned int noiseMap        = GORK::LoadTexture("/书/OGL_Test/", "/z1000m",".png",NotFlip,Repeat,RGB,mipmap);
    unsigned int noiseMap1        = GORK::LoadTexture("/书/OGL_Test/", "/z1000",".jpg",Flip,Repeat,RGB,mipmap);
    //=========================
    
    
    
    
    


    // lights
    // ------
    glm::vec3 lightPositions[] = {
        glm::vec3(-10.0f,  10.0f, 10.0f),
        glm::vec3( 10.0f,  10.0f, 10.0f),
        glm::vec3(-10.0f, -10.0f, 10.0f),
        glm::vec3( 10.0f, -10.0f, 10.0f),
    };
    glm::vec3 lightColors[] = {
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f),
        glm::vec3(300.0f, 300.0f, 300.0f)
    };
    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;

// ------------------------------------------------------------------------------

    //========================================================================



    //========================================================================
    // IBL 漫反射辐照度 1.HDR转换立方体贴图

//    // pbr: setup framebuffer 设定帧缓冲（记录HDR转换为立方体的六个面）
//    // ----------------------
    unsigned int captureFBO;
    unsigned int captureRBO;

    GORK::SetupFramebuffer(captureFBO,captureRBO);
    
    

    
    
    
    
    // 设置投影和观察矩阵，以将数据捕获到6个立方图面方向上
    // ----------------------------------------------------------------------------------------------
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);//投影矩阵（FOV视锥体竖直张开角度，横向竖直张开角度比，近裁剪面深度，远裁剪面深度）

    
    
    
    
    unsigned int envCubemap; // 帧缓冲的纹理附件（生成相应的立方体贴图，为其六个面预先分配内存）

    GORK::HdrToCubemap(envCubemap, captureProjection, GORK::CubeCapture());//HDR转换为立方体贴图

    
    
    
    // pbr IBL1 : 创建辐照度立方体贴图，然后重新将FBO捕获缩放为辐照度比例。
    // --------------------------------------------------------------------------------
    unsigned int irradianceMap;//帧缓冲的纹理附件
    
//    GORK::CreateIrradianceMap(irradianceMap, envCubemap, captureFBO, captureRBO, captureProjection, captureViews);//创建辐照度立方体贴图
    GORK::CreateIrradianceMap(irradianceMap, envCubemap, captureFBO, captureRBO, captureProjection, GORK::CubeCapture());//创建辐照度立方体贴图
    
    
    
    
    

    // pbr IBL2 (specular): 创建预滤波HDR环境贴图，帧缓冲纹理附件大小：128 * 128
    // --------------------------------------------------------------------------------
    unsigned int prefilterMap;
    
//    GORK::CreatePrefilterMap(prefilterMap, envCubemap, captureFBO, captureRBO, captureProjection, captureViews);
    GORK::CreatePrefilterMap(prefilterMap, envCubemap, captureFBO, captureRBO, captureProjection, GORK::CubeCapture());
    
    
    
    
    
    // pbr IBL2 (specular): 根据所使用的BRDF方程生成2D LUT。
    // ----------------------------------------------------
    unsigned int brdfLUTTexture;

    GORK::CreateBrdfLUTTexture(brdfLUTTexture, captureFBO, captureRBO);

    
    

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    // 然后在渲染之前，将视口配置为原始帧缓冲区的屏幕尺寸
    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);





    //========================================================================






    // ------------------------------
    // 渲染循环 render loop
    while (!glfwWindowShouldClose(window)) //每次循环的开始前检查一次GLFW是否被要求退出
    {


        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime(); //获取当前时间
        GORK::deltaTime = currentFrame - GORK::lastFrame; //上一帧时间间隔
        GORK::lastFrame = currentFrame; //保存时间，留在下一帧用


        // 输入 input
        GORK::ProcessInput(window); //输入控制（检查是否按下Esc）


        
        
        
        
        // render
        // -----------------------------------------
        // 第一处理阶段(Pass 1)
        //=========================================================================
//        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);// 绑定到帧缓冲区并绘制场景，就像我们通常用颜色纹理一样
//        glEnable(GL_DEPTH_TEST); // 启用深度测试 ( Pass 2 中渲染屏幕空间四边形时被禁用 )

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //设置清空屏幕后填充的颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清空屏幕的 颜色缓冲 和 深度缓冲
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //清空模版缓冲




        // draw 绘制场景
        // -----------------------------------------



//        // 配置观察/投影矩阵 configure view/projection matrices
        glm::mat4 projection = glm::perspective(glm::radians(GORK::camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //投影矩阵（FOV视锥体竖直张开角度，横向竖直张开角度比，近裁剪面深度，远裁剪面深度）
        glm::mat4 view = GORK::camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        glm::vec3 camPos = GORK::camera.Position;



//===========================================================================================
        // (pbr IBL Diffuse + Specular)渲染场景，将卷积的辐照度图提供给最终的着色器。
        // ------------------------------------------------------------------------------------------
        GORK::ShaderSet(pbrShader, view, camPos, projection);

        // -------------------------------------------
        // IBL1 Diffuse irradiance: bind pre-computed IBL data (在shader中绑定辐照度贴图)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
        // -------------------------------------------

//        //-------------------------------------------
        
        material_gold.BindShaderTexture();
//        material_greasy.BindShaderTexture();
//        material_iron.BindShaderTexture();
//        material_graybricks.BindShaderTexture();
        //-------------------------------------------
        
        pbrShader.setFloat("standardDeviation", standardDeviation);
        
        model = glm::mat4(1.0f);
//        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));//绕x轴转
        model = glm::scale(model, glm::vec3(0.5f));
        pbrShader.setMat4("model", model);
//        GORK::RenderSphere();
        bunny.Draw(pbrShader);//兔子
//        armour.Draw(pbrShader);
//        cabinet.Draw(pbrShader);
//        spoon.Draw(pbrShader);
//        dragon.Draw(pbrShader);
////        //-------------------------------------------
//
//        //-------------------------------------------
//        //模型
//        // 绑定PBR材质贴图（传入shader）
        material_cerberus.BindShaderTexture();
//        //-------------------------------------------

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-7.0, 0.0, 2.0));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));//绕x轴转
        model = glm::rotate(model, glm::radians(120.0f), glm::vec3(0.0, 0.0, 1.0));
        model = glm::scale(model, glm::vec3(0.05f));
//        model = glm::scale(model, glm::vec3(5.0f));
        pbrShader.setMat4("model", model);
        gun.Draw(pbrShader);//渲染枪
       
////-------------------------------------------

        
        GORK::ShaderSet(pbrShader_redBall, view, camPos, projection);
        
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, noiseMap);
        
        

        
        
        GORK::MaterialBalls(pbrShader_redBall, 7, 7, 2.5);
        // 使用材质定义的材质属性渲染行*列数的球体（它们都具有相同的材质属性）

////-------------------------------------------
        
        GORK::SetLights(lightPositions, lightColors, pbrShader, pbrShader_redBall);
        
        //传光源数据。并为了方便用同一个shader画出光源球

////-------------------------------------------


        // 渲染天空盒（最后渲染以防止 overdraw（过度绘制））
        backgroundShader.use();
        backgroundShader.setMat4("view", view);
        backgroundShader.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);//环境贴图
//        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap); // display irradiance map
//        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
        GORK::RenderCube();


//        // 把 LUT 渲染到屏幕 render brdfLUTTexture to screen
//        screenShader.use();
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
//        GORK::RenderQuad();






        //=========================================================================




        glfwSwapBuffers(window); //交换颜色缓冲，用来绘制，并且将会作为输出显示在屏幕上。
        glfwPollEvents(); //检查有没有触发什么事件
    }
    // -------------------------------









    // 终止，释放/删除之前的分配的所有资源
    // -------------------------------
    glfwTerminate(); //（Terminate终止） 释放/删除之前的分配的所有资源
    return 0;
}


