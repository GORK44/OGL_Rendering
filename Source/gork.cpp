//
//  gork.cpp
//  OpenGLtest1
//
//  Created by apple on 2019/12/8.
//  Copyright © 2019 apple. All rights reserved.
//

#include "gork.hpp"
#include "stb_image.h"


////HDR
//bool GORK::hdr = true;
//bool GORK::hdrKeyPressed = false;
//float GORK::exposure = 1.0f;

//bloom
bool GORK::bloom = Effect.BLOOM;  //bloom
bool GORK::bloomKeyPressed = false;





float GORK::deltaTime = 0.0f;
float GORK::lastFrame = 0.0f;
float GORK::lastX = SCR_WIDTH / 2.0f;
float GORK::lastY = SCR_HEIGHT / 2.0f;
bool GORK::firstMouse = true;




// 在每次窗口大小被调整的时候被调用 glfw: whenever the window size changed
// --------------------------------------
void GORK::Framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); //1,2参数控制窗口左下角的位置。3,4控制渲染窗口的宽度和高度（可以比GLFW的维度小）
}

// 在GLFW中实现一些输入控制：WSAD 和 esc
// --------------------------------------
void GORK::ProcessInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //检查用户是否按下了返回键(Esc)
    glfwSetWindowShouldClose(window, true); //GLFW被要求退出（渲染循环时检查）

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);


    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !bloomKeyPressed)
    {
        bloom = !bloom;
        bloomKeyPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
    {
        bloomKeyPressed = false;
    }

//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//    {
//        if (exposure > 0.0f)
//        exposure -= 0.001f;
//        else
//        exposure = 0.0f;
//    }
//    else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//    {
//        exposure += 0.001f;
//    }
    
//    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !nb)
//    {
////        bloom = !bloom;
//        nn+=0.01;
//        nb = true;
//    }
//    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
//    {
//        nb = false;
//    }
//    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !nb)
//    {
//        //        bloom = !bloom;
//        nn-=0.01;
//        nb = true;
//    }
//    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE)
//    {
//        nb = false;
//    }
}


// 鼠标移动 glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void GORK::Mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

    camera.ProcessMouseMovement(xoffset * 3, yoffset * 3);
}

// 鼠标滑轮 glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void GORK::Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


GLFWwindow* GORK::InitWindow(){
    // ----------------------------------
    // 初始化和配置 glfw: initialize and configure
    glfwInit(); //初始化GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //使用 glfwWindowHint 函数来配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // //适配 OS X 的编译
#endif
    // ----------------------------------
//    // 创建一个窗口对象 glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL); //创建窗口
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); //（Terminate终止） 释放/删除之前的分配的所有资源
        return 0;
    }
    glfwMakeContextCurrent(window); //将创建的窗口设置为当前窗口（通知GLFW将我们窗口的上下文设置为当前线程的主上下文）
    glfwSetFramebufferSizeCallback(window, GORK::Framebuffer_size_callback); //每当窗口调整大小的时候调用framebuffer_size_callback

    // ----------------------------------
//    // 鼠标控制
//    glfwSetCursorPosCallback(window, GORK::Mouse_callback);//鼠标一移动mouse_callback函数就会被调用
//    glfwSetScrollCallback(window, GORK::Scroll_callback);//滚轮一滚动scroll_callback函数就会被调用
//
//    // 捕捉鼠标 tell GLFW to capture our mouse
//    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
    // ----------------------------------
    // 加载所有OpenGL函数指针 glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 0;
    }
    // ----------------------------------

    return window;
}




