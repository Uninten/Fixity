#ifndef URDFFILELOADER_H
#define URDFFILELOADER_H

#include <QString>
#include <QList>
#include "stlfileloader.h"
#include "urdfparser.h"

class URDFFileLoader
{
public:
    URDFFileLoader(QString filename, int ratio);
    ~URDFFileLoader();
    void draw();
    void setupColor(int r, int g, int b, int alpha);
    void printJointTree();
    QMatrix4x4 calculateGlobalTransform(const STLFileLoader* mod);
    QList<STLFileLoader*> getModels() {return models;}

private:
    void loadUrdf(const QString &filename);
    void loadTextUrdf(const QString &filename);
    void loadBinaryUrdf(const QString &filename);

private:
    float mRatio{1.0f};
    URDFParser* parser{nullptr};
    QList<STLFileLoader*> models{};

};

#endif // URDFFILELOADER_H
