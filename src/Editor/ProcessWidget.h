#pragma once

#include "Core/GeomIO.h"
#include "Core/Scene.h"
#include "Core/SceneNode.h"

#include <QtWidgets>
#include <QOpenGLWidget>

class ProcessWidget : public QWidget
{
	Q_OBJECT

public:
	ProcessWidget(QWidget* parent, QOpenGLWidget* glWidget);
	virtual ~ProcessWidget();

public Q_SLOTS:
	virtual void onImport(GeomInfo* info) = 0; // Clear all states and info
	void onInitializeScene(Scene* scene);

protected:
	QOpenGLWidget* _glWidget;
	Scene* _scene;
};