//加载纹理贴图
unsigned int GORK::LoadTexture(char const * pathFolder, char const * fileName, char const * suffix, bool isFlipped, bool isRepeated, bool gammaCorrection, TextureFiltering toSmall)//pathFolder文件夹路径，fileName文件名，suffix后缀
{
    char path[100];
    std::strcpy(path, pathFolder);
    std::strcat(path, fileName);
    std::strcat(path, suffix);


    unsigned int textureID;
    glGenTextures(1, &textureID); //生成 1 个纹理，保存ID到textureID

    int width, height, nrComponents;

    if(isFlipped)
        stbi_set_flip_vertically_on_load(true); // 让stb_image.h在加载图片时翻转y轴

    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0); //加载图片，得到长宽颜色通道信息
    if (data)
    {
        GLenum internalFormat;
        GLenum dataFormat;
        if (nrComponents == 1)
        {
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID); // 绑定纹理，接下来所有GL_TEXTURE_2D操作都是对此纹理
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data); //生成一个纹理
        //参数：纹理目标GL_TEXTURE_2D，Mipmap级别0，纹理存储为RGB格式，宽度，高度，历史遗留总是0，使用RGB值加载，储存为char(byte)数组，图像数据
        glGenerateMipmap(GL_TEXTURE_2D); //自动生成所有需要的多级渐远纹理（Mipmap）

        // 设置纹理环绕方式参数 set the texture wrapping parameters
        if(isRepeated){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }else{
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//超出的部分会重复纹理坐标的边缘
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        switch (toSmall) {
            case linear:
            // 设置纹理过滤参数 set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//纹理被缩小的时候使用Mipmap
            break;
            case mipmap:
            // 设置纹理过滤参数 set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//纹理被缩小的时候使用Mipmap
            break;
            case nearest:
            // 设置纹理过滤参数 set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//纹理被缩小的时候使用Mipmap
            break;
            
            default:
            break;
        }
        // 设置纹理过滤参数 set texture filtering parameters
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//纹理被缩小的时候使用Mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//纹理被放大的时候使用线性过滤

        stbi_image_free(data); //生成了纹理和相应的Mipmap后，释放图像的内存
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;

}






// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)// -X (left)// +Y (top)// -Y (bottom)// +Z (front)// -Z (back)
//加载立方体贴图
// -------------------------------------------------------
unsigned int GORK::LoadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    stbi_set_flip_vertically_on_load(false); // 让stb_image.h在加载图片时翻转y轴

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}


// renders (and builds at first invocation) a sphere
// -------------------------------------------------
unsigned int sphereVAO = 0;
unsigned int indexCount;
void GORK::RenderSphere()
{
    if (sphereVAO == 0)
    {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;

        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
//        const float PI = 3.14159265359;
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                float xSegment = (float)x / (float)X_SEGMENTS;
                float ySegment = (float)y / (float)Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                positions.push_back(glm::vec3(xPos, yPos, zPos));
                normals.push_back(glm::vec3(xPos, yPos, zPos));
                uv.push_back(glm::vec2(xSegment, ySegment));
            }
        }

        bool oddRow = false;
        for (int y = 0; y < Y_SEGMENTS; ++y)
        {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                for (int x = 0; x <= X_SEGMENTS; ++x)
                {
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            }
            else
            {
                for (int x = X_SEGMENTS; x >= 0; --x)
                {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y       * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }
        indexCount = indices.size();

        std::vector<float> data;
        for (int i = 0; i < positions.size(); ++i)
        {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0)
            {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0)
            {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        float stride = (3 + 3 + 2) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    }

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}


// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void GORK::RenderCube()
{
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
            1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
            1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
            1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}



// (屏幕缓冲)renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void GORK::RenderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}




void GORK::SetupFramebuffer(unsigned int &captureFBO, unsigned int &captureRBO)
{
    glGenFramebuffers(1, &captureFBO);//帧缓冲
    glGenRenderbuffers(1, &captureRBO);// 渲染缓冲对象。我们需要 深度 用于测试，但不需要对它们进行采样，所以渲染缓冲对象（只写）非常适合它们
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);//绑定帧缓冲对象
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);//绑定渲染缓冲对象
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);//24位深度
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);// 把rbo附加到帧缓冲的深度附件上
}



