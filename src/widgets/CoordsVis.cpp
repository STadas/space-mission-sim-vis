#include "CoordsVis.hpp"

CoordsVis::CoordsVis(QWidget *parent, Settings *const settings,
                     Resources *const resources)
    : QWidget(parent)
    , settings_(settings)
    , resources_(resources)
{
    QBoxLayout *layout = new VBoxLayout(this);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto view3d = new Qt3DExtras::Qt3DWindow();
    auto container = QWidget::createWindowContainer(view3d, this);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->layout()->addWidget(container);

    // root entity
    this->rootEntity_ = new Qt3DCore::QEntity();
    view3d->setRootEntity(this->rootEntity_);

    // background
    view3d->defaultFrameGraph()->setClearColor(
        QColor(this->settings_->coordsVisBackgroundColor.value()));

    // camera
    this->camera_ = view3d->camera();
    this->camera_->lens()->setPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f,
                                                    10000.0f);
    this->camera_->setPosition(QVector3D(0, 10.0f, 20.0f));
    this->camera_->setUpVector(QVector3D(0, 1, 0));
    this->camera_->setViewCenter(QVector3D(0, 0, 0));

    this->initPlane();
    this->initPathGeometry();
    this->initControls();
}

CoordsVis::~CoordsVis()
{
}

void CoordsVis::initControls()
{
    QWidget *controlsWrapper = new QWidget(this);
    QHBoxLayout *controlsLayout = new QHBoxLayout(controlsWrapper);
    controlsLayout->setMargin(0);

    // plane scale
    QLabel *planeScaleLabel = new QLabel("Plane scale", controlsWrapper);
    planeScaleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSpinBox *planeScaleBox = new QSpinBox(this);
    planeScaleBox->setMaximum(INT_MAX);
    planeScaleBox->setValue(this->planeTransform_->scale());
    QObject::connect(planeScaleBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, [=](const int &newVal) {
                         this->planeTransform_->setScale(newVal);
                     });

    // far plane
    QLabel *farPlaneLabel = new QLabel("Cam far plane", controlsWrapper);
    farPlaneLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    QSpinBox *farPlaneBox = new QSpinBox(this);
    farPlaneBox->setMaximum(INT_MAX);
    farPlaneBox->setValue(this->camera_->lens()->farPlane());
    QObject::connect(farPlaneBox, QOverload<int>::of(&QSpinBox::valueChanged),
                     this, [=](const int &newVal) {
                         this->camera_->lens()->setFarPlane(newVal);
                     });

    controlsLayout->addWidget(planeScaleLabel);
    controlsLayout->addWidget(planeScaleBox);
    controlsLayout->addWidget(farPlaneLabel);
    controlsLayout->addWidget(farPlaneBox);

    this->layout()->addWidget(controlsWrapper);

    // camera
    this->camController_ =
        new Qt3DExtras::QOrbitCameraController(this->rootEntity_);
    this->camController_->setCamera(this->camera_);

    QObject::connect(
        this->camera_, &Qt3DRender::QCamera::positionChanged, this,
        [this](const QVector3D &pos) {
            QByteArray posBytes = this->posAttr_->buffer()->data();
            if (posBytes.size() == 0)
                return;

            float *positions = reinterpret_cast<float *>(posBytes.data());

            QVector3D pointPos = {positions[this->lastActiveIdx_ * 3],
                                  positions[this->lastActiveIdx_ * 3 + 1],
                                  positions[this->lastActiveIdx_ * 3 + 2]};

            this->camController_->setLinearSpeed((pos - pointPos).length() * 2);
            this->camera_->setViewCenter(pointPos);
            this->camera_->transform()->setRotationZ(0);
        });
}

void CoordsVis::initPlane()
{
    auto *planeEntity = new Qt3DCore::QEntity(this->rootEntity_);
    auto *planeMesh = new Qt3DExtras::QPlaneMesh();
    auto *planeMaterial = new Qt3DExtras::QPhongMaterial();
    this->planeTransform_ = new Qt3DCore::QTransform();

    planeMesh->setWidth(1.0f);
    planeMesh->setHeight(1.0f);

    planeMaterial->setDiffuse(this->settings_->coordsVisPlaneColor.value());
    planeMaterial->setAmbient(QColor("#ffffff"));
    planeMaterial->setShininess(0);

    this->planeTransform_->setScale(10);
    this->planeTransform_->setRotation(
        QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), 0.0f));
    this->planeTransform_->setTranslation({0, 0, 0});

    planeEntity->addComponent(planeMesh);
    planeEntity->addComponent(planeMaterial);
    planeEntity->addComponent(this->planeTransform_);
}

