#ifndef H_Database
#define H_Database
//---------------------------------------------------------------------------
#include "Table.hpp"
#include <unordered_map>
#include <string>
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
/// Manager for the database instance
class Database {
   private:
   /// The database schema file
   std::string repoFile;
   /// The base directory
   std::string baseDir;
   /// All tables
   std::unordered_map<std::string, Table> tables;
   /// Unwritten changes?
   bool dirty;

   /// Dirty?
   bool isDirty() const;
   /// In-memory?
   bool isInMemory() const;
   /// Read the metadata information
   void read();
   /// Write the metadata information
   void write();

   public:
   /// Constructor
   Database();
   /// Destructor
   ~Database() noexcept;

   Database(const Database&) = delete;
   Database& operator=(const Database&) = delete;

   /// Open an existing database
   void open(std::string name);
   /// Create a new database
   void create(std::string name = {});
   /// Close the database
   void close();

   /// Create a new table
   Table& createTable(std::string_view name);
   /// Drop a table
   void dropTable(std::string_view name);
   /// Has a table?
   bool hasTable(std::string_view name) const { return tables.count(std::string{name}); }
   /// Get a table
   Table& getTable(std::string_view name);

   /// Update the statistics
   void runStats();

   /// Generate a sample university database
   void createUniDb(std::string name = {});
   /// Generate a sample university database
   void loadUniDb();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
