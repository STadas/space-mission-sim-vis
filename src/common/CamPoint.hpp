#pragma once

#include <QtCore>
#include <QtWidgets>

/**
 * Cam point class. Holds information about a single resulting camera point from
 * a command, to be used in widgets
 */
class CamPoint
{
public:
     /**
     * Constructor
     * @param lineNum Line number of the command in the editor
     * @param position Position of the camera after executing the command
     */
    CamPoint(const unsigned int &lineNum, const QVector3D &position);
    /**
     * Destructor
     */
    ~CamPoint();

    /**
     * @return Command's line number in the editor
     */
    unsigned int lineNum() const;
    /**
     * @return Camera position as a vector3D
     */
    QVector3D position() const;

private:
    unsigned int lineNum_;
    QVector3D position_;
};
