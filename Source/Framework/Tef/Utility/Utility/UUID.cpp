#include "Prerequisites/PrerequisitesUtil.h"
#include "Utility/UUID.h"
#include <windows.h>
#include <iphlpapi.h>
#include <intrin.h>

namespace
{
    constexpr const char HEX_TO_LITERAL[16] =
    { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

    constexpr const te::UINT8 LITERAL_TO_HEX[256] =
    { 0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        // 0 through 9 translate to 0  though 9
        0x00,  0x01,  0x02,  0x03,  0x04,  0x05,  0x06,  0x07,  0x08,  0x09,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        // A through F translate to 10 though 15
        0xFF,  0x0A,  0x0B,  0x0C,  0x0D,  0x0E,  0x0F,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        // a through f translate to 10 though 15
        0xFF,  0x0A,  0x0B,  0x0C,  0x0D,  0x0E,  0x0F,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,
        0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF
    };
}

namespace te
{
    UUID UUID::EMPTY;

    UUID::UUID(const String& uuid)
    {
        memset(_data, 0, sizeof(_data));

        if (uuid.size() < 36)
            return;

        UINT32 idx = 0;

        // First group: 8 digits
        for (INT32 i = 7; i >= 0; --i)
        {
            char charVal = uuid[idx++];
            UINT8 hexVal = LITERAL_TO_HEX[(int)charVal];

            _data[0] |= hexVal << (i * 4);
        }

        idx++;

        // Second group: 4 digits
        for (INT32 i = 7; i >= 4; --i)
        {
            char charVal = uuid[idx++];
            UINT8 hexVal = LITERAL_TO_HEX[(int)charVal];

            _data[1] |= hexVal << (i * 4);
        }

        idx++;

        // Third group: 4 digits
        for (INT32 i = 3; i >= 0; --i)
        {
            char charVal = uuid[idx++];
            UINT8 hexVal = LITERAL_TO_HEX[(int)charVal];

            _data[1] |= hexVal << (i * 4);
        }

        idx++;

        // Fourth group: 4 digits
        for (INT32 i = 7; i >= 4; --i)
        {
            char charVal = uuid[idx++];
            UINT8 hexVal = LITERAL_TO_HEX[(int)charVal];

            _data[2] |= hexVal << (i * 4);
        }

        idx++;

        // Fifth group: 12 digits
        for (INT32 i = 3; i >= 0; --i)
        {
            char charVal = uuid[idx++];
            UINT8 hexVal = LITERAL_TO_HEX[(int)charVal];

            _data[2] |= hexVal << (i * 4);
        }

        for (INT32 i = 7; i >= 0; --i)
        {
            char charVal = uuid[idx++];
            UINT8 hexVal = LITERAL_TO_HEX[(int)charVal];

            _data[3] |= hexVal << (i * 4);
        }
    }

    String UUID::ToString() const
    {
        UINT8 output[36];
        UINT32 idx = 0;

        // First group: 8 digits
        for (INT32 i = 7; i >= 0; --i)
        {
            UINT32 hexVal = (_data[0] >> (i * 4)) & 0xF;
            output[idx++] = HEX_TO_LITERAL[hexVal];
        }

        output[idx++] = '-';

        // Second group: 4 digits
        for (INT32 i = 7; i >= 4; --i)
        {
            UINT32 hexVal = (_data[1] >> (i * 4)) & 0xF;
            output[idx++] = HEX_TO_LITERAL[hexVal];
        }

        output[idx++] = '-';

        // Third group: 4 digits
        for (INT32 i = 3; i >= 0; --i)
        {
            UINT32 hexVal = (_data[1] >> (i * 4)) & 0xF;
            output[idx++] = HEX_TO_LITERAL[hexVal];
        }

        output[idx++] = '-';

        // Fourth group: 4 digits
        for (INT32 i = 7; i >= 4; --i)
        {
            UINT32 hexVal = (_data[2] >> (i * 4)) & 0xF;
            output[idx++] = HEX_TO_LITERAL[hexVal];
        }

        output[idx++] = '-';

        // Fifth group: 12 digits
        for (INT32 i = 3; i >= 0; --i)
        {
            UINT32 hexVal = (_data[2] >> (i * 4)) & 0xF;
            output[idx++] = HEX_TO_LITERAL[hexVal];
        }

        for (INT32 i = 7; i >= 0; --i)
        {
            UINT32 hexVal = (_data[3] >> (i * 4)) & 0xF;
            output[idx++] = HEX_TO_LITERAL[hexVal];
        }

        return String((const char*)output, 36);
    }

    UUID UUIDGenerator::GenerateUUID()
    {
        ::UUID uuid;
        UuidCreate(&uuid);

        // Endianess might not be correct, but it shouldn't matter
        UINT32 data1 = uuid.Data1;
        UINT32 data2 = uuid.Data2 | (uuid.Data3 << 16);
        UINT32 data3 = uuid.Data3 | (uuid.Data4[0] << 16) | (uuid.Data4[1] << 24);
        UINT32 data4 = uuid.Data4[2] | (uuid.Data4[3] << 8) | (uuid.Data4[4] << 16) | (uuid.Data4[5] << 24);

        return UUID(data1, data2, data3, data4);
    }
}