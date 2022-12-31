#include "Material.h"

Material::Material(Shader* shader)
    : mShader(shader) { }

Material::~Material()
{
    //this needs to be handled
}

void Material::pushUniform(const std::string& name, const Uniform& uniform)
{
    mUniforms[name] = uniform;
}

void Material::use() const
{
    int texTarget = 0;

    for (const auto& entry : mUniforms)
    {
        const auto& name = entry.first;
        const auto& data = entry.second;
        switch (data.getType())
        {
            case(UniformType::Bool): mShader->setBool(name, data.get<bool>()); break;
            case(UniformType::Int): mShader->setInt(name, data.get<int>()); break;
            case(UniformType::Float): mShader->setFloat(name, data.get<float>()); break;
            case(UniformType::Vec2): mShader->setVec2(name, data.get<glm::vec2>()); break;
            case(UniformType::Vec3): mShader->setVec3(name, data.get<glm::vec3>()); break;
            case(UniformType::Vec4): mShader->setVec4(name, data.get<glm::vec4>()); break;
            case(UniformType::Mat2): mShader->setMat2(name, data.get<glm::mat2>()); break;
            case(UniformType::Mat3): mShader->setMat3(name, data.get<glm::mat3>()); break;
            case(UniformType::Mat4): mShader->setMat4(name, data.get<glm::mat4>()); break;
            case(UniformType::Sampler2D): mShader->setTex(name, data.get<int>(), texTarget++); break;
        }
    }

    mShader->use();
}
