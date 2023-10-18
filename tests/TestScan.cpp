#include "Database.hpp"
#include "operator/Tablescan.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <catch2/catch_test_macros.hpp>
//---------------------------------------------------------------------------
using namespace std;
using namespace tinydb;
//---------------------------------------------------------------------------
TEST_CASE("Scan") {
   Database db;
   db.createUniDb();
   Table& studenten = db.getTable("studenten");

   Tablescan scan(studenten);
   const Register* name = scan.getOutput("name");

   stringstream result;
   scan.open();
   while (scan.next())
      result << name->getString() << '\n';
   scan.close();

   REQUIRE(result.str() == "Xenokrates\n"
                           "Jonas\n"
                           "Fichte\n"
                           "Aristoxenos\n"
                           "Schopenhauer\n"
                           "Carnap\n"
                           "Theophrastos\n"
                           "Feuerbach\n");
}
