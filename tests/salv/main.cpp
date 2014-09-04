#include <QtCore>
#include <QtDebug>

int main(int argc, char **argv)
{
    if (argc != 2) {
        qDebug( "Usage: %s inputfile", argv[0]);
        return 0;
    }

    return 0;
}
