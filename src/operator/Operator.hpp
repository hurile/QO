#ifndef H_operator_Operator
#define H_operator_Operator
//---------------------------------------------------------------------------
#include <iosfwd>
#include <vector>
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
class Register;
//---------------------------------------------------------------------------
/// Operator interface
class Operator {
   public:
   /// Constructor
   Operator();
   /// Copy constructor
   Operator(const Operator&) = delete;
   /// Destructor
   virtual ~Operator() noexcept;

   /// Open the operator
   virtual void open() = 0;
   /// Produce the next tuple
   virtual bool next() = 0;
   /// Close the operator
   virtual void close() = 0;

   /// Get all produced values
   [[nodiscard]] virtual std::vector<const Register*> getOutput() const = 0;

   /// Print operator tree
   virtual void printTree(std::ostream& out) const = 0;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif
