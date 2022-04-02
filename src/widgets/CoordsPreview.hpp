#pragma once

#include <QtCore>
#include <QtWidgets>

#include <Qt3DCore/QEntity>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QCamera>

#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QGraphicsApiFilter>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QPointSize>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DRender/QTechnique>

#include <Qt3DCore/QTransform>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DRender/QRenderAspect>

#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/Qt3DWindow>

#include "common/CamPoint.hpp"
#include "common/HBoxLayout.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"

class CoordsPreview : public QWidget
{
    Q_OBJECT

public:
    CoordsPreview(QWidget *parent, Settings *const settings);
    ~CoordsPreview();

    void updatePoints(const QList<CamPoint> &pointsList);
    void updateActive(const unsigned int &activeIdx);

private:
    Settings *const settings_;

    Qt3DRender::QAttribute *posAttr_;
    Qt3DRender::QAttribute *idxAttr_;
    Qt3DRender::QAttribute *colorAttr_;
    Qt3DRender::QAttribute *pSizeAttr_;

    unsigned int lastActiveIdx_{};
};