void GORK::HdrToCubemap(unsigned int &envCubemap, glm::mat4 &captureProjection, glm::mat4 * captureViews)
{
    
    
    Shader equirectangularToCubemapShader("/书/OGL_Test/Shader/cubemap.vs", "/书/OGL_Test/Shader/equirectangular_to_cubemap.fs");
    
    
    
    // pbr: setup framebuffer 设定帧缓冲（记录HDR转换为立方体的六个面）
    // ----------------------
    unsigned int captureFBO;
    unsigned int captureRBO;
    //    glGenFramebuffers(1, &captureFBO);//帧缓冲
    //    glGenRenderbuffers(1, &captureRBO);// 渲染缓冲对象。我们需要 深度 用于测试，但不需要对它们进行采样，所以渲染缓冲对象（只写）非常适合它们
    //
    //    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);//绑定帧缓冲对象
    //    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);//绑定渲染缓冲对象
    //    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);//24位深度
    //    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);// 把rbo附加到帧缓冲的深度附件上
    GORK::SetupFramebuffer(captureFBO,captureRBO);
    
    
    
    
    // pbr: 加载辐射率 HDR 图像 load the HDR environment map
    // ---------------------------------
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    //    float *data = stbi_loadf("/书/OGL_Test/环境/Arches_E_PineTree/Arches_E_PineTree_3k.hdr", &width, &height, &nrComponents, 0);
    //    float *data = stbi_loadf("/书/OGL_Test/环境/snow_machine/test8_Ref.hdr", &width, &height, &nrComponents, 0);
    float *data = stbi_loadf("/书/OGL_Test/环境/colorful_studio_8k.hdr", &width, &height, &nrComponents, 0);
    unsigned int hdrTexture = 0;
    if (data)
    {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load HDR image." << std::endl;
    }
    
    // pbr: 设置立方体贴图以渲染并附加到帧缓冲区 setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    //    unsigned int envCubemap; // 帧缓冲的纹理附件（生成相应的立方体贴图，为其六个面预先分配内存）
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);//生成一个纹理
        //        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);//生成一个纹理
        //参数：纹理目标GL_TEXTURE_CUBE_MAP_POSITIVE_X + i，Mipmap级别0，纹理存储为RGB格式，宽度，高度，历史遗留总是0，使用RGB值加载，储存为float浮点数，图像数据（不初始化）
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 启用预过滤器Mipmap采样（与可见点伪像作战）enable pre-filter mipmap sampling (combatting visible dots artifact)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    
    
//    // pbr: 设置投影和观察矩阵，以将数据捕获到6个立方图面方向上
//    // ----------------------------------------------------------------------------------------------
//    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);//投影矩阵（FOV视锥体竖直张开角度，横向竖直张开角度比，近裁剪面深度，远裁剪面深度）
//    glm::mat4 captureViews[] =
//    {
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), //相机位置，相机方向，up竖直向上向量
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
//        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
//    };
    
    // pbr: 将HDR等距柱状投影图转换为立方体贴图
    // ----------------------------------------------------------------------
    equirectangularToCubemapShader.use();
    equirectangularToCubemapShader.setInt("equirectangularMap", 0);
    equirectangularToCubemapShader.setMat4("projection", captureProjection);//设置投影矩阵（FOV视锥体）
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    
    glViewport(0, 0, 512, 512); // 不要忘记将视口viewport配置为捕获尺寸。
    //    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        equirectangularToCubemapShader.setMat4("view", captureViews[i]);//设置观察矩阵（相机位置不变，方向改变）
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);//把纹理envCubemap附加到帧缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        GORK::RenderCube();//从中心点画6次HDR立方体贴图。每次相机方向对准其中一面，画一次相当于保存这一面HDR贴图到帧缓冲。
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    // 然后让OpenGL从第一个mip面生成mipmap（解决可见点伪像）then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}



