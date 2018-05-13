#pragma once

/// Wrap hwlib to silence warnings.

#pragma GCC system_header /// Hush...
#include <hwlib.hpp>

/// Little hack for adding a endline with carrage return to the hwlib namespace, written by Wiebe van Breukelen (@wvanbreukelen)
/// Maybe an opportunity for the leads to standardlize this. It may also be removed.
namespace hwlib {
constexpr char endlRet[3] = "\r\n";
}
