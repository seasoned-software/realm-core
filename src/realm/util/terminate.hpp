/*************************************************************************
 *
 * REALM CONFIDENTIAL
 * __________________
 *
 *  [2011] - [2012] Realm Inc
 *  All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains
 * the property of Realm Incorporated and its suppliers,
 * if any.  The intellectual and technical concepts contained
 * herein are proprietary to Realm Incorporated
 * and its suppliers and may be covered by U.S. and Foreign Patents,
 * patents in process, and are protected by trade secret or copyright law.
 * Dissemination of this information or reproduction of this material
 * is strictly forbidden unless prior written permission is obtained
 * from Realm Incorporated.
 *
 **************************************************************************/
#ifndef REALM_UTIL_TERMINATE_HPP
#define REALM_UTIL_TERMINATE_HPP

#include <cstdlib>
#include <sstream>

#include <realm/util/features.h>
#include <realm/util/inspect.hpp>
#include <realm/version.hpp>

#define REALM_TERMINATE(msg) realm::util::terminate((msg), __FILE__, __LINE__)

namespace realm {
namespace util {
REALM_NORETURN void terminate_internal(std::stringstream&) noexcept;

REALM_NORETURN void terminate(const char* message, const char* file, long line) noexcept;

// LCOV_EXCL_START
template<class... Ts>
REALM_NORETURN void terminate(const char* message, const char* file, long line,
                              Ts... infos) noexcept
{
    std::stringstream ss;
    static_assert(sizeof...(infos) == 2 || sizeof...(infos) == 4 || sizeof...(infos) == 6,
                  "Called realm::util::terminate() with wrong number of arguments");
    ss << file << ':' << line << ": " REALM_VER_CHUNK " " << message << " [";
    inspect_all(ss, std::forward<Ts>(infos)...);
    ss << "]" << '\n';

    terminate_internal(ss);
}
// LCOV_EXCL_STOP

template<class... Args>
REALM_NORETURN void terminate_with_info(const char* assert_message, int line, const char* file,
                                        const char* interesting_names,
                                        Args&&... interesting_values) noexcept
{
    std::stringstream ss;
    ss << file << ':' << line << ": " REALM_VER_CHUNK " ";
    ss << assert_message << " with " << interesting_names << " = (";
    inspect_all(ss, std::forward<Args>(interesting_values)...);
    ss << "). \n";
    terminate_internal(ss);
}

} // namespace util
} // namespace realm

#endif // REALM_UTIL_TERMINATE_HPP
