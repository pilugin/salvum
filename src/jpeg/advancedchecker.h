#ifndef ADVANCEDCHECKER_H
#define ADVANCEDCHECKER_H

#include <jpeg/icheck.h>
#include <QObject>
#include <QImage>

namespace Jpeg {

class AdvancedChecker : public QObject, public ICheck
{
    Q_OBJECT
public:
    AdvancedChecker(QObject *parent =nullptr);
    bool check(const QImage &image, int blockBegin, int blockEnd, double *relevance =nullptr);
    double minRelevance() const;
    
public slots:
    void addThumbnail(const QString &path);

private:
    double processRow(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;

    double processRow_Xtremum(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;
    double processRow_Colors(const QImage &image, int blockBeginX, int blockEndX, int blockY) const;
    
    QImage mThumbnail;
};

} // eons Jpeg

#endif // ADVANCEDCHECKER_H
