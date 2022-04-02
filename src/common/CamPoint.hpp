#pragma once

#include <QtCore>
#include <QtWidgets>

class CamPoint
{
public:
    CamPoint(const unsigned int &lineNum, const QVector3D &position);
    ~CamPoint();

    unsigned int lineNum() const;
    QVector3D position() const;

private:
    unsigned int lineNum_;
    QVector3D position_;
};
