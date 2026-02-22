#include "urdffileloader.h"
#include <QFile>
#include <QDebug>
#include <QVector3D>
#include <QDataStream>
#include <QOpenGLFunctions>  // Qt封装的OpenGL函数
#include <QMatrix4x4>

URDFFileLoader::URDFFileLoader(QString filename, int ratio) {
    mRatio = ratio;
    models.clear();
    parser = new URDFParser();
    loadUrdf(filename);
}

URDFFileLoader::~URDFFileLoader()
{

}

void URDFFileLoader::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 渲染所有网格
    for (auto* mod : models) {
        // 计算全局变换矩阵
        QMatrix4x4 globalTransform = calculateGlobalTransform(mod);

        setupColor(mod->color()[0], mod->color()[1], mod->color()[2], mod->color()[3]);

        // 调用带参数的draw版本
        mod->draw(globalTransform);
    }
}

void URDFFileLoader::setupColor(int r, int g, int b, int alpha = 1)
{
    // 设置材质颜色（包含透明度）
    GLfloat color[] = {
        static_cast<GLfloat>(r),
        static_cast<GLfloat>(g),
        static_cast<GLfloat>(b),
        static_cast<GLfloat>(alpha)
    };

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    // 启用混合（必须）
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void URDFFileLoader::printJointTree()
{
    for (const auto* mod : models) {
        if (!mod->parent()){
            qDebug() << mod->Name();
        }else{
            qDebug() << mod->Name() << "--->" << mod->parent()->Name();
        }
    }
}

QMatrix4x4 URDFFileLoader::calculateGlobalTransform(const STLFileLoader* mod) {
    QMatrix4x4 transform;

    if (mod->parent()) {
        transform = calculateGlobalTransform(mod->parent());
    }

    transform.translate(mod->position());
    transform.rotate(mod->rotation());
    transform.scale(mod->scale());

    return transform;
}

void URDFFileLoader::loadUrdf(const QString &filename)
{
    if (parser->parseURDF(filename)) {
        auto& visuals = parser->getVisuals();
        QHash<QString, STLFileLoader*> meshCache;
        for (auto& visual : visuals) {
            if (!visual.meshFilename.isEmpty()) {
                QString filename = visual.meshFilename;
                // 检查是否已经加载过这个文件
                if (meshCache.contains(filename)) {
                    // 如果已经存在，先移除旧的实例
                    STLFileLoader* existingMesh = meshCache.value(filename);
                    models.removeOne(existingMesh);
                    delete existingMesh; // 释放旧对象内存
                }
                // 使用你现有的STL加载器加载模型
                STLFileLoader* mesh = new STLFileLoader(visual.meshFilename, 1000);

                if (mesh) {
                    // 更新缓存，记录这是该文件的最后一个实例
                    meshCache.insert(filename, mesh);
                    // 设置变换
                    mesh->setPosition(visual.origin);
                    mesh->setRotation(visual.rotation);
                    mesh->setScale(visual.scale);
                    mesh->setColor(visual.color);
                    mesh->setName(visual.name);
                    mesh->setAxis(visual.axis);
                    if (!visual.parentname.isEmpty())
                    {
                        for (STLFileLoader* parentNode : models) {
                            if (visual.parentname == parentNode->Name()) {
                                visual.parent = parentNode;
                                mesh->setParent(visual.parent);
                                break;
                            }
                        }
                    }

                    // 添加到渲染列表
                    models.append(mesh);
                }
            }
        }
        meshCache.clear();
    }
}
