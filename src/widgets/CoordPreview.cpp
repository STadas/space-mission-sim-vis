#include "CoordPreview.hpp"

CoordPreview::CoordPreview(QWidget *parent)
    : QWidget(parent)
{
    QBoxLayout *layout = new VBoxLayout(this);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    Qt3DExtras::Qt3DWindow *view3d = new Qt3DExtras::Qt3DWindow();
    QWidget *container = QWidget::createWindowContainer(view3d, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->layout()->addWidget(container);

    // root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();
    view3d->setRootEntity(rootEntity);

    // background
    //TODO: setting
    view3d->defaultFrameGraph()->setClearColor(QColor(QColor("#000000")));

    // camera
    Qt3DRender::QCamera *camera = view3d->camera();
    //TODO: setting
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f,
                                             10000.0f);
    camera->setPosition(QVector3D(0, 0, 20.0f));
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // camera controls
    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera);

    // material stuff
    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    //TODO: setting
    material->setAmbient(QColor("#ffffff"));
    material->setDiffuse(QColor("#ffffff"));

    // point size
    auto effect = material->effect();
    for (auto t : effect->techniques())
    {
        for (auto rp : t->renderPasses())
        {
            auto pointSize = new Qt3DRender::QPointSize();
            pointSize->setSizeMode(Qt3DRender::QPointSize::SizeMode::Fixed);
            pointSize->setValue(8.0f);
            rp->addRenderState(pointSize);
        }
    }

    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(rootEntity);
    Qt3DRender::QBuffer *vertBuffer = new Qt3DRender::QBuffer(geometry);

    this->posAttr_ = new Qt3DRender::QAttribute(geometry);
    this->posAttr_->setName(
        Qt3DRender::QAttribute::defaultPositionAttributeName());
    this->posAttr_->setByteStride(3 * sizeof(float));
    this->posAttr_->setVertexSize(3);
    this->posAttr_->setVertexBaseType(Qt3DRender::QAttribute::Float);
    this->posAttr_->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    this->posAttr_->setBuffer(vertBuffer);
    geometry->addAttribute(this->posAttr_);

    Qt3DRender::QBuffer *idxBuffer = new Qt3DRender::QBuffer(geometry);
    this->idxAttr_ = new Qt3DRender::QAttribute(geometry);
    this->idxAttr_->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    this->idxAttr_->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    this->idxAttr_->setBuffer(idxBuffer);
    geometry->addAttribute(this->idxAttr_);

    auto *lineMesh = new Qt3DRender::QGeometryRenderer(rootEntity);
    lineMesh->setGeometry(geometry);
    lineMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *lineEntity = new Qt3DCore::QEntity(rootEntity);
    lineEntity->addComponent(lineMesh);
    lineEntity->addComponent(material);

    //TODO: have separate, simpler index array for points
    auto *pointsMesh = new Qt3DRender::QGeometryRenderer(rootEntity);
    pointsMesh->setGeometry(geometry);
    pointsMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);
    auto *pointsEntity = new Qt3DCore::QEntity(rootEntity);
    pointsEntity->addComponent(pointsMesh);
    pointsEntity->addComponent(material);

    HBoxLayout *hLayout = new HBoxLayout();
    layout->addLayout(hLayout);

    QPushButton *testButton = new QPushButton("Toggle shape", this);
    testButton->setCheckable(true);
    testButton->setChecked(false);
    QObject::connect(testButton, &QPushButton::toggled, this, [=](bool on) {
        if (on)
        {
            this->updatePoints({
                {-1, -1, -1},
                {-1, -1, 1},
                {1, -1, 1},
                {1, -1, -1},
                {-1, 1, -1},
                {-1, 1, 1},
                {1, 1, 1},
                {1, 1, -1},
            });
        }
        else
        {
            this->updatePoints({});
        }
    });

    hLayout->addWidget(testButton);
}

CoordPreview::~CoordPreview()
{
}

void CoordPreview::updatePoints(const QList<QVector3D> &pointsList)
{
    QByteArray vertBytes;
    vertBytes.resize(pointsList.size() * 3 * sizeof(float));

    float *positions = reinterpret_cast<float *>(vertBytes.data());
    for (auto &vert : pointsList)
    {
        *positions++ = vert.x();
        *positions++ = vert.y();
        *positions++ = vert.z();
    }
    this->posAttr_->buffer()->setData(vertBytes);
    this->posAttr_->setCount(pointsList.size());

    // indices
    QByteArray idxBytes;
    unsigned int vertCount = std::max(pointsList.size() - 1, 0);
    unsigned int idxCount = vertCount * 2;
    idxBytes.resize(idxCount * sizeof(unsigned int));

    unsigned int *indices = reinterpret_cast<unsigned int *>(idxBytes.data());

    unsigned int xd = 0;
    for (unsigned int i = 0; i < vertCount; ++i)
    {
        *indices++ = i;
        *indices++ = i + 1;
        xd += 2;
    }

    this->idxAttr_->buffer()->setData(idxBytes);
    this->idxAttr_->setCount(idxCount);
}
