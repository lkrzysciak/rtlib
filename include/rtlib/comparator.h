#pragma once

#include <stdbool.h>
#include <string.h>

#if defined(__GNUC__) || defined(__clang__)
#define RTLIB__COMPARATOR_DEPRECATED(msg) __attribute__((deprecated(msg)))
#else
#define RTLIB__COMPARATOR_DEPRECATED(msg)
#endif

RTLIB__COMPARATOR_DEPRECATED(
    "rtlib: comparator fallback uses memcmp; consider implementing a custom comparator for this type")
static inline int rtlib_MemcmpFallbackCompare(const void * v1, const void * v2, size_t type_size)
{
    return memcmp(v1, v2, type_size);
}

static inline int rtlib_MemcmpFallbackCompareRaw(const void * v1, const void * v2, size_t type_size)
{
    return memcmp(v1, v2, type_size);
}

#ifdef __cplusplus
#include <type_traits>

namespace rtlib_detail {
template<typename T,
         bool IsFastPath = (std::is_arithmetic<T>::value || std::is_enum<T>::value || std::is_pointer<T>::value)>
struct ComparatorCppImpl;

template<typename T>
struct ComparatorCppImpl<T, true>
{
    static inline int Compare(const T * v1, const T * v2) { return (((*v1) > (*v2)) - ((*v1) < (*v2))); }
};

template<typename T>
struct ComparatorCppImpl<T, false>
{
    static inline int Compare(const T * v1, const T * v2) { return rtlib_MemcmpFallbackCompare(v1, v2, sizeof(T)); }
};

template<typename T>
inline int CompareCpp(const T * v1, const T * v2)
{
    return ComparatorCppImpl<T>::Compare(v1, v2);
}
}  // namespace rtlib_detail
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(__cplusplus) && defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#define RTLIB__COMPARE_SCALAR(a, b) (((a) > (b)) - ((a) < (b)))
#define RTLIB__COMPARATOR_GENERIC_KEY(v) (+(*(v)))

#define comparator(type)                                                    \
    static inline int type##_Compare(const type * v1, const type * v2)      \
    {                                                                       \
        int result = _Generic(RTLIB__COMPARATOR_GENERIC_KEY(v1),            \
            int: RTLIB__COMPARE_SCALAR(*v1, *v2),                           \
            unsigned int: RTLIB__COMPARE_SCALAR(*v1, *v2),                  \
            long: RTLIB__COMPARE_SCALAR(*v1, *v2),                          \
            unsigned long: RTLIB__COMPARE_SCALAR(*v1, *v2),                 \
            long long: RTLIB__COMPARE_SCALAR(*v1, *v2),                     \
            unsigned long long: RTLIB__COMPARE_SCALAR(*v1, *v2),            \
            double: RTLIB__COMPARE_SCALAR(*v1, *v2),                        \
            long double: RTLIB__COMPARE_SCALAR(*v1, *v2),                   \
            default: rtlib_MemcmpFallbackCompareRaw(v1, v2, sizeof(type))); \
        return result;                                                      \
    }
#elif defined(__cplusplus)
#define comparator(type)                                               \
    static inline int type##_Compare(const type * v1, const type * v2) \
    {                                                                  \
        return rtlib_detail::CompareCpp<type>(v1, v2);                 \
    }
#else
#define comparator(type)                                               \
    static inline int type##_Compare(const type * v1, const type * v2) \
    {                                                                  \
        return rtlib_MemcmpFallbackCompare(v1, v2, sizeof(type));      \
    }
#endif

#define comparator_impl(type) comparator(type)

#ifdef __cplusplus
}
#endif