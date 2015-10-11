#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFStream.h"
#include "GAFData.h"

FGAFStream::FGAFStream(FGAFData* input):
    Input(input),
    CurrentByte(0),
    UnusedBits(0)
{
    verify(Input != nullptr);
}

FGAFStream::~FGAFStream()
{

}

void FGAFStream::ReadNBytesOfT(void* dest, uint32 n)
{
    Align();
    Input->ReadBytes(dest, n);
}

bool FGAFStream::ReadBool()
{
    return ReadUint(1) ? true : false;
}

// Reads a bit-packed unsigned integer from the stream
// and returns it.  The given bitcount determines the
// number of bits to read.

uint32 FGAFStream::ReadUint(uint16 bitcount)
{
    verify(bitcount <= 32);

    uint32 retval = 0;

    while (bitcount)
    {
        if (UnusedBits)
        {
            if (bitcount >= UnusedBits)
            {
                retval |= (CurrentByte << (bitcount - UnusedBits));
                bitcount -= UnusedBits;

                CurrentByte = 0;
                UnusedBits = 0;
            }
            else
            {
                retval |= (CurrentByte >> (UnusedBits - bitcount));
                CurrentByte &= ((1 << (UnusedBits - bitcount)) - 1);
                UnusedBits -= bitcount;

                bitcount = 0;
            }
        }
        else
        {
            Input->Read(CurrentByte);
            UnusedBits = 8;
        }
    }

    verify(bitcount == 0);

    return retval;
}

// Reads a bit-packed little-endian signed integer
// from the stream.  The given bitcount determines the
// number of bits to read.
int32 FGAFStream::ReadSint(uint16 bitcount)
{
    verify(bitcount <= 32);

    int32 retval = (int32)ReadUint(bitcount);

    if (retval & (1 << (bitcount - 1)))
    {
        retval |= -1 << bitcount;
    }

    return retval;
}

float FGAFStream::ReadFixed()
{
    int32 retval = 0;
    Input->Read(retval);
    return static_cast<float>(retval / 65536.0f);
}

float FGAFStream::ReadFixed8()
{
    int16_t retval = 0;
    Input->Read(retval);
    return static_cast<float>(retval / 255.0f);
}

float FGAFStream::ReadFloat()
{
    float retval = 0.f;
    Input->Read(retval);
    return retval;
}

uint8 FGAFStream::ReadUByte()
{
    Align();
    uint8 retval = 0;
    Input->Read(retval);
    return retval;
}

char FGAFStream::ReadSByte()
{
    Align();
    int8 retval = 0;
    Input->Read(retval);
    return retval;
}

uint16 FGAFStream::ReadU16()
{
    uint16 retval = 0;
    ReadNBytesOfT(&retval, sizeof(uint16));
    return retval;
}

uint32 FGAFStream::ReadU32()
{
    Align();
    uint32 retval = 0;
    Input->Read(retval);
    return retval;
} 

int32 FGAFStream::ReadS32()
{
    Align();
    int32 retval = 0;
    Input->Read(retval);
    return retval;
}

void FGAFStream::ReadString(FString* out)
{
    Input->ReadString(out);
}

FGAFData* FGAFStream::GetInput() const
{
    return Input;
}

FTags::Enum FGAFStream::OpenTag()
{
    Align();

    uint16 TagType = ReadU16();
    uint32 TagLength = ReadU32();

    verify(UnusedBits == 0);

    TagRecord record = { Input->GetPosition() + TagLength, TagLength, (FTags::Enum)TagType };

    TagStack.AddHead(record);

    return (FTags::Enum)TagType;
}

void FGAFStream::CloseTag()
{
    verify(TagStack.Num() > 0);

    TagStack_t::TDoubleLinkedListNode* top = TagStack.GetHead();
    TagRecord record = top->GetValue();
    TagStack.RemoveNode(top);

    const uint32 inputPosition = Input->GetPosition();

    verify(record.expectedStreamPos == inputPosition);

    if (record.expectedStreamPos != inputPosition)
    {
        //TODO : Log
        //CCLOGERROR("Tag [%s] hasn't been correctly read, tag length is not respected. Expected [%d] but actually [%d]", 
            //Tags::toString(record.tagType).c_str(), record.expectedStreamPos, inputPosition);
    }

    Input->Rewind(record.expectedStreamPos);
    UnusedBits = 0;
}

uint32 FGAFStream::GetTagLenghtOnStackTop() const
{
    verify(TagStack.Num() > 0);

    return TagStack.GetHead()->GetValue().tagSize;
}

uint32 FGAFStream::GetTagExpectedPosition() const
{
    verify(TagStack.Num() > 0);

    return TagStack.GetHead()->GetValue().expectedStreamPos;
}

uint32 FGAFStream::GetPosition() const
{
    return Input->GetPosition();
}

void FGAFStream::Align()
{
    CurrentByte = UnusedBits = 0;
}

bool FGAFStream::IsEndOfStream() const
{
    return Input->IsEOF();
}
