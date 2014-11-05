#ifndef PICOJPEGDECODR_H
#define PICOJPEGDECODR_H

#include <jpeg/common.h>

#include <core-3/decodr.h>
#include <util/singleton.h>

namespace Jpeg {

class ICheck;

class PicoJpegDecodr : public Core3::Decodr< DecodrState >, public Singleton<PicoJpegDecodr>
{
public:
    PicoJpegDecodr(ICheck *checkr);
    ~PicoJpegDecodr();

    void init();
    bool feed(const Common::Cluster &cluster);
    bool initialized() const;

    bool checkOk() const;
    bool decodOk() const;
    bool end() const;

    const State &state() const;
protected:
    void doRestore(const State &state);

    struct Private;
    Private *m_d;
};

} // eons Jpeg

#endif // PICOJPEGDECODR_H