void GORK::CreateIrradianceMap(unsigned int &irradianceMap, unsigned int &envCubemap, unsigned int &captureFBO, unsigned int &captureRBO, glm::mat4 captureProjection, glm::mat4 captureViews[6])
{
    
    Shader irradianceShader("/书/OGL_Test/Shader/cubemap.vs", "/书/OGL_Test/Shader/irradiance_convolution.fs");//辐照度图卷积
    
    
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
    
    // pbr IBL1 :  通过卷积求解扩散积分以创建辐照度（立方体）贴图。
    // -----------------------------------------------------------------------------
    irradianceShader.use();
    irradianceShader.setInt("environmentMap", 0);
    irradianceShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    
    glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader.setMat4("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);//把纹理irradianceMap附加到帧缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        GORK::RenderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
}



void GORK::CreatePrefilterMap(unsigned int &prefilterMap, unsigned int &envCubemap, unsigned int &captureFBO, unsigned int &captureRBO, glm::mat4 captureProjection, glm::mat4 captureViews[6])
{
    
    Shader prefilterShader("/书/OGL_Test/Shader/cubemap.vs", "/书/OGL_Test/Shader/prefilter.fs");
    
    
    
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // 确保将其缩小过滤器设置为 mip_linear（MipMap）
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 为立方体贴图生成mipmap，以便OpenGL自动分配所需的内存。
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    
    
    // pbr IBL2 (specular): 在hdr环境贴图辐射率采样上运行准蒙特卡洛采样，以创建预滤波（立方体）贴图。
    // ----------------------------------------------------------------------------------------------------
    prefilterShader.use();
    prefilterShader.setInt("environmentMap", 0);
    prefilterShader.setMat4("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // 根据MIP级大小调整帧缓冲区的大小。
        unsigned int mipWidth  = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);
        
        float roughness = (float)mip / (float)(maxMipLevels - 1);//0，1/4，2/4，3/4，1
        prefilterShader.setFloat("roughness", roughness);
        //        prefilterShader.setFloat("roughness", 0.3);
        
        for (unsigned int i = 0; i < 6; ++i)//绘制六次，每次朝向立方体一个面
        {
            prefilterShader.setMat4("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);//把纹理的mipmap附加到帧缓冲
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            GORK::RenderCube();//绘制一个面到帧缓冲（30次：5个mip等级 * 立方体6个面）（5个mip等级不同的立体贴图）
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    

}



void GORK::CreateBrdfLUTTexture(unsigned int &brdfLUTTexture, unsigned int &captureFBO, unsigned int &captureRBO)
{
    
    
    Shader brdfShader("/书/OGL_Test/Shader/brdf.vs", "/书/OGL_Test/Shader/brdf.fs");
    
    
    glGenTextures(1, &brdfLUTTexture);
    
    // 为LUT纹理预先分配足够的内存。（512*512）
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
    // be sure to set wrapping mode to GL_CLAMP_TO_EDGE（防止边缘采样的伪像）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // 然后重新配置帧缓冲区对象captureFBO，并使用BRDF着色器渲染屏幕空间四边形。
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUTTexture, 0);
    
    glViewport(0, 0, 512, 512);
    brdfShader.use();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GORK::RenderQuad();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void GORK::ShaderSet_PBR_Ball(Shader &pbrShader_redBall)
{
    pbrShader_redBall.use();
    pbrShader_redBall.setInt("irradianceMap", 0);
    pbrShader_redBall.setInt("prefilterMap", 1);
    pbrShader_redBall.setInt("brdfLUT", 2);
    pbrShader_redBall.setVec3("albedo", 1.0f, 1.0f, 1.0f);
    pbrShader_redBall.setFloat("ao", 1.0f);
    
    pbrShader_redBall.setInt("noiseMap", 3);
}

void GORK::ShaderSet_PBR_Model(Shader &pbrShader)
{
    pbrShader.use();
    pbrShader.setInt("irradianceMap", 0);// shader中的 irradianceMap 对应 GL_TEXTURE0 绑定的纹理
    pbrShader.setInt("prefilterMap", 1);// 预滤波贴图
    pbrShader.setInt("brdfLUT", 2); // LUT贴图
    pbrShader.setInt("albedoMap", 3); // 反照率（基础颜色）贴图
    pbrShader.setInt("normalMap", 4); // 法线贴图
    pbrShader.setInt("metallicMap", 5); // 金属度贴图
    pbrShader.setInt("roughnessMap", 6); // 粗糙度贴图
    pbrShader.setInt("aoMap", 7); // 环境光遮蔽贴图
    
    pbrShader.setInt("noiseMap", 8);
}


glm::mat4 * GORK::CubeCapture()
{
    //C++ 不支持在函数外返回局部变量的地址，除非定义局部变量为 static 变量
    static glm::mat4 captureViews[] =
    {
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)), //相机位置，相机方向，up竖直向上向量
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    
    
    
    return captureViews;
}

void GORK::SetLights(glm::vec3 *lightPositions, glm::vec3 *lightColors, Shader &pbrShader, Shader &pbrShader_redBall)
{
    //传光源数据。并为了方便用同一个shader画出光源球
    for (unsigned int i = 0; i < sizeof(*lightPositions) / sizeof(lightPositions[0]); ++i)
    {
        i = 3;
    
    
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        newPos = lightPositions[i];
        //            newPos = lightPos;
        pbrShader_redBall.use();
        pbrShader_redBall.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        pbrShader_redBall.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
        pbrShader.use();
        pbrShader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        pbrShader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);
        //
        //            // 画光源球
        //            model = glm::mat4(1.0f);
        //            model = glm::translate(model, newPos);
        //            model = glm::scale(model, glm::vec3(0.5f));
        //            pbrShader_redBall.setMat4("model", model);
        //            GORK::RenderSphere();
    }
}

void GORK::MaterialBalls(Shader &pbrShader_redBall, int nrRows, int nrColumns, float spacing)
{
    
    //        // 使用材质定义的材质属性渲染行*列数的球体（它们都具有相同的材质属性）
    for (int row = 0; row < nrRows; ++row)//行数
    {
        pbrShader_redBall.setFloat("metallic", (float)row / (float)nrRows);//行数从下往上，金属度从低到高
        
        for (int col = 0; col < nrColumns; ++col)//列数
        {
            // 我们将粗糙度限制在0.025-1.0，因为完全光滑的表面（0.0的粗糙度）看起来有点偏离
            // 在直接照明下 on direct lighting.
            pbrShader_redBall.setFloat("roughness", glm::clamp((float)col / (float)nrColumns, 0.0f, 1.0f));//列数从左到右，粗糙度从小到大
            
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                                                    (float)(col - (nrColumns / 2)) * spacing,
                                                    (float)(row - (nrRows / 2)) * spacing,
                                                    -2.0f
                                                    ));
            //                model = glm::translate(model, glm::vec3(
            //                                                        (float)(col - (nrColumns / 2)) * spacing,
            //                                                        (float)(row - (nrRows / 2)) * spacing,
            //                                                        -2.0f
            //                                                        )+ glm::vec3(0.005*sin(glfwGetTime()) * 4.0f));
            pbrShader_redBall.setMat4("model", model);
            GORK::RenderSphere();
            //                bunny.Draw(pbrShader_redBall);
            //                GORK::RenderCube();
        }
        
    }
}

