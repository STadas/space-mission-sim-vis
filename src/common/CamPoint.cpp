#include "CamPoint.hpp"

CamPoint::CamPoint(const unsigned int &lineNum, const QVector3D &position)
    : lineNum_(lineNum)
    , position_(position)
{
}

CamPoint::~CamPoint()
{
}

unsigned int CamPoint::lineNum() const
{
    return this->lineNum_;
}

QVector3D CamPoint::position() const
{
    return this->position_;
}
