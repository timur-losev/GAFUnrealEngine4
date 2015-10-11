#pragma once

#include "GAFHeader.h"

class FGAFData
{
private:
    uint8*      Data = nullptr;
    uint32      DataPosition = 0;
    uint32      DataLen = 0;

    FGAFHeader  Header;

    void                    _ReadHeaderBegin(FGAFHeader&);
    bool                    _ProcessOpen();

public:

    template<typename T>
    typename TEnableIf<TIsArithmeticType<T>::Value>::Type
        Read(T& InOut)
    {
        ReadBytes(&InOut, sizeof(T));
    }

    bool                    IsEOF() const;

    SIZE_T                  ReadString(FString* dst); // function reads length prefixed string

    void                    ReadBytes(void* dst, uint32 len);

    void                    Close();


    bool                    Open(const uint8* data, SIZE_T len);

    bool                    IsOpened() const;

    const FGAFHeader&       GetHeader() const;
    FGAFHeader&             GetHeader();

    uint32                  GetPosition() const;
    void                    Rewind(uint32 newPos);

    FGAFData();
    ~FGAFData();

};