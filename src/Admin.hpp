#ifndef H_operator_Admin
#define H_operator_Admin
//---------------------------------------------------------------------------
#include "Database.hpp"
#include <string>
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
/// Interface to easily interact with database
struct Admin {
   /// Create a table
   static void createTable(Database& db, std::string_view tableName, const std::vector<std::string_view>& rest);
   /// Load values in bulk
   static void bulkLoad(Database& db, Table& table, std::string_view rest);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
