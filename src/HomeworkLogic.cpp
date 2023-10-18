#include "HomeworkLogic.hpp"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
size_t HomeworkLogic::addHandler(const string& name, function<string(int argc, const char* const* argv)> handler)
// Add a homework handler
{
   handlers[name] = move(handler);
   return handlers.size();
}
//---------------------------------------------------------------------------
HomeworkLogic& HomeworkLogic::getInstance()
// Get homework logic instance
{
   static HomeworkLogic instance;
   return instance;
}
//---------------------------------------------------------------------------
std::string HomeworkLogic::run(const string& name, int argc, const char* const* argv)
// Run a homework handler
{
   auto it = handlers.find(name);
   if (it == handlers.end())
      return "MISSING";
   return it->second(argc, argv);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------