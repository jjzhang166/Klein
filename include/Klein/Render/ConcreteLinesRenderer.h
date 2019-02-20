#ifndef KLEIN_CONCRETELINESRENDERER_H
#define KLEIN_CONCRETELINESRENDERER_H

#include <Klein/Core/Export.h>

#include <QColor>
#include <QVector>
#include <QVector3D>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QCylinderGeometry>

namespace Qt3DRender
{
class QAttribute;
class QBuffer;
} // namespace Qt3DRender

namespace Klein
{

class KLEIN_API ConcreteLinesRenderer : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT

public:
    enum RequiredBuffers
    {
        BASIC,
        WITH_COLOR
    };

    enum LineType
    {
        LINES,
        LINE_STRIP,
        LINE_LOOP
    };

public:
    ConcreteLinesRenderer(RequiredBuffers requiredBuffers = BASIC,
                          Qt3DCore::QNode* parent = nullptr);

    virtual ~ConcreteLinesRenderer() = default;

public slots:
    void setRadius(float radius) { m_cylinder->setRadius(radius); }

    void setPositions(const QVector<QVector3D>& positions,
                      LineType type = LINES);

    void setColors(const QVector<QColor>& colors, LineType type = LINES);

private:
    Qt3DExtras::QCylinderGeometry* m_cylinder;
    Qt3DRender::QBuffer* m_modelBuffer;
    Qt3DRender::QBuffer* m_colorBuffer;
    Qt3DRender::QAttribute* m_instanceModel;
    Qt3DRender::QAttribute* m_instanceColor;
};

} // namespace Klein

#endif
