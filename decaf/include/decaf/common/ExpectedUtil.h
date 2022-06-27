#ifndef DECAF_COMMON_EXPECTEDUTIL_H
#define DECAF_COMMON_EXPECTEDUTIL_H

#include <utility>
#include <type_traits>
#include <tl/expected.hpp>

#if defined(__GNUC__) || defined(__clang__)

#define EXPECTED_CHECK(result)                                              \
    ({                                                                      \
        decltype(result) _result = std::move(result);                       \
                                                                            \
        if (!_result) {                                                     \
            return tl::make_unexpected(std::move(_result).error());         \
        }                                                                   \
                                                                            \
        _result;                                                            \
    })

#define EXPECTED_TRY(result)                                                \
    ({                                                                      \
        EXPECTED_CHECK(result).value();                                     \
    })

#endif // g++ or clang

#endif // DECAF_COMMON_EXPECTEDUTIL_H

