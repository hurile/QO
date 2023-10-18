#ifndef H_Config
#define H_Config
//---------------------------------------------------------------------------
#include <cassert>
#include <stdexcept>
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
/// Unreachable code
[[noreturn]] inline void unreachable() noexcept {
   assert(false && "unreachable");
#ifdef __GNUC__
   __builtin_unreachable();
#elif defined(_MSC_VER)
   __assume(false);
#else
   std::abort();
#endif
}
//---------------------------------------------------------------------------
/// Error indicating that a function is not implemented (should be implemented by the student)
struct NotImplementedError : std::runtime_error {
   /// Constructor
   using std::runtime_error::runtime_error;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
