#pragma once

#include <Qt3DCore>
#include <Qt3DExtras>
#include <Qt3DRender>
#include <QtCore>
#include <QtWidgets>

#include "common/CamPoint.hpp"
#include "common/HBoxLayout.hpp"
#include "common/Resources.hpp"
#include "common/VBoxLayout.hpp"
#include "common/settings/Settings.hpp"

class CoordsVis : public QWidget
{
    Q_OBJECT

public:
    CoordsVis(QWidget *parent, Settings *const settings,
              Resources *const resources);
    ~CoordsVis();

    void updatePoints(const QList<CamPoint> &pointsList);
    void updateActive(const unsigned int &activeIdx);

private:
    void initPathGeometry();

    Settings *const settings_;
    Resources *const resources_;

    Qt3DCore::QEntity *rootEntity_;
    Qt3DRender::QCamera *camera_;

    Qt3DRender::QAttribute *posAttr_;
    Qt3DRender::QAttribute *idxAttr_;
    Qt3DRender::QAttribute *colorAttr_;
    Qt3DRender::QAttribute *pSizeAttr_;

    unsigned int lastActiveIdx_{};
};
