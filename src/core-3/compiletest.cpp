#include <core-3/archive.h>
#include <core-3/fetch.h>
#include <core-3/check.h>
#include <core-3/decodr.h>
#include <core-3/decoding.h>

using namespace Core3;

void F()
{
    Archive<QByteArray> ar;
    ar.addInitCluster(qMakePair(123, QByteArray()));
    ar.setNewState(QByteArray(), true);

}