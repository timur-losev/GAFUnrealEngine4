#include "GAFPluginLoaderPrivatePCH.h"
#include "GAFGeometry.h"

FGAFSize::FGAFSize()
{
}

FGAFSize::FGAFSize(float w, float h) : Width(w), Height(h)
{
}

FGAFSize::FGAFSize(const FGAFSize& other) : Width(other.Width), Height(other.Height)
{
}

FGAFSize::FGAFSize(const FVector2D& point) : Width(point.X), Height(point.Y)
{
}

FGAFSize& FGAFSize::operator= (const FGAFSize& other)
{
    SetSize(other.Width, other.Height);
    return *this;
}

FGAFSize& FGAFSize::operator= (const FVector2D& point)
{
    SetSize(point.X, point.Y);
    return *this;
}

FGAFSize FGAFSize::operator+(const FGAFSize& right) const
{
    return FGAFSize(this->Width + right.Width, this->Height + right.Height);
}

FGAFSize FGAFSize::operator-(const FGAFSize& right) const
{
    return FGAFSize(this->Width - right.Width, this->Height - right.Height);
}

FGAFSize FGAFSize::operator*(float a) const
{
    return FGAFSize(this->Width * a, this->Height * a);
}

FGAFSize FGAFSize::operator/(float a) const
{
    check(a != 0 && "FGAFSize division by 0.");
    return FGAFSize(this->Width / a, this->Height / a);
}

void FGAFSize::SetSize(float w, float h)
{
    this->Width = w;
    this->Height = h;
}

bool FGAFSize::Equals(const FGAFSize& target) const
{
    return (FMath::Abs(this->Width - target.Width) < FLT_EPSILON)
        && (FMath::Abs(this->Height - target.Height) < FLT_EPSILON);
}

const FGAFSize FGAFSize::ZERO = FGAFSize(0, 0);

/************************************************************************/
/* RECT                                                                 */
/************************************************************************/
FGAFRect::FGAFRect()
{
    SetRect(0.0f, 0.0f, 0.0f, 0.0f);
}

FGAFRect::FGAFRect(float x, float y, float width, float height)
{
    SetRect(x, y, width, height);
}

FGAFRect::FGAFRect(const FVector2D& pos, const FGAFSize& dimension)
{
    SetRect(pos.X, pos.Y, dimension.Width, dimension.Height);
}

FGAFRect::FGAFRect(const FGAFRect& other)
{
    SetRect(other.Origin.X, other.Origin.Y, other.Size.Width, other.Size.Height);
}

FGAFRect& FGAFRect::operator= (const FGAFRect& other)
{
    SetRect(other.Origin.X, other.Origin.Y, other.Size.Width, other.Size.Height);
    return *this;
}

void FGAFRect::SetRect(float x, float y, float width, float height)
{
    Origin.X = x;
    Origin.Y = y;

    Size.Width = width;
    Size.Height = height;
}

bool FGAFRect::Equals(const FGAFRect& rect) const
{
    return (Origin.Equals(rect.Origin, THRESH_VECTORS_ARE_NEAR) &&
            Size.Equals(rect.Size));
}

float FGAFRect::GetMaxX() const
{
    return Origin.X + Size.Width;
}

float FGAFRect::GetMidX() const
{
    return Origin.X + Size.Width / 2.0f;
}

float FGAFRect::GetMinX() const
{
    return Origin.X;
}

float FGAFRect::GetMaxY() const
{
    return Origin.Y + Size.Height;
}

float FGAFRect::GetMidY() const
{
    return Origin.Y + Size.Height / 2.0f;
}

float FGAFRect::GetMinY() const
{
    return Origin.Y;
}

bool FGAFRect::ContainsPoint(const FVector2D& point) const
{
    bool bRet = false;

    if (point.X >= GetMinX() && point.X <= GetMaxX()
        && point.Y >= GetMinY() && point.Y <= GetMaxY())
    {
        bRet = true;
    }

    return bRet;
}

bool FGAFRect::IntersectsRect(const FGAFRect& rect) const
{
    return !(     GetMaxX() < rect.GetMinX() ||
             rect.GetMaxX() <      GetMinX() ||
                  GetMaxY() < rect.GetMinY() ||
             rect.GetMaxY() <      GetMinY());
}

bool FGAFRect::IntersectsCircle(const FVector2D &center, float radius) const
{
    FVector2D rectangleCenter((Origin.X + Size.Width / 2),
                         (Origin.Y + Size.Height / 2));
    
    float w = Size.Width / 2.f;
    float h = Size.Height / 2.f;
    
    float dx = FMath::Abs(center.X - rectangleCenter.X);
    float dy = FMath::Abs(center.Y - rectangleCenter.Y);
    
    if (dx > (radius + w) || dy > (radius + h))
    {
        return false;
    }
    
    FVector2D circleDistance(FMath::Abs(center.X - Origin.X - w),
                        FMath::Abs(center.Y - Origin.Y - h));
    
    if (circleDistance.X <= (w))
    {
        return true;
    }
    
    if (circleDistance.Y <= (h))
    {
        return true;
    }
    
    float cornerDistanceSq = FMath::Pow(circleDistance.X - w, 2.f) + FMath::Pow(circleDistance.Y - h, 2.f);
    
    return (cornerDistanceSq <= (FMath::Pow(radius, 2.f)));
}

void FGAFRect::Merge(const FGAFRect& rect)
{
    float top1 = GetMaxY();
    float left1 = GetMinX();
    float right1 = GetMaxX();
    float bottom1 = GetMinY();
    
    float top2 = rect.GetMaxY();
    float left2 = rect.GetMinX();
    float right2 = rect.GetMaxX();
    float bottom2 = rect.GetMinY();
    Origin.X = FMath::Min(left1, left2);
    Origin.Y = FMath::Min(bottom1, bottom2);
    Size.Width = FMath::Max(right1, right2) - Origin.X;
    Size.Height = FMath::Max(top1, top2) - Origin.Y;
}

FGAFRect FGAFRect::UnionWithRect(const FGAFRect & rect) const
{
    float thisLeftX = Origin.X;
    float thisRightX = Origin.X + Size.Width;
    float thisTopY = Origin.Y + Size.Height;
    float thisBottomY = Origin.Y;
    
    if (thisRightX < thisLeftX)
    {
        std::swap(thisRightX, thisLeftX);   // This rect has negative width
    }
    
    if (thisTopY < thisBottomY)
    {
        std::swap(thisTopY, thisBottomY);   // This rect has negative height
    }
    
    float otherLeftX = rect.Origin.X;
    float otherRightX = rect.Origin.X + rect.Size.Width;
    float otherTopY = rect.Origin.Y + rect.Size.Height;
    float otherBottomY = rect.Origin.Y;
    
    if (otherRightX < otherLeftX)
    {
        std::swap(otherRightX, otherLeftX);   // Other rect has negative width
    }
    
    if (otherTopY < otherBottomY)
    {
        std::swap(otherTopY, otherBottomY);   // Other rect has negative height
    }
    
    float combinedLeftX = FMath::Min(thisLeftX, otherLeftX);
    float combinedRightX = FMath::Max(thisRightX, otherRightX);
    float combinedTopY = FMath::Max(thisTopY, otherTopY);
    float combinedBottomY = FMath::Min(thisBottomY, otherBottomY);
    
    return FGAFRect(combinedLeftX, combinedBottomY, combinedRightX - combinedLeftX, combinedTopY - combinedBottomY);
}

const FGAFRect FGAFRect::ZERO = FGAFRect(0, 0, 0, 0);

const FGAFAffineTransform FGAFAffineTransform::IDENTITY = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
