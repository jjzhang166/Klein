#include <Klein/Gui/AbstractQt3DWindow.h>

#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DCore/QEntity>
#include <Qt3DInput/QInputAspect>
#include <Qt3DInput/QInputSettings>
#include <Qt3DLogic/QLogicAspect>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QRenderSettings>

namespace Klein
{

AbstractQt3DWindow::AbstractQt3DWindow(QWindow* parent) : QWindow(parent)
{
    this->setSurfaceType(QSurface::OpenGLSurface);
    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
#ifdef QT_OPENGL_ES_2
    format.setRenderableType(QSurfaceFormat::OpenGLES);
#else
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        format.setVersion(4, 3);
        format.setProfile(QSurfaceFormat::CoreProfile);
    }
#endif
    format.setDepthBufferSize(24);
    format.setSamples(4);
    format.setStencilBufferSize(8);
    this->setFormat(format);
    QSurfaceFormat::setDefaultFormat(format);

    m_aspectEngine = new Qt3DCore::QAspectEngine(this);
    m_aspectEngine->registerAspect(new Qt3DInput::QInputAspect(m_aspectEngine));
    m_aspectEngine->registerAspect(new Qt3DLogic::QLogicAspect(m_aspectEngine));
    m_aspectEngine->registerAspect(new Qt3DRender::QRenderAspect(m_aspectEngine));
}

void AbstractQt3DWindow::exposeEvent(QExposeEvent*)
{
    if (!m_initialized && isExposed()) {
        auto root = createSceneGraph();
        root->addComponent(createRenderSettings(root));
        root->addComponent(createInputSettings(root));
        m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(root));
        m_initialized = true;
    }
}

Qt3DInput::QInputSettings* AbstractQt3DWindow::createInputSettings(
    Qt3DCore::QEntity* root)
{
    auto settings = new Qt3DInput::QInputSettings(root);
    settings->setEventSource(this);
    return settings;
}

} // namespace Klein
