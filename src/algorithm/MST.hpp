#ifndef H_algorithm_MST
#define H_algorithm_MST
//---------------------------------------------------------------------------
#include <cstdint>
#include <vector>
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
/// Kruskal's algorithm
class MST {
   /// A node
   struct Node {
      /// The parent index
      unsigned parent;
      /// The size of the subtree
      unsigned size = 1;
   };
   /// The parents
   std::vector<Node> parents;

   /// Grow
   void grow(unsigned size) {
      parents.reserve(size);
      while (parents.size() < size)
         parents.push_back({static_cast<unsigned>(parents.size() - 1)});
   }

   public:
   /// Find
   unsigned find(unsigned v) {
      // Fast path
      if (v >= parents.size() || parents[v].parent == v)
         return v;

      // With path splitting or path halving the following can be reduced to a single pass
      // We implement the classical approach
      unsigned cur = v;
      while (parents[cur].parent != cur)
         cur = parents[cur].parent;

      while (parents[v].parent != cur) {
         v = parents[v].parent;
         parents[v].parent = cur;
      }

      return cur;
   }

   /// Merge
   unsigned merge(unsigned a, unsigned b) {
      a = find(a);
      b = find(b);

      if (std::max(a, b) >= parents.size())
         grow(std::max(a, b));

      if (parents[a].size > parents[b].size)
         std::swap(a, b);

      parents[a].parent = b;
      parents[b].size += parents[a].size;

      return b;
   }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif