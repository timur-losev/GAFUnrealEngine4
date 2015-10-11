#pragma once

#include "GAFTagDefines.h"

class FGAFData;

class FGAFStream
{
private:
    FGAFData*    Input = nullptr;
    uint8        CurrentByte = 0;
    uint8        UnusedBits = 0;

    struct TagRecord
    {
        uint32 expectedStreamPos;
        uint32 tagSize;
        FTags::Enum tagType;
    };

    typedef TDoubleLinkedList<TagRecord> TagStack_t;
    TagStack_t TagStack;

public:
    FGAFStream(FGAFData*);
    ~FGAFStream();

    void                 ReadNBytesOfT(void* dest, uint32);
    bool                 ReadBool();
    uint32               ReadUint(uint16 bitcount);
    int32                ReadSint(uint16 bitcount);
    float                ReadFixed();
    float                ReadFixed8();

    float                ReadFloat();

    uint8                ReadUByte();
    char                 ReadSByte();

    uint8                ReadU8() { return ReadUByte(); }
    int8                 ReadS8() { return ReadSByte(); }
    uint16               ReadU16();
    uint32               ReadU32();
    int32                ReadS32();

    void                 ReadString(FString* out);

    FGAFData*            GetInput() const;

    FTags::Enum          OpenTag();
    void                 CloseTag();
    uint32               GetTagLenghtOnStackTop() const;
    uint32               GetTagExpectedPosition() const;
    uint32               GetPosition() const;

    void                 Align();

    bool                 IsEndOfStream() const;

};