#ifndef URDFPARSER_H
#define URDFPARSER_H

#include <QVector3D>
#include <QQuaternion>
#include <QXmlStreamReader>
#include <QFileInfo>
#include <cmath>
#include <QMap>
#include <QString>
#include "stlfileloader.h"

struct URDFVisual {
    QString meshFilename;  // STL文件路径
    QVector3D origin;      // 位置偏移
    QQuaternion rotation;  // 旋转
    QVector3D axis;        // 坐标
    QVector3D scale;       // 缩放
    QVector4D color;       // 颜色
    STLFileLoader* parent; // 父节点
    QString parentname;    // 父节点名字
    QString name;         // 本节点名字
};

class URDFParser
{
public:
    URDFParser();
    ~URDFParser();

    QString getAncestorPath(const QString &path, int levels = 1);

    bool parseURDF(const QString& filepath);
    QList<URDFVisual>& getVisuals() { return visuals; }
private:
    QList<URDFVisual> visuals;
    QString basePath;
    void parseLink(QXmlStreamReader& xml, URDFVisual& visual);
    void parseVisual(QXmlStreamReader& xml, URDFVisual& visual);
    void parseOrigin(QXmlStreamReader& xml, URDFVisual& visual);
    void parseGeometry(QXmlStreamReader& xml, URDFVisual& visual);
    void parseMaterial(QXmlStreamReader& xml, URDFVisual& visual);
    void parseJoint(QXmlStreamReader& xml, URDFVisual& visual);
};
#endif // URDFPARSER_H