void GORK::ShaderSet(Shader &shader, glm::mat4 &view, glm::vec3 &camPos, glm::mat4 &projection)
{
    shader.use();
    shader.setMat4("view", view);
    shader.setVec3("camPos", camPos);
    shader.setMat4("projection", projection);
}


void GORK::TextureAttachments(unsigned int &gAttachment, int attachmentNumber)
{
    if(attachmentNumber < 2)//对于位置和法向量的纹理，我们希望使用高精度的纹理(每分量16或32位的浮点数)
    {
        // 颜色缓冲  color buffer
        glGenTextures(1, &gAttachment);//生成 1 个纹理，保存ID到textureColorbuffer
        glBindTexture(GL_TEXTURE_2D, gAttachment);// 绑定纹理，接下来所有GL_TEXTURE_2D操作都是对此纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);//生成一个纹理
        //参数：纹理目标GL_TEXTURE_2D，Mipmap级别0，纹理存储为RGB格式，宽度，高度，历史遗留总是0，使用RGB值加载，储存为char(byte)数组，图像数据（不初始化）
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//线性过滤
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, GL_TEXTURE_2D, gAttachment, 0);//把纹理附加到帧缓冲
        //参数：缓冲的目标，附加一个颜色附件，附加的纹理类型，附加的纹理本身，多级渐远纹理的级别0
    }
    else if (attachmentNumber == 2)//而对于反照率和镜面值，使用(RGBA四通道)默认的纹理(每分量8位浮点数)就够了。
    {
        // 漫反射颜色+镜面反射 颜色缓冲 color + specular color buffer
        glGenTextures(1, &gAttachment);
        glBindTexture(GL_TEXTURE_2D, gAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentNumber, GL_TEXTURE_2D, gAttachment, 0);
    }
    
}

void GORK::RenderBufferObject(unsigned int &rboDepth)
{
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
}

void GORK::SetDeferredLightInfo(const unsigned int NR_LIGHTS, std::vector<glm::vec3> &lightPositions, std::vector<glm::vec3> &lightColors)
{
        srand(13);
        for (unsigned int i = 0; i < NR_LIGHTS; i++)
        {
            // 计算轻微的随机偏移 calculate slightly random offsets
            float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
            float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
            lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
            // 还计算随机颜色 also calculate random color
            float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
            lightColors.push_back(glm::vec3(rColor, gColor, bColor));
        }
}

