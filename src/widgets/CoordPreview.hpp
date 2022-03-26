#pragma once

#include <QtCore>
#include <QtWidgets>

#include <Qt3DCore/qentity.h>
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qcamera.h>
#include <Qt3DRender/qcameralens.h>

#include <Qt3DExtras/qphongmaterial.h>
#include <Qt3DExtras/qtorusmesh.h>
/* #include <Qt3DExtras/qpervertexcolormaterial.h> */
#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qeffect.h>
#include <Qt3DRender/qmaterial.h>
#include <Qt3DRender/qmesh.h>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DRender/qpointsize.h>
#include <Qt3DRender/qrenderpass.h>
#include <Qt3DRender/qsceneloader.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/qtexture.h>

#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/qforwardrenderer.h>
#include <Qt3DRender/qrenderaspect.h>

#include <Qt3DExtras/qorbitcameracontroller.h>
#include <Qt3DExtras/qt3dwindow.h>

#include "common/HBoxLayout.hpp"
#include "common/VBoxLayout.hpp"

class CoordPreview : public QWidget
{
    Q_OBJECT

public:
    CoordPreview(QWidget *parent);
    ~CoordPreview();

    void updatePoints(const QList<QVector3D> &pointsList);

private:
    Qt3DRender::QAttribute *posAttr_;
    Qt3DRender::QAttribute *idxAttr_;
};
