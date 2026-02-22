#include "stlfileloader.h"
#include <QFile>
#include <QDebug>
#include <QVector3D>
#include <QDataStream>
#include <QOpenGLFunctions>  // Qt封装的OpenGL函数

STLFileLoader::STLFileLoader(QString filename, int ratio) {
    mRatio = ratio;
    model.clear();
    loadStl(filename);
}

STLFileLoader::~STLFileLoader() {}

void STLFileLoader::loadStl(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray arr;
        arr = file.read(5);
        file.close();
        if (arr == "solid") {
            loadTextStl(filename);
        } else {
            loadBinaryStl(filename);
        }
    } else {
        qDebug() << filename << u8"不存在";
    }
}

void STLFileLoader::loadTextStl(const QString &filename)
{
    qDebug() << "load text file:" << filename;
    model.clear(); //清除模型
    QList <QVector3D> triangle;
    STLTriangle tSTLTriangle;
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!file.atEnd()) {
            QString line = file.readLine().trimmed();
            QStringList words = line.split(' ', QString::SkipEmptyParts);

            if (words[0] == "facet") {
                triangle.clear();
                tSTLTriangle.reset();
                tSTLTriangle.setNormal(QVector3D(words[2].toFloat(), words[3].toFloat(), words[4].toFloat()));
            } else if (words[0] == "vertex") {
                triangle.append(QVector3D(words[1].toFloat(), words[2].toFloat(), words[3].toFloat()));
            } else if (words[0] == "endloop") {
                if (triangle.length() == 3) {
                    for (int i = 0; i < 3; ++i) {
                        tSTLTriangle.setVertex(i, triangle[i]);
                    }
                    model.append(tSTLTriangle);
                }
            }
        }
        file.close();
    }
}

void STLFileLoader::loadBinaryStl(const QString &filename)
{
    qDebug() << "load Binary file:" << filename;
    model.clear();          //清除模型
    QList <QVector3D> triangle;
    STLTriangle tSTLTriangle;

    QFile STL_file(filename);

    int fileSize = STL_file.size();
    char *buf = (char *) malloc(sizeof(char) * fileSize);

    bool isOk = STL_file.open(QIODevice::ReadOnly);
    if (!isOk) return;

    QDataStream stream(&STL_file);
    stream.readRawData(buf, fileSize);
    STL_file.close();

    const char *p = buf;
    char name[80];          //起始80个字节 文件名
    int triangle_num;       //4个字节 三角形个数
    float n1, n2, n3;       //法向量
    float v1, v2, v3;       //定点

    memcpy(name, p, 80);                        //记录文件名
    p += 80;                                    //跳过文件名

    memcpy(&triangle_num, p, 4);                //记录三角形个数
    p += 4;                                     //跳过个数标识

    for (int i = 0; i < triangle_num; i++) {     //读取法向量
        memcpy(&n1, p, 4);
        p += 4;
        memcpy(&n2, p, 4);
        p += 4;
        memcpy(&n3, p, 4);
        p += 4;

        triangle.clear();
        tSTLTriangle.reset();
        tSTLTriangle.setNormal(QVector3D(n1, n2, n3));

        for (int j = 0; j < 3; j++) {             //读取顶点信息
            memcpy(&v1, p, 4);
            p += 4;
            memcpy(&v2, p, 4);
            p += 4;
            memcpy(&v3, p, 4);
            p += 4;
            triangle.append(QVector3D(v1, v2, v3));
        }
        if (triangle.length() == 3) {
            for (int i = 0; i < 3; ++i) {
                tSTLTriangle.setVertex(i, triangle[i]);
            }
            model.append(tSTLTriangle);
        }
        p += 2;//跳过尾部标志 两字节
    }
    free(buf);
}

void STLFileLoader::draw()
{
    QList <STLTriangle> triangles = model;
    QVector3D normal;
    QVector3D vertex;
    glBegin(GL_TRIANGLES); // 绘制一个或多个三角形
    foreach(STLTriangle tri, triangles) {
        normal = tri.getNormal();
        glNormal3f(mRatio * normal.x(), mRatio * normal.y(), mRatio * normal.z());
        for (int j = 0; j < 3; ++j) {
            vertex = tri.getVertex(j);
            glVertex3f(mRatio * vertex.x(), mRatio * vertex.y(), mRatio * vertex.z());
        }
    }
    glEnd();
}

void STLFileLoader::draw(const QMatrix4x4 &globalTransform)
{
    QList<STLTriangle> triangles = model;

    glBegin(GL_TRIANGLES);
    foreach(STLTriangle tri, triangles) {
        // 变换法线向量
        QVector3D normal = (globalTransform * QVector4D(tri.getNormal(), 0.0f)).toVector3D().normalized();
        glNormal3f(mRatio * normal.x(), mRatio * normal.y(), mRatio * normal.z());

        for (int j = 0; j < 3; ++j) {
            QVector3D vertex = tri.getVertex(j);
            // 变换顶点位置
            QVector4D transformedVertex = globalTransform * QVector4D(vertex, 1.0f);
            glVertex3f(mRatio * transformedVertex.x(), mRatio * transformedVertex.y(), mRatio * transformedVertex.z());
        }
    }
    glEnd();
}
