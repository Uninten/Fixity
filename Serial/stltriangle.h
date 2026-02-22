#ifndef STLTRIANGLE_H
#define STLTRIANGLE_H

#include <QVector3D>

class STLTriangle
{
public:
    STLTriangle();
    STLTriangle(const QVector3D &normal,
                    const QVector3D &v1,
                    const QVector3D &v2,
                    const QVector3D &v3)
            : m_normal(normal), m_vertices{v1, v2, v3} {}

    // 获取法向量
    QVector3D getNormal() const { return m_normal; }
    // 设置法向量
    void setNormal(const QVector3D &normal) { m_normal = normal; }
    // 获取顶点 (索引0-2)
    QVector3D getVertex(int index) const;
    // 设置顶点
    void setVertex(int index, const QVector3D &vertex);
    // 设置所有三个顶点
    void setVertices(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3);
    // 计算法向量（如果STL文件没有提供法向量）
    void calculateNormal();
    // 重置
    void reset();

private:
    QVector3D m_normal;            // 三角形法向量
    QVector3D m_vertices[3];       // 三角形的三个顶点
};

#endif // STLTRIANGLE_H
