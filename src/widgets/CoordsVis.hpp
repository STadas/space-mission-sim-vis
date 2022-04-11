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

/**
 * 3D coordinates visualization class
 */
class CoordsVis : public QWidget
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Parent widget
     * @param settings Settings object
     * @param resources Resources object
     */
    CoordsVis(QWidget *parent, Settings *const settings,
              Resources *const resources);
    /**
     * Destructor
     */
    ~CoordsVis();

    /**
     * Update the points being drawn in the visualization
     * @param pointsList List of camera points to update to
     */
    void updatePoints(const QList<CamPoint> &pointsList);
    /**
     * Update the currently active point in the visualization
     * @param activeIdx The index to set as the active point
     */
    void updateActive(const unsigned int &activeIdx);

private:
    /**
     * Initialize camera controls
     */
    void initControls();
    /**
     * Initialize surface plane
     */
    void initPlane();
    /**
     * Initialize points and lines of the flight path
     */
    void initPathGeometry();

    Settings *const settings_;
    Resources *const resources_;

    Qt3DCore::QEntity *rootEntity_;
    Qt3DRender::QCamera *camera_;
    Qt3DExtras::QOrbitCameraController *camController_;

    Qt3DCore::QTransform *planeTransform_;

    Qt3DRender::QAttribute *posAttr_;
    Qt3DRender::QAttribute *idxAttr_;
    Qt3DRender::QAttribute *colorAttr_;
    Qt3DRender::QAttribute *pSizeAttr_;

    unsigned int lastActiveIdx_{};
};
