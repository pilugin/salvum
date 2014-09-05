#include <jpeg/advancedchecker.h>
#include <QImage>
#include <QtDebug>
#include <QString>
#include <util/ilog.h>

using namespace Log;

namespace Jpeg {

AdvancedChecker::AdvancedChecker()
{
}

double AdvancedChecker::minRelevance() const
{
    return 0.72;
}

bool AdvancedChecker::check(const QImage &image, int blockBegin, int blockEnd, double *relevance)
{
    int blockBeginX = blockBegin % (image.width()/8);
    int blockBeginY = blockBegin / (image.width()/8);

    int blockEndX = blockEnd % (image.width()/8);
    int blockEndY = blockEnd / (image.width()/8);

    const int maxBlockX = (image.width()/8) -1;

    Msg("[{%d-%d #%d} ", blockBeginX, blockBeginY, blockEnd - blockBegin);

    double result = 0.;
    static const double min = minRelevance();

    if (blockBeginY == blockEndY) {
        result = processRow(image, blockBeginX, blockEndX, blockBeginY);

    } else {
        int secondX = qMin(blockBeginX-1, blockEndX);

        double firstPart = maxBlockX - blockBeginX +1;
        double secondPart = secondX +1;

        firstPart = firstPart / (firstPart + secondPart);
        secondPart = 1. - firstPart;

        result = (processRow(image, blockBeginX, maxBlockX, blockBeginY) * firstPart)
                  + (processRow(image, 0, secondX, blockBeginY+1) * secondPart);

    }

    Msg("=%g]", result);

    if (relevance)
        *relevance = result;

    return (result > min);
}

double AdvancedChecker::processRow(const QImage &image, int blockBeginX, int blockEndX, int blockY) const
{
    static const double colorsLimit = .4;
    double colors = processRow_Colors(image, blockBeginX, blockEndX, blockY);
    if (colors < colorsLimit) {
        Msg("Colors-%g", colors);
        return 0.;
    }

    double xtremum = processRow_Xtremum(image, blockBeginX, blockEndX, blockY);

    return xtremum;
}

double AdvancedChecker::processRow_Xtremum(const QImage &image, int blockBeginX, int blockEndX, int blockY) const
{
    if (blockY == 0)
        return 1.0;

    int beginX = blockBeginX*8;
    int endX = blockEndX*8;


    int xtremumOnBreak =0;
    int lines = (endX - beginX +1) *3; //< each pixel. Three times: R, G, B

    for (int x = beginX; x<=endX; ++x) {
        int maxDRpos=0;
        int maxDGpos=0;
        int maxDBpos=0;
        int color = 0;
        int colorNext = 0;
        int maxDR = 0;
        int maxDG = 0;
        int maxDB = 0;
        int dR = 0;
        int dG = 0;
        int dB = 0;

        int y=blockY*8-8;

        color = image.pixel(x, y);
        for (; y<blockY*8+7; ++y) {
            colorNext = image.pixel(x, y+1);

            dR = qAbs(qRed(color) - qRed(colorNext));
            dG = qAbs(qGreen(color) - qGreen(colorNext));
            dB = qAbs(qBlue(color) - qBlue(colorNext));

            if (dR > maxDR) maxDRpos = y, maxDR = dR;
            if (dG > maxDG) maxDGpos = y, maxDG = dG;
            if (dB > maxDB) maxDBpos = y, maxDB = dB;


            color = colorNext;
        }

        if (maxDRpos == (blockY*8-1)) ++xtremumOnBreak;
        if (maxDGpos == (blockY*8-1)) ++xtremumOnBreak;
        if (maxDBpos == (blockY*8-1)) ++xtremumOnBreak;

    }

    return 1. - (double(xtremumOnBreak)/lines);
}

double AdvancedChecker::processRow_Colors(const QImage &image, int blockBeginX, int blockEndX, int blockY) const
{
    if (blockY == 0)
        return 1.0;

    static const int maxDelta = 45;

    int badColors =0;
    int blocks = (blockEndX - blockBeginX +1)*8;

    int beginX = blockBeginX *8;
    int endX = blockEndX *8;

    int y=blockY*8;
    for (int x = beginX; x<=endX; ++x) {
        int u = image.pixel(x, y-1);
        int l = image.pixel(x, y);
        int ru=qRed(u); //< r/g/b upper/lower block
        int rl=qRed(l);
        int gu=qGreen(u);
        int gl=qGreen(l);
        int bu=qBlue(u);
        int bl=qBlue(l);

        int delta = qAbs(ru-rl) + qAbs(gu-gl) + qAbs(bu-bl);
        if (delta > maxDelta) {
            ++badColors;
//            Msg("BAD color d=%d n=%d\t%06X %06X\n", delta, badColors, u, l);
        }
    }

    return 1. - (double(badColors)/blocks);
}

} // eons Jpeg


















#if 0
    static int CNT=0;

    QFile outR(QString().sprintf("%04d%s.R.pixels", CNT, OK ? "" : "-BAD" ));
    QFile outG(QString().sprintf("%04d%s.G.pixels", CNT, OK ? "" : "-BAD" ));
    QFile outB(QString().sprintf("%04d%s.B.pixels", CNT, OK ? "" : "-BAD" ));
    ++CNT;
    if (!outR.open(QFile::WriteOnly | QFile::Truncate)
            || !outG.open(QFile::WriteOnly | QFile::Truncate)
            || !outB.open(QFile::WriteOnly | QFile::Truncate)) {
        qDebug()<<outR.errorString();
        qDebug()<<outG.errorString();
        qDebug()<<outB.errorString();
        return 0.0;
    }

    for (int y=blockY*8-8; y<=blockY*8+7; ++y) {
        for (int x=blockBeginX*8; x<=blockEndX*8; ++x) {
            int r = qRed(image.pixel(x, y));
            int g = qGreen(image.pixel(x, y));
            int b = qBlue(image.pixel(x, y));
            outR.write(QString("%1 ").arg(r).toAscii());
            outG.write(QString("%1 ").arg(g).toAscii());
            outB.write(QString("%1 ").arg(b).toAscii());
        }
        outR.write("\n");
        outG.write("\n");
        outB.write("\n");
    }

    return 1.0;
#endif