std::vector<glm::vec3> GORK::DeferredObjectPositions()
{
    std::vector<glm::vec3> objectPositions;
    objectPositions.push_back(glm::vec3(-3.0,  -3.0, -3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -3.0, -3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -3.0, -3.0));
    objectPositions.push_back(glm::vec3(-3.0,  -3.0,  0.0));
    objectPositions.push_back(glm::vec3( 0.0,  -3.0,  0.0));
    objectPositions.push_back(glm::vec3( 3.0,  -3.0,  0.0));
    objectPositions.push_back(glm::vec3(-3.0,  -3.0,  3.0));
    objectPositions.push_back(glm::vec3( 0.0,  -3.0,  3.0));
    objectPositions.push_back(glm::vec3( 3.0,  -3.0,  3.0));
    
    return objectPositions;
}

void GORK::gBufferBind(unsigned int gBuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GORK::gBufferUnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, SCR_WIDTH*2, SCR_HEIGHT*2);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GORK::DrawNanosuits(std::vector<glm::vec3> &objectPositions, Shader &shader_GeometryPass, Model &nanosuit)
{
    for (unsigned int i = 0; i < objectPositions.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, objectPositions[i]);
        model = glm::scale(model, glm::vec3(0.25f));
        shader_GeometryPass.setMat4("model", model);
        nanosuit.Draw(shader_GeometryPass);
    }
}

void GORK::ShaderBindTextures_gBuffer(Shader &shader_LightingPass, unsigned int gPosition, unsigned int gNormal, unsigned int gAlbedoSpec)
{
    shader_LightingPass.use();
    shader_LightingPass.setInt("gPosition", 0);
    shader_LightingPass.setInt("gNormal", 1);
    shader_LightingPass.setInt("gAlbedoSpec", 2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
}

void GORK::ShaderSetLights_deferred(Shader &shader_LightingPass, std::vector<glm::vec3> &lightPositions, std::vector<glm::vec3> &lightColors)
{
    // 传递光的相关参数 send light relevant uniforms
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        shader_LightingPass.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        shader_LightingPass.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        // 更新衰减参数并计算半径 update attenuation parameters and calculate radius
        const float constant = 1.0; // 请注意，我们不会将其发送给着色器，我们假设它始终为1.0（在我们的示例中）
        const float linear = 0.7;
        const float quadratic = 1.8;
        shader_LightingPass.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
        shader_LightingPass.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        // 然后计算光体积的半径 then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        shader_LightingPass.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
    }
    shader_LightingPass.setVec3("viewPos", GORK::camera.Position);
}

void GORK::SetDepthBuffer_gBuffer(unsigned int gBuffer)
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // 写入到默认帧缓冲write to default framebuffer
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH*2, SCR_HEIGHT*2, GL_DEPTH_BUFFER_BIT, GL_NEAREST); //mac视网膜屏。对于glBlitFramebuffer，读取和绘制帧缓冲区分别是绑定到GL_READ_FRAMEBUFFER和GL_DRAW_FRAMEBUFFER目标的缓冲区。
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GORK::DrawLights_deferred(Shader &shader_LightBox, std::vector<glm::vec3> &lightPositions, std::vector<glm::vec3> &lightColors)
{
    for (unsigned int i = 0; i < lightPositions.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPositions[i]);
        model = glm::scale(model, glm::vec3(0.125f));
        shader_LightBox.setMat4("model", model);
        shader_LightBox.setVec3("lightColor", lightColors[i]);
        GORK::RenderCube();
    }
}

void GORK::BindTexture_IBL(unsigned int irradianceMap, unsigned int prefilterMap, unsigned int brdfLUTTexture)
{
    // IBL1 Diffuse irradiance: bind pre-computed IBL data (在shader中绑定辐照度贴图)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    //        glActiveTexture(GL_TEXTURE1);
    //        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}


float GORK::Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}


void GORK::TextureAttachments_SSAO(unsigned int ssaoFBO, unsigned int &ssaoColorBuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "SSAO Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
