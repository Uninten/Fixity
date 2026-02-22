#include "stltriangle.h"

STLTriangle::STLTriangle()
{

}

QVector3D STLTriangle::getVertex(int index) const
{
    Q_ASSERT(index >= 0 && index < 3);
    return m_vertices[index];
}

void STLTriangle::setVertex(int index, const QVector3D &vertex)
{
    Q_ASSERT(index >= 0 && index < 3);
    m_vertices[index] = vertex;
}

void STLTriangle::setVertices(const QVector3D &v1, const QVector3D &v2, const QVector3D &v3)
{
    m_vertices[0] = v1;
    m_vertices[1] = v2;
    m_vertices[2] = v3;
}

void STLTriangle::calculateNormal()
{
    QVector3D edge1 = m_vertices[1] - m_vertices[0];
    QVector3D edge2 = m_vertices[2] - m_vertices[0];
    m_normal = QVector3D::crossProduct(edge1, edge2).normalized();
}

void STLTriangle::reset()
{
    m_normal = QVector3D();  // 重置法向量
    m_vertices[0] = m_vertices[1] = m_vertices[2] = QVector3D();  // 重置顶点
}
