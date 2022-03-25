#include "CoordPreview.hpp"

CoordPreview::CoordPreview(QWidget *parent)
    : QWidget(parent)
{
    QLayout *layout = new VBoxLayout(this);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    Qt3DExtras::Qt3DWindow *view3d = new Qt3DExtras::Qt3DWindow();
    QWidget *container = QWidget::createWindowContainer(view3d, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->layout()->addWidget(container);

    // root entity
    Qt3DCore::QEntity *rootEntity = new Qt3DCore::QEntity();
    view3d->setRootEntity(rootEntity);

    // background
    view3d->defaultFrameGraph()->setClearColor(QColor(QColor("#000000")));

    // camera
    Qt3DRender::QCamera *camera= view3d->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20.0f));
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setViewCenter(QVector3D(0, 0, 0));

    // camera controls
    auto *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    camController->setCamera(camera);

    // material stuff
    auto *material = new Qt3DExtras::QPhongMaterial(rootEntity);
    material->setAmbient(QColor("#ffffff"));
    material->setDiffuse(QColor("#ffffff"));

    // point size
    auto effect = material->effect();
    for (auto t : effect->techniques()) {
        for (auto rp : t->renderPasses()) {
            auto pointSize = new Qt3DRender::QPointSize();
            pointSize->setSizeMode(Qt3DRender::QPointSize::SizeMode::Fixed);
            pointSize->setValue(8.0f);
            rp->addRenderState(pointSize);
        }
    }

    //FIXME: put into its own method

    QList<QVector3D> verticesList = {
        {-1, -1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, -1, -1},

        {-1, 1, -1},  {-1, 1, 1},  {1, 1, 1},  {1, 1, -1},
    };
    QByteArray vertBytes;
    vertBytes.resize(verticesList.size() * 3 * sizeof(float));

    float *positions = reinterpret_cast<float *>(vertBytes.data());
    for (auto &vert : verticesList)
    {
        *positions++ = vert.x();
        *positions++ = vert.y();
        *positions++ = vert.z();
    }

    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(rootEntity);
    Qt3DRender::QBuffer *buffer = new Qt3DRender::QBuffer(geometry);
    buffer->setData(vertBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setVertexSize(3);
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buffer);
    positionAttribute->setCount(verticesList.size());
    geometry->addAttribute(positionAttribute);

    QByteArray indexBytes;
    unsigned int idxCount = verticesList.size() * 2 - 2;
    indexBytes.resize(idxCount * sizeof(unsigned int)); // start to end
    unsigned int *indices = reinterpret_cast<unsigned int*>(indexBytes.data());

    /* verticesList.size() = 4 -> indices = {0, 1, 1, 2, 2, 3} */
    for (unsigned int i = 0; i < verticesList.size() - 1; ++i)
    {
        *indices++ = i;
        *indices++ = i + 1;
    }

    //FIXME: figure out how pointers work in this, if we can just change the data and it auto updates

    auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(idxCount);
    geometry->addAttribute(indexAttribute);

    auto *lineMesh = new Qt3DRender::QGeometryRenderer(rootEntity);
    lineMesh->setGeometry(geometry);
    lineMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    auto *pointsMesh = new Qt3DRender::QGeometryRenderer(rootEntity);
    pointsMesh->setGeometry(geometry);
    pointsMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);

    auto *lineEntity = new Qt3DCore::QEntity(rootEntity);
    lineEntity->addComponent(lineMesh);
    lineEntity->addComponent(material);

    auto *pointsEntity = new Qt3DCore::QEntity(rootEntity);
    pointsEntity->addComponent(pointsMesh);
    pointsEntity->addComponent(material);
}

CoordPreview::~CoordPreview()
{
}