void CoordsVis::initPathGeometry()
{
    // shader
    auto *material = new Qt3DRender::QMaterial();
    auto *pointEffect = new Qt3DRender::QEffect();
    auto *pointTechnique = new Qt3DRender::QTechnique();
    auto *pointRenderPass = new Qt3DRender::QRenderPass();
    auto *pointShaderProgram = new Qt3DRender::QShaderProgram();

    pointTechnique->graphicsApiFilter()->setApi(
        Qt3DRender::QGraphicsApiFilter::OpenGL);
    pointTechnique->graphicsApiFilter()->setProfile(
        Qt3DRender::QGraphicsApiFilter::CoreProfile);
    pointTechnique->graphicsApiFilter()->setMajorVersion(3);
    pointTechnique->graphicsApiFilter()->setMinorVersion(1);
    Qt3DRender::QFilterKey *filterKey = new Qt3DRender::QFilterKey(material);
    filterKey->setName(QStringLiteral("renderingStyle"));
    filterKey->setValue(QStringLiteral("forward"));
    pointTechnique->addFilterKey(filterKey);

    material->setEffect(pointEffect);

    pointEffect->addTechnique(pointTechnique);

    pointTechnique->addRenderPass(pointRenderPass);

    pointRenderPass->setShaderProgram(pointShaderProgram);
    auto pointSize = new Qt3DRender::QPointSize();
    pointSize->setSizeMode(Qt3DRender::QPointSize::SizeMode::Programmable);
    pointRenderPass->addRenderState(pointSize);

    pointShaderProgram->setVertexShaderCode(
        this->resources_->flightPathVShader);
    pointShaderProgram->setFragmentShaderCode(
        this->resources_->flightPathFShader);

    Qt3DRender::QGeometry *geometry =
        new Qt3DRender::QGeometry(this->rootEntity_);
    Qt3DRender::QBuffer *vertBuffer = new Qt3DRender::QBuffer(geometry);

    // vertexPosition attribute
    this->posAttr_ = new Qt3DRender::QAttribute(geometry);
    this->posAttr_->setName(
        Qt3DRender::QAttribute::defaultPositionAttributeName());
    this->posAttr_->setByteStride(3 * sizeof(float));
    this->posAttr_->setVertexSize(3);
    this->posAttr_->setVertexBaseType(Qt3DRender::QAttribute::Float);
    this->posAttr_->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    this->posAttr_->setBuffer(vertBuffer);
    geometry->addAttribute(this->posAttr_);

    // vertex indices
    Qt3DRender::QBuffer *idxBuffer = new Qt3DRender::QBuffer(geometry);
    this->idxAttr_ = new Qt3DRender::QAttribute(geometry);
    this->idxAttr_->setByteStride(sizeof(unsigned int));
    this->idxAttr_->setVertexSize(1);
    this->idxAttr_->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    this->idxAttr_->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    this->idxAttr_->setBuffer(idxBuffer);
    geometry->addAttribute(this->idxAttr_);

    // vertexColor attribute
    Qt3DRender::QBuffer *colorBuffer = new Qt3DRender::QBuffer(geometry);
    this->colorAttr_ = new Qt3DRender::QAttribute(geometry);
    this->colorAttr_->setName(
        Qt3DRender::QAttribute::defaultColorAttributeName());
    this->colorAttr_->setByteStride(4 * sizeof(float));
    this->colorAttr_->setVertexSize(4);
    this->colorAttr_->setVertexBaseType(Qt3DRender::QAttribute::Float);
    this->colorAttr_->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    this->colorAttr_->setBuffer(colorBuffer);
    geometry->addAttribute(this->colorAttr_);

    // pointSize attribute
    Qt3DRender::QBuffer *pSizeBuffer = new Qt3DRender::QBuffer(geometry);
    this->pSizeAttr_ = new Qt3DRender::QAttribute(geometry);
    this->pSizeAttr_->setName("pointSize");
    this->pSizeAttr_->setByteStride(sizeof(float));
    this->pSizeAttr_->setVertexSize(1);
    this->pSizeAttr_->setVertexBaseType(Qt3DRender::QAttribute::Float);
    this->pSizeAttr_->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    this->pSizeAttr_->setBuffer(pSizeBuffer);
    geometry->addAttribute(this->pSizeAttr_);

    auto *lineMesh = new Qt3DRender::QGeometryRenderer(this->rootEntity_);
    lineMesh->setGeometry(geometry);
    lineMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *lineEntity = new Qt3DCore::QEntity(this->rootEntity_);
    lineEntity->addComponent(lineMesh);
    lineEntity->addComponent(material);

    auto *pointsMesh = new Qt3DRender::QGeometryRenderer(this->rootEntity_);
    pointsMesh->setGeometry(geometry);
    pointsMesh->setPrimitiveType(Qt3DRender::QGeometryRenderer::Points);
    auto *pointsEntity = new Qt3DCore::QEntity(this->rootEntity_);
    pointsEntity->addComponent(pointsMesh);
    pointsEntity->addComponent(material);
}

