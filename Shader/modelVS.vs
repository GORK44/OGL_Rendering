#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoords;
//
//out vec3 FragPos; //片元在世界空间的坐标
//out vec3 Normal;

out VS_OUT {
    vec3 FragPos; //片元坐标（世界空间）
//    vec2 TexCoords; //uv坐标
//    vec3 TangentLightPos; //光源坐标（切线空间）
    vec3 TangentLightDir; //光照方向（切线空间）
    vec3 TangentViewPos; //观察坐标（切线空间）
    vec3 TangentFragPos; //片元坐标（切线空间）
} vs_out;




//定向光
struct DirLight {
    vec3 direction; // 平行光方向 (从光源出发的全局方向)

    vec3 ambient; // 光源的 环境光 强度（通常比较低）
    vec3 diffuse; // 光源的 漫反射 强度（通常为光的颜色，稍微暗一点搭配场景）
    vec3 specular; // 光源的 镜面反射 强度（通常会保持为vec3(1.0)，以最大强度发光）
};


//uniform vec3 lightDir; //把定向光放到全局
uniform DirLight dirLight; //把定向光放到全局
uniform vec3 viewPos; //相机位置


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0)); //片元坐标（世界空间）
//    vs_out.TexCoords = aTexCoords;//uv坐标
    TexCoords = aTexCoords;
    
    mat3 normalMatrix = transpose(inverse(mat3(model))); //逆转置矩阵（模型矩阵）
    vec3 T = normalize(normalMatrix * aTangent); //切线向量（世界空间）
    vec3 N = normalize(normalMatrix * aNormal); //法向量（世界空间）
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(T, N); //副切线向量（世界空间）
    
    mat3 TBN = transpose(mat3(T, B, N)); //TBN矩阵的逆矩阵（正交矩阵的置换矩阵=逆矩阵）用来把其他相关向量转换到切线空间
    vs_out.TangentLightDir = TBN * dirLight.direction; //光照方向（世界空间->切线空间）
    vs_out.TangentViewPos  = TBN * viewPos; //观察坐标（世界空间->切线空间）
    vs_out.TangentFragPos  = TBN * vs_out.FragPos; //片元坐标（世界空间->切线空间）
    
    
    
    
//    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);

//    Normal = mat3(transpose(inverse(model))) * aNormal;
//    FragPos = vec3(model * vec4(aPos, 1.0)); //片元在世界空间的坐标
}
