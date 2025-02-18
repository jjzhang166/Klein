#include <Klein/Render/PBRInstancedMaterial.h>

#include <Klein/Render/ResourceManager.h>
#include <Klein/Render/ShaderProgram.h>
#include <QString>
#include <Qt3DRender/QEffect>

namespace Klein
{

PBRInstancedMaterial::PBRInstancedMaterial(Qt3DCore::QNode* parent)
    : BasePBRMaterial(parent)
{
    m_useInstanceColor = new Qt3DRender::QParameter(
        QStringLiteral("useInstanceColor"), false, this);
    this->addParameter(m_useInstanceColor);
    this->setEffect(getEffectVariant(isShadowCastingEnabled()));
}

Qt3DRender::QEffect* PBRInstancedMaterial::createEffect(bool castShadow)
{
    const QString shaderPath("data/shader/");
    const auto shader =
        createShader(shaderPath + QStringLiteral("ShadingInstanced.vert"),
                     shaderPath + QStringLiteral("PBR.frag"));
    return createEffectImpl(shader, castShadow);
}

Qt3DRender::QEffect* PBRInstancedMaterial::getEffectVariant(bool castShadow)
{
    Qt3DRender::QEffect* effect = nullptr;
    if (castShadow) {
        effect =
            gResourceManager().get<Qt3DRender::QEffect>(effectName_CastShadow);
        if (effect == nullptr) {
            effect = createEffect(castShadow);
            gResourceManager().put(effectName_CastShadow, effect);
        }
    }
    else {
        effect =
            gResourceManager().get<Qt3DRender::QEffect>(effectName_NoShadow);
        if (effect == nullptr) {
            effect = createEffect(castShadow);
            gResourceManager().put(effectName_NoShadow, effect);
        }
    }
    return effect;
}

} // namespace Klein
