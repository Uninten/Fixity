#include "urdfparser.h"
#include <QDir>

URDFParser::URDFParser()
{

}

URDFParser::~URDFParser()
{

}

QString URDFParser::getAncestorPath(const QString &path, int levels)
{
    QDir dir(QFileInfo(path).absolutePath());
    while (levels-- > 0 && dir.cdUp());  // 尝试进入父目录多次
    return dir.absolutePath();
}

bool URDFParser::parseURDF(const QString &filepath)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    basePath = getAncestorPath(QFileInfo(filepath).absolutePath());

    QXmlStreamReader xml(&file);
    URDFVisual visual;
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement() && xml.name() == "link") {
            visual.name = xml.attributes().value("name").toString();
            parseLink(xml, visual);
        }
        else if (xml.isStartElement() && xml.name() == "joint"){
            parseJoint(xml, visual);
        }
        visuals.append(visual);
    }
    return !xml.hasError();
}

void URDFParser::parseLink(QXmlStreamReader& xml, URDFVisual& visual) {

    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isEndElement() && xml.name() == "link") {
            break;
        }

        if (xml.isStartElement() && xml.name() == "visual") {
            parseVisual(xml, visual);
        }
    }
}

void URDFParser::parseVisual(QXmlStreamReader& xml, URDFVisual& visual)
{
    // 初始化默认值
    visual.origin = QVector3D(0, 0, 0);
    visual.rotation = QQuaternion();
    visual.scale = QVector3D(1, 1, 1);
    visual.color = QVector4D(1.0f, 1.0f, 1.0f, 1.0f); // 默认灰色
    visual.axis = QVector3D(0, 0, 1);

    while (!xml.atEnd()) {
        xml.readNext();

        // 使用switch处理不同元素类型
        switch (xml.tokenType()) {
        case QXmlStreamReader::StartElement: {
            const QStringRef elementName = xml.name();

            if (elementName == "origin") {
                // 处理origin元素
                parseOrigin(xml, visual);
            }
            else if (elementName == "geometry") {
                // 处理geometry元素
                parseGeometry(xml, visual);
            }
            else if (elementName == "material") {
                // 处理material元素
                parseMaterial(xml, visual);
            }
            break;
        }
        case QXmlStreamReader::EndElement:
            if (xml.name() == "visual") {
                return; // 结束visual解析
            }
            break;
        default:
            break;
        }
    }
}

// 提取origin解析为独立函数
void URDFParser::parseOrigin(QXmlStreamReader& xml, URDFVisual& visual)
{
    QString xyz = xml.attributes().value("xyz").toString();
    QString rpy = xml.attributes().value("rpy").toString();

    // 处理位置(xyz)
    if (!xyz.isEmpty()) {
        QStringList parts = xyz.split(" ");
        if (parts.size() == 3) {
            visual.origin.setX(parts[0].toFloat());
            visual.origin.setY(parts[1].toFloat());
            visual.origin.setZ(parts[2].toFloat());
        }
    }

    // 处理旋转(rpy)
    if (!rpy.isEmpty()) {
        QStringList parts = rpy.split(" ");
        if (parts.size() == 3) {
            float roll = parts[0].toFloat();
            float pitch = parts[1].toFloat();
            float yaw = parts[2].toFloat();

            visual.rotation = QQuaternion::fromEulerAngles(
                roll * 180 / M_PI,
                pitch * 180 / M_PI,
                yaw * 180 / M_PI
            );
        }
    }
}

// 提取geometry解析为独立函数
void URDFParser::parseGeometry(QXmlStreamReader& xml, URDFVisual& visual)
{
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isEndElement() && xml.name() == "geometry") {
            break;
        }

        if (xml.isStartElement() && xml.name() == "mesh") {
            QString filename = xml.attributes().value("filename").toString();
            QString scaleStr = xml.attributes().value("scale").toString();

            // 处理相对路径
            if (filename.startsWith("package://")) {
                filename = filename.replace("package://", "");
                filename = basePath + "/" + filename;
            }

            visual.meshFilename = filename;

            // 处理缩放
            if (!scaleStr.isEmpty()) {
                QStringList parts = scaleStr.split(" ");
                if (parts.size() == 3) {
                    visual.scale = QVector3D(
                        parts[0].toFloat(),
                        parts[1].toFloat(),
                        parts[2].toFloat()
                    );
                } else if (parts.size() == 1) {
                    float s = parts[0].toFloat();
                    visual.scale = QVector3D(s, s, s);
                }
            }
        }
    }
}

// 提取material解析为独立函数
void URDFParser::parseMaterial(QXmlStreamReader& xml, URDFVisual& visual)
{
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isEndElement() && xml.name() == "material") {
            break;
        }

        if (xml.isStartElement() && xml.name() == "color") {
            QStringList rgba = xml.attributes().value("rgba").toString().split(" ");
            if (rgba.size() == 4) {
                visual.color = QVector4D(
                    rgba[0].toFloat(),
                    rgba[1].toFloat(),
                    rgba[2].toFloat(),
                    rgba[3].toFloat()
                );
            }
        }
    }
}

void URDFParser::parseJoint(QXmlStreamReader &xml, URDFVisual &visual)
{
    while (!xml.atEnd()) {
        xml.readNext();

        if (xml.isEndElement() && xml.name() == "joint") {
            break;
        }

        // 使用switch处理不同元素类型
        switch (xml.tokenType()) {
        case QXmlStreamReader::StartElement: {
            const QStringRef elementName = xml.name();

            if (elementName == "parent") {
                // 处理parent元素
                visual.parentname = xml.attributes().value("link").toString();
//                qDebug() << visual.name << "--->" << visual.parentname;
            }
            else if (elementName == "child") {
                // 处理child元素
//                visual.child = xml.attributes().value("link").toString();
            }
            else if (elementName == "origin") {
                // 处理origin元素
                // 解析坐标和旋转（xyz 和 rpy）
                visual.origin = QVector3D(
                    xml.attributes().value("xyz").toString().split(" ").at(0).toFloat(),
                    xml.attributes().value("xyz").toString().split(" ").at(1).toFloat(),
                    xml.attributes().value("xyz").toString().split(" ").at(2).toFloat()
                );
                visual.rotation = QQuaternion::fromEulerAngles(
                    xml.attributes().value("rpy").toString().split(" ").at(0).toFloat() * 180/M_PI,
                    xml.attributes().value("rpy").toString().split(" ").at(1).toFloat() * 180/M_PI,
                    xml.attributes().value("rpy").toString().split(" ").at(2).toFloat() * 180/M_PI
                );
            }
            else if (elementName == "axis") {
                // 解析旋转轴
                visual.axis = QVector3D(
                    xml.attributes().value("xyz").toString().split(" ").at(0).toFloat(),
                    xml.attributes().value("xyz").toString().split(" ").at(1).toFloat(),
                    xml.attributes().value("xyz").toString().split(" ").at(2).toFloat()
                );
            }
            break;
        }
        case QXmlStreamReader::EndElement:
            if (xml.name() == "joint") {
                return; // 结束joint解析
            }
            break;
        default:
            break;
        }
    }
}
