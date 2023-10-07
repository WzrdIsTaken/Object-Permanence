#pragma once
#include "EngineCore.h"

#include <functional>

// I added the DEFAULT variation of these macros on 08/09/23. They could be parameterized more to make it one set, 
// but the DELETE ones are already being used in lots of places around the code and I cba to refactor. 
// Also, perhaps the explicitness is good. 

#define DELETE_COPY_MOVE_CONSTRUCTORS(type) \
    DELETE_COPY_CONSTRUCTORS(type) \
    DELETE_MOVE_CONSTRUCTORS(type)

#define DELETE_COPY_CONSTRUCTORS(type) \
     type(type&) = delete; \
     type(type const&) = delete; \

#define DELETE_MOVE_CONSTRUCTORS(type) \
    type(type&&) = delete; \
    type(type const&&) = delete;

#define DEFAULT_COPY_MOVE_CONSTRUCTORS(type) \
    DEFAULT_COPY_CONSTRUCTORS(type) \
    DEFAULT_MOVE_CONSTRUCTORS(type)

#define DEFAULT_COPY_CONSTRUCTORS(type) \
     type(type&) = default; \
     type(type const&) = default; \

#define DEFAULT_MOVE_CONSTRUCTORS(type) \
    type(type&&) = default; \
    type(type const&&) = default;

namespace nabi::TypeUtils
{
    // Note - this has some flaws / limitations
    // See https://stackoverflow.com/questions/20833453/comparing-stdfunctions-for-equality

    template<typename T, typename... U>
    bool CompairStdFunctionEquality(std::function<T(U...)> lhs, std::function<T(U...)> rhs)
    {
        bool const equal = GetAddress(lhs) == GetAddress(rhs);
        return equal;
    }

    template<typename T, typename... U>
    size_t GetAddress(std::function<T(U...)> function) 
    {
        typedef T(FunctionType)(U...);
        FunctionType** functionPointer = function.template target<FunctionType*>();
        return (size_t)*functionPointer;
    }

    template <typename E>
    constexpr typename std::underlying_type<E>::type ToUnderlying(E e) noexcept 
    {
        // Note - to_underlying will do this in cpp23
        return static_cast<typename std::underlying_type<E>::type>(e);
    }
} // namespace nabi::TypeUtils

//#define interface __interface

// ngl i don't think 'TypeUtils' is the best name... if I think of something better I'll change it!
// types are classes and structs.. so that was my reasoning. but 'ClassStructUtils' seemed even worse xD
// but now i've started using this in namespaces as well. gj ben good name
