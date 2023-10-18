#include "Database.hpp"
#include "operator/Chi.hpp"
#include "operator/CrossProduct.hpp"
#include "operator/Printer.hpp"
#include "operator/Projection.hpp"
#include "operator/Selection.hpp"
#include "operator/Tablescan.hpp"
#include <vector>
#include <sstream>
#include <catch2/catch_test_macros.hpp>
//---------------------------------------------------------------------------
using namespace std;
using namespace tinydb;
//---------------------------------------------------------------------------
namespace {
//---------------------------------------------------------------------------
struct OperationResult {
   vector<unique_ptr<Register>> registerContainer;
   unique_ptr<Operator> result;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
static OperationResult getSelect(Database& db) {
   OperationResult retval;
   Table& studenten = db.getTable("studenten");

   auto scan = make_unique<Tablescan>(studenten);
   const Register* name = scan->getOutput("name");;
   const Register* semester = scan->getOutput("semester");
   //Register two;
   auto& two = *retval.registerContainer.emplace_back(make_unique<Register>());
   two.setInt(2);
   unique_ptr<Operator> op = make_unique<Selection>(move(scan), semester, &two);
   op = make_unique<Projection>(move(op), vector<const Register*>{name});
   retval.result = move(op);
   return retval;
}
//---------------------------------------------------------------------------
TEST_CASE("Select") {
   Database db;
   db.createUniDb();

   auto result = getSelect(db);

   stringstream outp;
   Printer out(move(result.result), outp);

   out.open();
   while (out.next())
      ;
   out.close();

   REQUIRE(outp.str() == "Theophrastos\n"
                         "Feuerbach\n");
}
