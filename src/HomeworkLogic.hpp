#include <functional>
#include <string>
#include <unordered_map>
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
/// Logic for handling homework
class HomeworkLogic {
   /// Homework handlers
   std::unordered_map<std::string, std::function<std::string(int argc, const char* const* argv)>> handlers;

   public:
   /// Add a homework handler
   size_t addHandler(const std::string& name, std::function<std::string(int argc, const char* const* argv)> handler);
   /// Run a homework handler
   std::string run(const std::string& name, int argc, const char* const* argv);
   /// Run a homework handler
   std::string run(const std::string& name) {
      const char* str[] = {name.c_str()};
      return run(name, 1, str);
   }

   /// Get homework logic instance
   static HomeworkLogic& getInstance();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------