#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;


// ----------------------------------------------------------------------------
uniform sampler2D noiseMap;
uniform sampler2D roughnessMap;

//uniform float scale; //细，粗粒表面一个片元的微法线m的数量之比（微表面数量之比）
//uniform float average; //正态分布平均数
//uniform float standardDeviation; //标准差
//
//uniform float time;


// ----------------------------------------------------------------------------



// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
//vec3 getNormalFromMap()
//{
//    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;
//    
//    vec3 Q1  = dFdx(WorldPos);
//    vec3 Q2  = dFdy(WorldPos);
//    vec2 st1 = dFdx(TexCoords);
//    vec2 st2 = dFdy(TexCoords);
//    
//    vec3 N   = normalize(Normal);
//    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
//    vec3 B  = -normalize(cross(N, T));
//    mat3 TBN = mat3(T, B, N);
//    
//    return normalize(TBN * tangentNormal);
//}
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / denom;
    
    
}
//// ----------------------------------------------------------------------------
//粗粒分布转换
float P(float k, float GGX)
{
    //    float cc = C(50,k);

    //===========
    //排列组合
    float n = 10.0;
    float m = k;
    //分子
    float son = 1.0;
    //分母
    float mother = 1.0;
    // 应用组合数的互补率简化计算量
    m = m > (n - m) ? (n - m) : m;

    if(m > 0)
    {
        for(float i = m;i > 0.0; i--) {
            son *= n;
            mother *= i;
            n--;
        }
    }


    float C = son / mother;
    //===========

//    C = 1;

    float p = C * pow((1.0 - GGX), 10.0-k) * pow(GGX, k);


    return p;
}
float NDF_Transform(float GGX)
{
    if (GGX > 1.0)
        GGX = 1.0;

    float CDF = 0.0;
    float inverseCDF = 10.0;

    float Xi = texture(noiseMap, TexCoords).r;
//    Xi = 0.5;

    for (float i = 0.0; i < 10.0; i++) {
        CDF += P(i, GGX);

        if (Xi <= CDF) {
            inverseCDF = i;
            break;
        }
    }

    float D_real = inverseCDF / 10.0; //一个粗粒片元里m=H的概率

    return D_real*100;
}
//// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------




void main()
{
    



    vec3 N = normalize(Normal);




    vec3 V = normalize(camPos - WorldPos);
    vec3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 4; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
//        texture(normalMap, TexCoords);
        
        
        float NDF = DistributionGGX(N, H, roughness);
//        NDF = NDF_Transform(NDF);
  
        
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }






    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;
//    vec3 prefilteredColor = textureLod(prefilterMap, R,  1).rgb;

    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F0 * brdf.x + brdf.y);
//    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);//用F代替F0，为什么？？？

    vec3 ambient = (kD * diffuse + specular) * ao;//GGX镜面IBL
//    vec3 ambient = kD * diffuse * ao;//GGX漫反射IBL

    
    vec3 color = ambient + Lo;
//    vec3 color = Lo;//无环境光


    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color , 1.0);//输出颜色



    //==========================
//    //输出NDF
//    float NDF = DistributionGGX(N, V, roughness);
//    NDF = NDF_Transform(NDF);
//    FragColor = vec4(NDF,NDF,NDF, 1.0);
    //==========================

     //   FragColor = vec4(texture(noiseMap, TexCoords).rrr , 1.0);
}
