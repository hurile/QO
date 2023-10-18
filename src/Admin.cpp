#include "Admin.hpp"
#include <fstream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
void Admin::createTable(Database& db, string_view tableName, const vector<string_view>& rest) {
   Table& table = db.createTable(tableName);
   for (int index = 0; index < rest.size(); ++index) {
      string_view name = rest[index++];
      bool key = false;
      if ((index < rest.size()) && (string(rest[index]) == "key")) {
         key = true;
         index++;
      }
      if (index >= rest.size()) {
         cerr << "invalid attribute specification" << endl;
         return;
      }
      Attribute::Type type;
      string_view t = rest[index];
      if (t == "int")
         type = Attribute::Type::Int;
      else if (t == "double")
         type = Attribute::Type::Double;
      else if (t == "bool")
         type = Attribute::Type::Bool;
      else if (t == "string")
         type = Attribute::Type::String;
      else {
         cerr << "invalid attribute type " << t << endl;
         return;
      }
      table.addAttribute(name, type, key);
   }
}
//---------------------------------------------------------------------------
void Admin::bulkLoad(Database& db, Table& table, string_view file) {
   ifstream in(string{file});
   if (!in.is_open()) {
      cerr << "unable to open " << file << endl;
      return;
   }
   vector<Register> values;
   values.resize(table.getAttributeCount());
   string line;
   while (getline(in, line)) {
      auto last = line.begin();
      auto writer = 0u;
      for (auto iter = line.begin(), limit = line.end(); iter != limit; ++iter)
         if ((*iter) == '|') {
            string d = string(last, iter);
            switch (table.getAttribute(writer).getType()) {
               case Attribute::Type::Int: values[writer].setInt(atoi(d.c_str())); break;
               case Attribute::Type::Double: values[writer].setDouble(atof(d.c_str())); break;
               case Attribute::Type::Bool: values[writer].setBool(d == "true"); break;
               default: values[writer].setString(d);
            }
            ++writer;
            last = iter + 1;
         }
      table.insertValues(values);
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
