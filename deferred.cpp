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
    
    
    
    //========================================================================
    // 配置 g-buffer 帧缓冲区 configure g-buffer framebuffer
    // ------------------------------
    unsigned int gBuffer;
    glGenFramebuffers(1, &gBuffer); //创建一个帧缓冲对象
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer); //将它绑定为激活的(Active)帧缓冲，做一些操作，之后解绑帧缓冲。
    unsigned int gPosition, gNormal, gAlbedoSpec; //三张纹理附件
    // 位置颜色缓冲 position color buffer
    GORK::TextureAttachments(gPosition, 0);
    
    // 法线颜色缓冲 normal color buffer
    GORK::TextureAttachments(gNormal, 1);
    
    // 漫反射颜色+镜面反射 颜色缓冲 color + specular color buffer
    GORK::TextureAttachments(gAlbedoSpec, 2);

    // 告诉OpenGL我们将使用（此帧缓冲区的）哪些颜色附件进行渲染tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };//传递多个颜色纹理附件的枚举
    glDrawBuffers(3, attachments);//当渲染到这个帧缓冲中的时候，每当着色器使用location(=0或=1或=2)，那么fragment就会用相应的颜色缓冲就会被用来渲染。
    
    
    // 深度缓冲：渲染缓冲对象（只写） create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;//写入深度用于测试
    GORK::RenderBufferObject(rboDepth);
    
    
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    
    // 照明信息 lighting info
    // -------------
    const unsigned int NR_LIGHTS = 20;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    
    GORK::SetDeferredLightInfo(NR_LIGHTS, lightPositions, lightColors);
    //========================================================================
    

    
    
    
    
    
    
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
    //Deferred Shading
    Shader shader_GeometryPass("/书/OGL_Test/Shader/G_buffer.vs", "/书/OGL_Test/Shader/G_buffer.fs");
    Shader shader_LightingPass("/书/OGL_Test/Shader/deferred_shading.vs", "/书/OGL_Test/Shader/deferred_shading.fs");
    Shader shader_LightBox("/书/OGL_Test/Shader/deferred_light_box.vs", "/书/OGL_Test/Shader/deferred_light_box.fs");  
    
    
    //模型
    Shader modelShader("/书/OGL_Test/Shader/model.vs", "/书/OGL_Test/Shader/model.fs");
    Model nanosuit("/书/OGL_Test/nanosuit/nanosuit.obj");
    
    std::vector<glm::vec3> objectPositions = GORK::DeferredObjectPositions();
    
    // ------------------------------------------------------------------------------
    
    
    
    
    
    
    
    
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
        
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //设置清空屏幕后填充的颜色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清空屏幕的 颜色缓冲 和 深度缓冲
        
        
        // draw 绘制场景
        // -----------------------------------------
        //        // 配置观察/投影矩阵 configure view/projection matrices
        glm::mat4 projection = glm::perspective(glm::radians(GORK::camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); //投影矩阵（FOV视锥体竖直张开角度，横向竖直张开角度比，近裁剪面深度，远裁剪面深度）
        glm::mat4 view = GORK::camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        
        glm::vec3 camPos = GORK::camera.Position;
        
        //===========================================================================================
        // 1. 几何处理阶段：将场景的几何/颜色数据渲染到gbuffer中
        // geometry pass: render scene's geometry/color data into gbuffer
        // -----------------------------------------------------------------
        
        GORK::gBufferBind(gBuffer);//绑定gBuffer
        // --------
        
        GORK::ShaderSet(shader_GeometryPass, view, camPos, projection);//设置shader
        GORK::DrawNanosuits(objectPositions, shader_GeometryPass, nanosuit);//绘制模型，将数据渲染到gPosition, gNormal, gAlbedoSpec三张gBuffer纹理附件上
        
        // --------
        GORK::gBufferUnBind();//解绑gBuffer
        
        
        
        // 2. 光照处理阶段：使用G缓冲计算场景的光照
        //lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
        // ----------------------------------------------------------------------------------------------
        GORK::ShaderBindTextures_gBuffer(shader_LightingPass, gPosition, gNormal, gAlbedoSpec);
        
        // 传递光的相关参数 send light relevant uniforms
        GORK::ShaderSetLights_deferred(shader_LightingPass, lightPositions, lightColors);
        
        // 渲染四边形填充屏幕(模型光照场景) finally render quad
        GORK::RenderQuad();
        
        
        
        // 2.5.将几何图形的深度缓冲区的内容复制到默认帧缓冲区的深度缓冲区
        // copy content of geometry's depth buffer to default framebuffer's depth buffer
        // ----------------------------------------------------------------------------------
        GORK::SetDepthBuffer_gBuffer(gBuffer);
        
        
        
        // 3. 渲染光源 render lights on top of scene
        // --------------------------------
        GORK::ShaderSet(shader_LightBox, view, camPos, projection);
        
        GORK::DrawLights_deferred(shader_LightBox, lightPositions, lightColors);
        
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
