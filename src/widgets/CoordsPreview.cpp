#include "CoordsPreview.hpp"
#include <qrgb.h>

CoordsPreview::CoordsPreview(QWidget *parent, Settings *const settings)
    : QWidget(parent)
    , settings_(settings)
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
    auto *material = new Qt3DRender::QMaterial(rootEntity);
    //TODO: setting

    // shader
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
    /* pointSize->setValue(8.0f); */
    pointRenderPass->addRenderState(pointSize);

    pointShaderProgram->setVertexShaderCode(
        "#version 330\n"

        "uniform mat4 modelViewProjection;"

        "layout(location = 0) in vec3 vertexPosition;"
        "layout(location = 1) in vec4 vertexColor;"
        "layout(location = 2) in float pointSize;"

        "out vec4 fragColor;"

        "void main(void)"
        "{"
        "    fragColor = vertexColor;"
        "    gl_PointSize = pointSize;"
        "    gl_Position = modelViewProjection * vec4(vertexPosition, 1);"
        "}");
    pointShaderProgram->setFragmentShaderCode("#version 330\n"

                                              "in vec4 fragColor;"

                                              "void main(void)"
                                              "{"
                                              "    gl_FragColor = fragColor;"
                                              "}");

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

    Qt3DRender::QBuffer *pSizeBuffer = new Qt3DRender::QBuffer(geometry);
    this->pSizeAttr_ = new Qt3DRender::QAttribute(geometry);
    this->pSizeAttr_->setName("pointSize");
    this->pSizeAttr_->setByteStride(sizeof(float));
    this->pSizeAttr_->setVertexSize(1);
    this->pSizeAttr_->setVertexBaseType(Qt3DRender::QAttribute::Float);
    this->pSizeAttr_->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    this->pSizeAttr_->setBuffer(pSizeBuffer);
    geometry->addAttribute(this->pSizeAttr_);

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
}

CoordsPreview::~CoordsPreview()
{
}

void CoordsPreview::updatePoints(const QList<CamPoint> &pointsList)
{
    // vertices
    {
        QByteArray vertBytes;
        vertBytes.resize(pointsList.size() * 3 * sizeof(float));

        float *positions = reinterpret_cast<float *>(vertBytes.data());
        for (auto &p : pointsList)
        {
            *positions++ = p.position().x();
            *positions++ = p.position().y();
            *positions++ = p.position().z();
        }
        this->posAttr_->buffer()->setData(vertBytes);
        this->posAttr_->setCount(pointsList.size());
    }

    // indices
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
            *sizes++ = 4.f;
        }
        this->pSizeAttr_->buffer()->setData(sizeBytes);
        this->pSizeAttr_->setCount(pointsList.size());
    }
}

void CoordsPreview::updateActive(const unsigned int &activeIdx)
{
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

        sizes[this->lastActiveIdx_] = 4.f;
        sizes[activeIdx] = 8.f;

        this->pSizeAttr_->buffer()->setData(sizeBytes);
        this->pSizeAttr_->setCount(count);
    }

    this->lastActiveIdx_ = activeIdx;
}
