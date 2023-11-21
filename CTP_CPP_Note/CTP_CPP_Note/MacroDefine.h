#pragma once
#include<memory>

#define SHARED_PTR(NAME)                                 \
using NAME ## Ptr = std::shared_ptr< NAME >;             \
using NAME ## ConstPtr = std::shared_ptr< const NAME >;



