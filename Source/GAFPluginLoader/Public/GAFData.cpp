#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFData.h"

void FGAFData::_ReadHeaderBegin(FGAFHeader& header)
{
    ReadBytes(&header.Compression, 4);
    Read(header.Version);
    Read(Header.FileLenght);
}

bool FGAFData::IsEOF() const
{
    return DataPosition >= DataLen;
}

SIZE_T FGAFData::ReadString(FString* dst)
{
    verify(dst != nullptr);
    verify(DataPosition + sizeof(uint16) <= DataLen);

    uint16 Len = 0;
    Read(Len);

    verify(DataPosition + Len <= DataLen);
    ANSICHAR* StrData = static_cast<ANSICHAR*>(FMemory::Malloc(Len + 1));
    ReadBytes(StrData, Len);
    StrData[Len] = 0;

    {
        FString tempStr(StrData);
        dst->Append(tempStr);
    }

    FMemory::Free(StrData);

    return SIZE_T(dst->Len() + sizeof(uint16));
}

void FGAFData::ReadBytes(void* dst, uint32 len)
{
    verify(dst && Data);
    verify(DataPosition + len <= DataLen);
    FMemory::Memcpy(dst, Data + DataPosition, len);
    DataPosition += len;
}

void FGAFData::Close()
{
    FMemory::Free(Data);
    Data = nullptr;
    DataLen = 0;
    DataPosition = 0;
}

bool FGAFData::Open(const uint8* InData, SIZE_T InLen)
{
    Close();

    Data = static_cast<uint8*>(FMemory::Malloc(sizeof(uint8) * InLen));
    FMemory::Memcpy(Data, InData, InLen);
    DataLen = InLen;

    return _ProcessOpen();
}

bool FGAFData::_ProcessOpen()
{
    _ReadHeaderBegin(Header);

    // 10 = sizeof(Footprint) + sizeof(FileLen) + sizeof(Version)
    static const uint32 UncompressedDataSize = 10;
    verify(DataPosition == UncompressedDataSize);

    if (Header.Compression == FGAFHeader::CompressedNone)
    {

    }
    else if (Header.Compression == FGAFHeader::CompressedZip)
    {
        //TODO
    }
    else
    {
        return false;
    }

    return true;
}

bool FGAFData::IsOpened() const
{
    return !!Data;
}

const FGAFHeader& FGAFData::GetHeader() const
{
    return Header;
}

FGAFHeader& FGAFData::GetHeader()
{
    return Header;
}

uint32 FGAFData::GetPosition() const
{
    return DataPosition;
}

void FGAFData::Rewind(uint32 newPos)
{
    DataPosition = newPos;
}

FGAFData::FGAFData()
{

}

FGAFData::~FGAFData()
{
    Close();
}