void CoordsVis::updatePoints(const QList<CamPoint> &pointsList)
{
    // positions
    {
        QByteArray posBytes;
        posBytes.resize(pointsList.size() * 3 * sizeof(float));

        float *positions = reinterpret_cast<float *>(posBytes.data());
        for (auto &p : pointsList)
        {
            // the command treats z as the vertical axis, whereas the 3D
            // environment - y, so we switch them around
            *positions++ = p.position().x();
            *positions++ = p.position().z();
            *positions++ = p.position().y();
        }
        this->posAttr_->buffer()->setData(posBytes);
        this->posAttr_->setCount(pointsList.size());
    }

    // indices
    if (pointsList.size() != 1)
    {
        QByteArray idxBytes;
        unsigned int vertCount = std::max(pointsList.size() - 1, 0);
        unsigned int idxCount = vertCount * 2;
        idxBytes.resize(idxCount * sizeof(unsigned int));

        unsigned int *indices =
            reinterpret_cast<unsigned int *>(idxBytes.data());

        for (unsigned int i = 0; i < vertCount; ++i)
        {
            *indices++ = i;
            *indices++ = i + 1;
        }
        this->idxAttr_->buffer()->setData(idxBytes);
        this->idxAttr_->setCount(idxCount);
    }
    else
    {
        // hacky way to still draw a point if there's just one vertex
        // using the same line indices structure
        QByteArray idxBytes;
        unsigned int idxCount = 2;
        idxBytes.resize(idxCount * sizeof(unsigned int));

        unsigned int *indices =
            reinterpret_cast<unsigned int *>(idxBytes.data());
        *indices++ = 0;
        *indices++ = 0;

        this->idxAttr_->buffer()->setData(idxBytes);
        this->idxAttr_->setCount(idxCount);
    }

    // colors
    {
        QByteArray colorBytes;
        colorBytes.resize(pointsList.size() * 4 * sizeof(float));

        float *colors = reinterpret_cast<float *>(colorBytes.data());
        QColor defaultColor(settings_->coordsVisDefaultColor.value());
        for (auto &p : pointsList)
        {
            *colors++ = defaultColor.redF();
            *colors++ = defaultColor.greenF();
            *colors++ = defaultColor.blueF();
            *colors++ = defaultColor.alphaF();
        }
        this->colorAttr_->buffer()->setData(colorBytes);
        this->colorAttr_->setCount(pointsList.size());
    }

    // point size
    {
        QByteArray sizeBytes;
        sizeBytes.resize(pointsList.size() * sizeof(float));

        float *sizes = reinterpret_cast<float *>(sizeBytes.data());
        for (unsigned int i = 0; i < pointsList.size(); i++)
        {
            *sizes++ = this->settings_->coordsVisDefaultPSize.value();
        }
        this->pSizeAttr_->buffer()->setData(sizeBytes);
        this->pSizeAttr_->setCount(pointsList.size());
    }
}

void CoordsVis::updateActive(const unsigned int &activeIdx)
{
    if (this->posAttr_->buffer()->data().size() == 0)
        return;

    // colors
    {
        QByteArray colorBytes = this->colorAttr_->buffer()->data();
        unsigned int count = colorBytes.size() / 4 / sizeof(float);

        float *colors = reinterpret_cast<float *>(colorBytes.data());

        QColor defaultColor(settings_->coordsVisDefaultColor.value());
        QColor activeColor(this->settings_->coordsVisActiveColor.value());

        colors[this->lastActiveIdx_ * 4] = defaultColor.redF();
        colors[this->lastActiveIdx_ * 4 + 1] = defaultColor.greenF();
        colors[this->lastActiveIdx_ * 4 + 2] = defaultColor.blueF();
        colors[this->lastActiveIdx_ * 4 + 3] = defaultColor.alphaF();

        colors[activeIdx * 4] = activeColor.redF();
        colors[activeIdx * 4 + 1] = activeColor.greenF();
        colors[activeIdx * 4 + 2] = activeColor.blueF();
        colors[activeIdx * 4 + 3] = activeColor.alphaF();

        this->colorAttr_->buffer()->setData(colorBytes);
        this->colorAttr_->setCount(count);
    }

    // point size
    {
        QByteArray sizeBytes = this->pSizeAttr_->buffer()->data();
        unsigned int count = sizeBytes.size() / sizeof(float);

        float *sizes = reinterpret_cast<float *>(sizeBytes.data());

        sizes[this->lastActiveIdx_] =
            this->settings_->coordsVisDefaultPSize.value();
        sizes[activeIdx] = this->settings_->coordsVisActivePSize.value();

        this->pSizeAttr_->buffer()->setData(sizeBytes);
        this->pSizeAttr_->setCount(count);
    }

    // camera position
    {
        QByteArray posBytes = this->posAttr_->buffer()->data();
        float *positions = reinterpret_cast<float *>(posBytes.data());
        QVector3D pointPos = {positions[activeIdx * 3],
                              positions[activeIdx * 3 + 1],
                              positions[activeIdx * 3 + 2]};

        QVector3D offset =
            this->camera_->position() - this->camera_->viewCenter();

        this->lastActiveIdx_ = activeIdx;

        this->camera_->setPosition(pointPos + offset);
    }
}
