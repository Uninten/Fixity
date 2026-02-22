#ifndef STLFILELOADER_H
#define STLFILELOADER_H

#include <QString>
#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include "stltriangle.h"
#include <QMatrix4x4>

class STLTriangle;

class STLFileLoader
{
public:
    STLFileLoader(QString filename, int ratio);
    ~STLFileLoader();
    void draw();
    void draw(const QMatrix4x4& globalTransform);

    void setPosition(QVector3D origin){pos = origin;}
    void setRotation(QQuaternion rotation){rot = rotation;}
    void setScale(QVector3D scale){sca = scale;}
    void setAxis(QVector3D axis){axi = axis;}
    void setColor(QVector4D color){col = color;}
    void setParent(STLFileLoader* parent){par = parent;}
    void setName(QString Name){name = Name;}
    const QVector3D& position() const{return pos;}
    const QQuaternion& rotation() const{return rot;}
    const QVector3D& scale() const{return sca;}
    const QVector3D& axis() const{return axi;}
    const QVector4D& color() const{return col;}
    const QString& Name() const{return name;}
    const STLFileLoader* parent() const{return par;}
    const QMatrix4x4& translation() const{return translate;}

private:
    void loadStl(const QString &filename);
    void loadTextStl(const QString &filename);
    void loadBinaryStl(const QString &filename);


private:
    QList<STLTriangle> model{};
    float mRatio{1.0f};
    QVector3D pos{};
    QQuaternion rot{};
    QVector3D sca{};
    QVector3D axi{};
    QVector4D col{};
    QString name{};
    STLFileLoader* par{nullptr};
    QMatrix4x4 translate{};
};

#endif // STLFILELOADER_H
