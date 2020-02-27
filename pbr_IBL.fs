#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;


uniform sampler2D noiseMap;


uniform float standardDeviation; //标准差


// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
// Easy trick to get tangent-normals to world-space to keep PBR code simplified.
// Don't worry if you don't get what's going on; you generally want to do normal
// mapping the usual way for performance anways; I do plan make a note of this
// technique somewhere later in the normal mapping tutorial.
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;
    
    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);
    
    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    
    return normalize(TBN * tangentNormal);
}
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
    
    
    
    //    return exp( (NdotH2 - 1) / (a2 * NdotH2) ) / ( PI * a2 * NdotH2 * NdotH2 );
    
    // ----------------------------------------------------------------------------
    ////各向异性
    //    float NdotH = max(dot(N, H), 0.0);
    
    //    vec3 Q1  = dFdx(WorldPos);
    //    vec3 Q2  = dFdy(WorldPos);
    //    vec2 st1 = dFdx(TexCoords);
    //    vec2 st2 = dFdy(TexCoords);
    //
    //    vec3 X  = normalize(Q1*st2.t - Q2*st1.t);
    //    vec3 Y  = -normalize(cross(N, X));
    //
    //    float HdotX = dot(X, H);
    //    float HdotY = dot(Y, H);
    //    float anisotropic = 0.95;
    //    float aspect = sqrt(1.0 - 0.9 * anisotropic);
    //    float roughnessSqr = roughness * roughness;
    //    float NdotHSqr = NdotH * NdotH;
    //    float ax = roughnessSqr / aspect;
    //    float ay = roughnessSqr * aspect;
    //    float d = HdotX * HdotX / (ax * ax) + HdotY * HdotY / (ay * ay) + NdotHSqr;
    //    return 1 / (PI * ax * ay * d * d);
    // ----------------------------------------------------------------------------
    
    
    
    //粗NDF=GGX*1000*[正态分布函数(Xi)]  //Xi为真随机数
    //GGX*20000(m朝向H概率*细粒片元m总数=细粒片元m朝向H总数=1000个粗粒片元中m朝向H总数）
    //GGX*20000*正态分布函数中一个随机值=随机一个粗粒片元中m朝向H总数
    //GGX*20000*[正态分布函数(Xi)]/20 = 随机一个粗粒片元中m朝向H的概率 = 粗NDF
    //20000/20 是粗细粒表面一个片元的微法线m的数量之比（微表面数量之比）。可以随着摄像机距离调节?
    
    float GGX = nom / denom;
    
    float Xi = 0.5f;
    
    //现实世界细粒度模型
    float o = 1;
    float ss = 10;
    
    
    float u = 0;//平均数=0
    
    float ZhengTai = 1.0/(o * sqrt(2.0 * PI)) * exp(-(Xi-u)*(Xi-u)/(2*o*o));
    
    float CuNDF = GGX * ss * ZhengTai; //要找到标准差和 细,粗粒微面之比的关系
    
    return CuNDF;
    
    
}
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
    // material properties
    vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
    float metallic = texture(metallicMap, TexCoords).r;
    float roughness = texture(roughnessMap, TexCoords).r;
//    roughness = 0.3;
    float ao = texture(aoMap, TexCoords).r;
    
    // input lighting data
    vec3 N = getNormalFromMap();
    
    
    // ----------------------------------------------------------------------------
//    //法线按NDF随机偏置方向（正态分布）
//    uint ii = uint(texture(noiseMap, TexCoords).r * 1024.0f);
//    //    uint ii = uint(gl_FragCoord.x) * uint(gl_FragCoord.y) % 1024u;
//    
//    //    uint ii = uint(fract(gl_FragCoord.x*100000.0)) % 1024u;
//    //    uint ii = uint(pow(gl_FragCoord.x,gl_FragCoord.y)) % 64u;
//    //    ii = ii % 1024u;
//    //    uint ii = 0u;
//    vec2 jj = Hammersley(ii, 1024u);
//    N = ImportanceSampleGGX(jj, N, 0.2);
    // ----------------------------------------------------------------------------
    
    
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
        float NDF = DistributionGGX(N, H, roughness);
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
//    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness < 0.3 ? roughness : (roughness/6) * MAX_REFLECTION_LOD).rgb;
//        vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness > 0.1 ? roughness : 0 * MAX_REFLECTION_LOD).rgb;
//        vec3 prefilteredColor = textureLod(prefilterMap, R,  0).rgb;

    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F0 * brdf.x + brdf.y);
    
    vec3 ambient = (kD * diffuse + specular) * ao;
//    vec3 ambient = (kD * diffuse) * ao;

    
    vec3 color = ambient + Lo;
    
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color , 1.0);
}
