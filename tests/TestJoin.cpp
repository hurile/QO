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
static OperationResult getJoin(Database& db) {
   Table& professoren = db.getTable("professoren");
   Table& vorlesungen = db.getTable("vorlesungen");

   auto scanProfessoren = make_unique<Tablescan>(professoren);
   auto scanVorlesungen = make_unique<Tablescan>(vorlesungen);

   const Register* name = scanProfessoren->getOutput("name");
   const Register* persnr = scanProfessoren->getOutput("persnr");
   const Register* titel = scanVorlesungen->getOutput("titel");
   const Register* gelesenvon = scanVorlesungen->getOutput("gelesenvon");

   auto cp = make_unique<CrossProduct>(move(scanProfessoren), move(scanVorlesungen));
   auto select = make_unique<Selection>(move(cp), persnr, gelesenvon);
   auto project = make_unique<Projection>(move(select), vector<const Register*>{name, titel});

   return OperationResult{{}, move(project)};
}
//---------------------------------------------------------------------------
TEST_CASE("Join") {
   Database db;
   db.createUniDb();

   auto result = getJoin(db);

   stringstream outp;
   Printer out(move(result.result), outp);

   out.open();
   while (out.next())
      ;
   out.close();

   REQUIRE(outp.str() == "Sokrates Ethik\n"
                         "Sokrates Mäeutik\n"
                         "Sokrates Logik\n"
                         "Russel Erkenntnistheorie\n"
                         "Russel Wissenschaftstheorie\n"
                         "Russel Bioethik\n"
                         "Popper Der Wiener Kreis\n"
                         "Augustinus Glaube und Wissen\n"
                         "Kant Grundzüge\n"
                         "Kant Die 3 Kritiken\n");
}
