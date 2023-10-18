#include "Admin.hpp"
#include "Attribute.hpp"
#include "Database.hpp"
#include "HomeworkLogic.hpp"
#include "JoinQuery.hpp"
#include "QueryGraph.hpp"
#include "Table.hpp"
#include "operator/Printer.hpp"
#include "operator/Tablescan.hpp"
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
static void initDB(const string& name)
// Create a new database
{
   Database db;
   db.create(name);
   db.close();

   cout << "ok" << endl;
}
//---------------------------------------------------------------------------
static void createTable(const string& dbName, const string& tableName, int count, char* rest[])
// Create a new table
{
   vector<string_view> restView;
   restView.reserve(count);
   for (unsigned i = 0; i < count; i++)
      restView.emplace_back(rest[i]);

   Database db;
   db.open(dbName);
   Admin::createTable(db, tableName, restView);
   db.close();
   cout << "ok" << endl;
}
//---------------------------------------------------------------------------
static void dropTable(const string& dbName, const string& tableName)
// Remove a table
{
   Database db;
   db.open(dbName);
   db.dropTable(tableName);
   db.close();
   cout << "ok" << endl;
}
//---------------------------------------------------------------------------
static void insertValues(const string& dbName, const string& tableName, unsigned count, char* rest[])
// Insert values into a database
{
   Database db;
   db.open(dbName);
   if (!db.hasTable(tableName)) {
      cerr << "unknown table " << tableName << endl;
      return;
   }
   Table& table = db.getTable(tableName);
   if (count != table.getAttributeCount()) {
      cerr << "the table has " << table.getAttributeCount() << " columns, " << count << " values were provided" << endl;
      return;
   }

   vector<Register> values;
   for (unsigned index = 0; index < count; index++) {
      Register r;
      const Attribute& a = table.getAttribute(index);
      switch (a.getType()) {
         case Attribute::Type::Int: r.setInt(atoi(rest[index])); break;
         case Attribute::Type::Double: r.setDouble(atof(rest[index])); break;
         case Attribute::Type::Bool: r.setBool(string(rest[index]) == "true"); break;
         default: r.setString(rest[index]);
      }
      values.push_back(r);
   }
   table.insertValues(values);
   db.close();
   cout << "ok" << endl;
}
//---------------------------------------------------------------------------
static void bulkload(const string& dbName, const string& tableName, int count, char* rest[])
// Bulkload
{
   Database db;
   db.open(dbName);
   if (!db.hasTable(tableName)) {
      cerr << "unknown table " << tableName << endl;
      return;
   }
   Table& table = db.getTable(tableName);
   if (count != 1) {
      cerr << "source file expected" << endl;
      return;
   }

   Admin::bulkLoad(db, table, rest[0]);

   db.close();
   cout << "ok" << endl;
}
//---------------------------------------------------------------------------
static void dumpTable(const string& dbName, const string& tableName)
// Show the content of a table
{
   Database db;
   db.open(dbName);
   if (!db.hasTable(tableName)) {
      cerr << "unknown table " << tableName << endl;
      return;
   }
   Table& table = db.getTable(tableName);

   std::unique_ptr<Tablescan> scan(new Tablescan(table));
   Printer p(move(scan), cout);
   p.open();
   while (p.next())
      ;
   p.close();

   db.close();
   cout << "ok" << endl;
}
//---------------------------------------------------------------------------
static void runStats(const string& dbName)
// Update the statistics
{
   Database db;
   db.open(dbName);
   db.runStats();
   db.close();
   cout << "ok" << endl;
}
//---------------------------------------------------------------------------
static void runHomework(const string& hw, int argc, const char* const* argv)
// Run a homework task
{
   cout << HomeworkLogic::getInstance().run(hw, argc, argv) << endl;
}
//---------------------------------------------------------------------------
static void showHelp(const char* argv0)
// Display a short help
{
   cout << "usage: " << argv0 << " [cmd] [db] [arg(s)]" << endl
        << "known commands:" << endl
        << "initdb [db] - creates a new database" << endl
        << "loaduni [db] - load sample uni database" << endl
        << "createtable [db] [table] [attributes] - creates a new table" << endl
        << "droptable [db] [table] - deletes a table" << endl
        << "insertvalues [db] [table] [values] - insert values into a table" << endl
        << "dumptable [db] [table] - show the content of a table" << endl
        << "canonical [db] [sql] - run canonical plan" << endl
        << "semana [db] [sql] - print analyzed sql query" << endl
        << "cards [db] [sql] - print cardinality estimation matrix" << endl
        << "goo [db] [sql] - run GOO plan" << endl
        << "dp [db] [sql] - run DP plan" << endl
        << "sql [db] [sql] - run sql query" << endl
        << "printqg [db] [sql] - print the query graph" << endl
        << "quickpick [db] [sql] - print quick pick values" << endl
        << "runstats [db] - update the statistics" << endl
        << "hw [hw] - run a homework task" << endl;
}
//---------------------------------------------------------------------------
static void executeTree(OperatorTree op, ostream& out)
// Execute an operator tree
{
   if (!op.root) {
      out << "EMPTY" << endl;
      return;
   }
   op.root->printTree(out);
   out << endl;
   out << "--result" << endl;
   auto output = op.root->getOutput();
   Printer p(move(op.root), move(output), out);
   p.open();
   while (p.next())
      ;
   p.close();
   out << "--end" << endl;
}
//---------------------------------------------------------------------------
static int adminMain(int argc, char* argv[])
// Entry point
{
   if (argc < 3) {
      showHelp(argv[0]);
      return 1;
   }

   try {
      string cmd = argv[1], dbName = argv[2];
      if (cmd == "initdb")
         initDB(dbName);
      else if (cmd == "loaduni") {
         Database db;
         db.open(dbName);
         db.loadUniDb();
         db.close();
         cout << "ok" << endl;
      } else if (cmd == "runstats")
         runStats(dbName);
      else if (cmd == "hw")
         runHomework(dbName, argc - 2, argv + 2);
      else {
         auto getTable = [&]() {
            if (argc < 4) {
               cerr << "no table specified!" << endl;
               showHelp(argv[0]);
               exit(1);
            }
            return string{argv[3]};
         };
         auto runWithQuery = [&](auto&& callback) {
            Database db;
            db.open(dbName);
            string query;
            for (int i = 3; i < argc; i++) {
               query += (query.empty() ? "" : " ");
               query += argv[i];
            }
            if (query.empty()) {
               do {
                  getline(cin, query);
                  if (!query.empty()) {
                     try {
                        callback(db, query);
                     } catch (const std::exception& err) {
                        cout << "err: " << err.what() << endl;
                        if (auto* sqlErr = dynamic_cast<const IncorrectSqlError*>(&err))
                           cerr << sqlErr->actualWhat() << endl;
                     }
                  }
               } while (cin);
            } else {
               try {
                  callback(db, query);
               } catch (const std::exception& err) {
                  cout << "err: " << err.what() << endl;
               }
            }
         };
         ostream& out = cout;
         if (cmd == "createtable")
            createTable(dbName, getTable(), argc - 4, argv + 4);
         else if (cmd == "droptable")
            dropTable(dbName, getTable());
         else if (cmd == "insertvalues")
            insertValues(dbName, getTable(), argc - 4, argv + 4);
         else if (cmd == "bulkload")
            bulkload(dbName, getTable(), argc - 4, argv + 4);
         else if (cmd == "dumptable")
            dumpTable(dbName, getTable());
         else if (cmd == "sql") {
            runWithQuery([&](Database& db, const string& query) {
               auto jq = JoinQuery::parseAndAnalyseJoinQuery(db, query);
               auto qg = QueryGraph::fromJoinQuery(db, jq);
               auto res = qg.runDP(db);
               auto output = res.tree.root->getOutput();
               Printer p(move(res.tree.root), move(output), cout);
               p.open();
               while (p.next())
                  ;
               p.close();
               cout << "ok" << endl;
            });
         } /* else if (cmd == "printqg") {
            runWithQuery([&](Database& db, const string& query) {
               auto jq = JoinQuery::parseAndAnalyseJoinQuery(db, query);
               auto qg = QueryGraph::fromJoinQuery(db, jq);
               cout << qg.relations.size() << endl;
               for (auto& r : qg.relations)
                  cout << r.cardinality << endl;
               cout << endl;
               cout << qg.joins.size() << endl;
               for (auto& e : qg.joins) {
                  cout << e.left << " " << e.right << " " << e.selectivity << " t" << endl;
               }
            });
         } */
         else if (cmd == "semana") {
            runWithQuery([&](Database& db, const string& query) {
               auto jq = JoinQuery::parseAndAnalyseJoinQuery(db, query);
               out << "ok" << endl;

               {
                  auto vs = jq.relations;
                  sort(vs.begin(), vs.end());
                  out << vs.size() << " ";
                  for (auto& r : vs)
                     out << r.table << " " << r.binding << " ";
                  out << endl;
               }
               {
                  auto vs = jq.projection;
                  sort(vs.begin(), vs.end());
                  out << vs.size() << " ";
                  for (auto& r : vs)
                     out << r.binding << " " << r.attribute << " ";
                  out << endl;
               }
               {
                  auto vs = jq.joinConditions;
                  for (auto& r : vs) {
                     if (r.second < r.first)
                        swap(r.first, r.second);
                  }

                  sort(vs.begin(), vs.end());
                  out << vs.size() << " ";
                  for (auto& r : vs) {
                     out << r.first.binding << " " << r.first.attribute << " " << r.second.binding << " " << r.second.attribute << " ";
                  }
                  out << endl;
               }
               {
                  auto vs = jq.selections;
                  sort(vs.begin(), vs.end());
                  out << vs.size() << " ";
                  for (auto& r : vs)
                     out << r.first.binding << " " << r.first.attribute << " " << r.second << " ";
                  out << endl;
               }
            });
         } else if (cmd == "canonical") {
            runWithQuery([&](Database& db, const string& query) {
               auto jq = JoinQuery::parseAndAnalyseJoinQuery(db, query);
               auto op = jq.buildCanonicalTree(db);
               out << "ok" << endl;
               executeTree(move(op), out);
            });
         } else if (cmd == "cards") {
            runWithQuery([&](Database& db, const string& query) {
               auto jq = JoinQuery::parseAndAnalyseJoinQuery(db, query);
               auto qg = QueryGraph::fromJoinQuery(db, jq);
               out << "ok" << endl;

               auto vs = jq.relations;
               sort(vs.begin(), vs.end());

               for (const auto& v1 : jq.relations)
                  out << fixed << setprecision(2) << qg.getCardinality(v1.binding) << " ";
               out << endl;
               for (const auto& v1 : jq.relations) {
                  for (const auto& v2 : jq.relations)
                     out << fixed << setprecision(2) << qg.getSelectivity(v1.binding, v2.binding) << " ";
                  out << endl;
               }
            });
         } else if (cmd == "goo") {
            runWithQuery([&](Database& db, const string& query) {
               auto jq = JoinQuery::parseAndAnalyseJoinQuery(db, query);
               auto qg = QueryGraph::fromJoinQuery(db, jq);
               auto res = qg.runGOO(db);
               out << "ok" << endl;
               out << "cost: " << fixed << setprecision(2) << res.cost << endl;
               executeTree(move(res.tree), out);
            });
         } else if (cmd == "dp") {
            runWithQuery([&](Database& db, const string& query) {
               auto jq = JoinQuery::parseAndAnalyseJoinQuery(db, query);
               auto qg = QueryGraph::fromJoinQuery(db, jq);
               auto res = qg.runDP(db);
               out << "ok" << endl;
               out << "cost: " << fixed << setprecision(2) << res.cost << endl;
               executeTree(move(res.tree), out);
            });
         } else {
            showHelp(argv[0]);
            return 1;
         }
      }
   } catch (const std::exception& err) {
      cout << "err: " << err.what() << endl;
   }

   return 0;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
// Entry point
{
   return tinydb::adminMain(argc, argv);
}
//---------------------------------------------------------------------------
