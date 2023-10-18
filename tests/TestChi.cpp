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
static OperationResult getChi(Database& db) {
   OperationResult retval;
   Table& studenten = db.getTable("studenten");

   auto scan = make_unique<Tablescan>(studenten);
   const Register* semester = scan->getOutput("semester");
   const Register* name = scan->getOutput("name");
   vector<const Register*> output = {name};

   // find all students where semester num. is not 2
   //Register two;
   auto& two = *retval.registerContainer.emplace_back(make_unique<Register>());
   two.setInt(2);
   auto chi = make_unique<Chi>(move(scan), Chi::notEqual, semester, &two);
   const Register* chiResult = chi->getResult();

   auto select = make_unique<Selection>(move(chi), chiResult);
   auto project = make_unique<Projection>(move(select), output);

   retval.result = move(project);
   return retval;
}
//---------------------------------------------------------------------------
TEST_CASE("Chi") {
   Database db;
   db.createUniDb();

   // select s.name from studenten s where s.semester <> 2
   auto result = getChi(db);

   stringstream outp;
   Printer out(move(result.result), outp);

   out.open();
   while (out.next())
      ;
   out.close();

   REQUIRE(outp.str() == "Xenokrates\n"
                         "Jonas\n"
                         "Fichte\n"
                         "Aristoxenos\n"
                         "Schopenhauer\n"
                         "Carnap\n");
}
