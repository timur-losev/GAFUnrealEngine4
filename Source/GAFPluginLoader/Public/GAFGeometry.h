#pragma once

class FGAFSize
{
public:
    float Width = 0.f;
    float Height = 0.f;
public:
    FGAFSize();
    FGAFSize(float width, float height);
    FGAFSize(const FGAFSize& other);
    explicit FGAFSize(const FVector2D& point);

    FGAFSize& operator= (const FGAFSize& other);

    FGAFSize& operator= (const FVector2D& point);

    FGAFSize operator+(const FGAFSize& right) const;

    FGAFSize operator-(const FGAFSize& right) const;

    FGAFSize operator*(float a) const;

    FGAFSize operator/(float a) const;

    void SetSize(float width, float height);

    bool Equals(const FGAFSize& target) const;

    static const FGAFSize ZERO;
};

class FGAFRect
{
public:
    /**Low left point of rect.*/
    FVector2D Origin;
    /**Width and height of the rect.*/
    FGAFSize  Size;

public:
    FGAFRect();

    FGAFRect(float x, float y, float width, float height);

    FGAFRect(const FVector2D& pos, const FGAFSize& dimension);

    FGAFRect(const FGAFRect& other);

    FGAFRect& operator= (const FGAFRect& other);

    void SetRect(float x, float y, float width, float height);

    float GetMinX() const;
    float GetMidX() const;
    float GetMaxX() const;
    float GetMinY() const;
    float GetMidY() const;
    float GetMaxY() const;
    bool Equals(const FGAFRect& rect) const;
    
    bool ContainsPoint(const FVector2D& point) const;

    bool IntersectsRect(const FGAFRect& rect) const;

    bool IntersectsCircle(const FVector2D& center, float radius) const;

    FGAFRect UnionWithRect(const FGAFRect & rect) const;
    
    void Merge(const FGAFRect& rect);

    static const FGAFRect ZERO;
};

typedef FVector2D FGAFPoint_t;

struct FGAFAffineTransform 
{
    float a, b, c, d;
    float tx, ty;

    static const FGAFAffineTransform IDENTITY;
};