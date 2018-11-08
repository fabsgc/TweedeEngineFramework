#include "Prerequisites/PrerequisitesUtil.h"
#include "md5.h"

namespace te
{
    String md5(const WString& source)
    {
        MD5 md5;
        md5.update((UINT8*)source.data(), (UINT32)source.length() * sizeof(WString::value_type));
        md5.finalize();

        UINT8 digest[16];
        md5.decdigest(digest, sizeof(digest));

        String buf;
        buf.resize(32);
        for (int i = 0; i < 16; i++)
            snprintf(&(buf[0]) + i * 2, 3, "%02x", digest[i]);

        return buf;
    }

    String md5(const String& source)
    {
        MD5 md5;
        md5.update((UINT8*)source.data(), (UINT32)source.length() * sizeof(String::value_type));
        md5.finalize();

        UINT8 digest[16];
        md5.decdigest(digest, sizeof(digest));

        String buf;
        buf.resize(32);
        for (int i = 0; i < 16; i++)
            snprintf(&(buf[0]) + i * 2, 3, "%02x", digest[i]);

        return buf;
    }
}