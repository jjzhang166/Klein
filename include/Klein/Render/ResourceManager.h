#ifndef KLEIN_RESOURCEMANAGER_H
#define KLEIN_RESOURCEMANAGER_H

#include <Klein/Core/Export.h>

#include <QHash>
#include <QString>
#include <QUrl>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QShaderProgram>

namespace Klein
{

enum BuiltinResource : int
{
    BUILTIN_SHADER_PBR,
    BUILTIN_SHADER_PBRSOLIDWIREFRAME,
    BUILTIN_SHADER_PBRINSTANCED,
    BUILTIN_SHADER_SHADOWMAP,
};

class KLEIN_API ResourceManager
{
public:
    static ResourceManager& instance()
    {
        static ResourceManager m;
        return m;
    }

public:
    ResourceManager(const ResourceManager&) = delete;

    void operator=(const ResourceManager&) = delete;

    void setRoot(Qt3DCore::QEntity* root) { m_root = root; }

    template<typename T>
    T* get(BuiltinResource bs);

private:
    ResourceManager() {}

private:
    Qt3DCore::QEntity* m_root;
    QHash<BuiltinResource, Qt3DCore::QNode*> m_builtins;
};

inline KLEIN_API ResourceManager& gResourceManager()
{
    return ResourceManager::instance();
}

static inline Qt3DRender::QShaderProgram* createShader(const QString& fvert,
                                                       const QString& ffrag)

{
    auto shader = new Qt3DRender::QShaderProgram();
    shader->setVertexShaderCode(
        Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(fvert)));
    shader->setFragmentShaderCode(
        Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(ffrag)));
    return shader;
}

static inline Qt3DRender::QShaderProgram* createShader(const QString& fvert,
                                                       const QString& fgeom,
                                                       const QString& ffrag)
{
    auto shader = new Qt3DRender::QShaderProgram();
    shader->setVertexShaderCode(
        Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(fvert)));
    shader->setGeometryShaderCode(
        Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(fgeom)));
    shader->setFragmentShaderCode(
        Qt3DRender::QShaderProgram::loadSource(QUrl::fromLocalFile(ffrag)));
    return shader;
}

template<>
inline Qt3DRender::QShaderProgram*
ResourceManager::get<Qt3DRender::QShaderProgram>(BuiltinResource bs)
{
    Qt3DRender::QShaderProgram* shader = nullptr;
    if (!m_builtins.contains(bs)) {
        QString shaderPath("data/shader/");
        switch (bs) {
        case BUILTIN_SHADER_PBR:
            shader = createShader(shaderPath + QStringLiteral("PBR.vert"),
                                  shaderPath + QStringLiteral("PBR.frag"));
            shader->setParent(m_root);
            m_builtins[bs] = shader;
            break;
        case BUILTIN_SHADER_PBRSOLIDWIREFRAME:
            shader = createShader(
                shaderPath + QStringLiteral("PBRSolidWireframe.vert"),
                shaderPath + QStringLiteral("PBRSolidWireframe.geom"),
                shaderPath + QStringLiteral("PBRSolidWireframe.frag"));
            shader->setParent(m_root);
            m_builtins[bs] = shader;
            break;
        case BUILTIN_SHADER_PBRINSTANCED:
            shader =
                createShader(shaderPath + QStringLiteral("PBRInstanced.vert"),
                             shaderPath + QStringLiteral("PBR.frag"));
            shader->setParent(m_root);
            m_builtins[bs] = shader;
            break;
        case BUILTIN_SHADER_SHADOWMAP:
            shader = createShader(
                shaderPath + QStringLiteral("ShadowMappingDepth.vert"),
                shaderPath + QStringLiteral("ShadowMappingDepth.frag"));
            shader->setParent(m_root);
            m_builtins[bs] = shader;
            break;
        default: break;
        }
    }
    else {
        shader = dynamic_cast<Qt3DRender::QShaderProgram*>(m_builtins[bs]);
    }
    return shader;
}

} // namespace Klein

#endif
