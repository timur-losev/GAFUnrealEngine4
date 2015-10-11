#pragma once

#include "UnrealEd.h"
#include <unordered_map>
#include <tuple>
#include <list>
#include <limits>


#define ForwardSharedPointer(__class__)\
class __class__;\
typedef TSharedPtr<__class__> __class__##Ptr;\
typedef TSharedPtr<__class__ const> __class__##ConstPtr

#define ForwardWeakPointer(__class__)\
class __class__;\
typedef TWeakPtr<__class__> __class__##WeakPtr;\
typedef TWeakPtr<__class__ const> __class__##WeakConstPtr
