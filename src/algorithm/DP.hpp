#ifndef H_algorithm_DP
#define H_algorithm_DP
//---------------------------------------------------------------------------
#include <bit>
#include <cstdint>
#include <functional>
#include <span>
#include <unordered_set>
//---------------------------------------------------------------------------
namespace tinydb {
//---------------------------------------------------------------------------
/// Algorithm for DPccp iteration
class DPccp {
   /// Number of nodes
   size_t numNodes;
   /// The neighborhood callback
   std::function<uint64_t(uint64_t)> neighborhood;
   /// The found connected sets
   std::unordered_set<uint64_t> connected;

   public:
   /// Constructor
   DPccp(size_t numNodes, std::function<uint64_t(uint64_t)> neighborhood) : numNodes(numNodes), neighborhood(move(neighborhood)) {}

   /// Enumerate the subsets of a set
   static void enumerateSubsets(uint64_t s, auto&& emit) {
      if (!s)
         return;
      auto s1 = s & (~s + 1);
      do {
         emit(s1);
         s1 = s & (s1 - s);
      } while (s1 != 0);
   }

   /// Enumerate the bits, descending
   static void enumerateDescending(uint64_t S, auto&& emit) {
      for (auto cur = S; cur != 0;) {
         auto v = std::bit_floor(cur);
         cur -= v;
         emit(v);
      }
   }

   /// Enumerate connected subgraphs recursively
   void enumerateCsgRec(uint64_t S, uint64_t X, auto&& emit) {
      if (!connected.contains(S))
         return;

      emit(S);
      auto N = neighborhood(S) & ~X;
      enumerateSubsets(N, [&](uint64_t Sp) {
         enumerateCsgRec(S | Sp, X | N, emit);
      });
   }

   /// Enumerate connected subgraphs
   void enumerateCsg(uint64_t S, uint64_t X, auto&& emit) {
      enumerateDescending(S, [&](uint64_t v) {
         enumerateCsgRec(v, X | ((v - 1) & S), emit);
      });
   }

   /// Enumerate connected subgraphs
   void enumerateCsg(auto&& emit) {
      enumerateCsg((1ull << numNodes) - 1, 0, emit);
   }

   /// Enumerate complement
   void enumerateCmp(uint64_t S, auto&& emit) {
      auto X = ((S & -S) - 1) | S;
      auto N = neighborhood(S) & ~X;
      enumerateCsg(N, X, emit);
   }

   /// Enumerate connected subgraphs and complements
   void enumerateCsgCmp(auto&& emit) {
      connected.clear();
      for (size_t i = 0; i < numNodes; i++)
         connected.insert(1ull << i);

      enumerateCsg([&](uint64_t s1) {
         enumerateCmp(s1, [&](uint64_t s2) {
            if (emit(s1, s2))
               connected.insert(s1 | s2);
         });
      });
   }
};
//---------------------------------------------------------------------------
/// Algorithm for DPsize iteration
class DPSize {
   /// Number of nodes
   size_t numNodes;
   /// Connected subgraphs that have already been found
   std::vector<std::vector<uint64_t>> connected;

   public:
   /// Constructor
   explicit DPSize(uint64_t numNodes) : numNodes(numNodes) {}

   /// Enumerate connected subgraphs and complements
   void enumerateCsgCmp(auto&& emit) {
      connected.resize(numNodes + 1);
      for (auto& c : connected)
         c.clear();
      for (size_t i = 0; i < numNodes; i++)
         connected[1].push_back(1ull << i);

      for (size_t sz = 2; sz <= numNodes; sz++) {
         for (size_t a = 1; a <= sz / 2; a++) {
            size_t b = sz - a;
            for (size_t i = 0; i < connected[a].size(); i++) {
               for (size_t j = (a == b ? i : 0); i < connected[b].size(); i++) {
                  auto s1 = connected[a][i], s2 = connected[b][j];
                  if (emit(s1, s2))
                     connected[sz].push_back(s1 | s2);
               }
            }
         }
      }
   }
};
//---------------------------------------------------------------------------
/// Algorithm for DPsub iteration
class DPSub {
   /// Number of nodes
   size_t numNodes;
   /// The found connected sets
   std::unordered_set<uint64_t> connected;

   public:
   /// Constructor
   explicit DPSub(uint64_t numNodes) : numNodes(numNodes) {}

   /// Enumerate connected subgraphs and complements
   void enumerateCsgCmp(auto&& emit) {
      connected.clear();
      for (size_t i = 0; i < numNodes; i++)
         connected.insert(1ull << i);

      assert(numNodes <= 63);
      for (uint64_t S = 1; S <= (1ull << numNodes) - 1; S++) {
         DPccp::enumerateSubsets(S, [&](uint64_t s1) {
            auto s2 = S & ~s1;
            if (!connected.contains(s1) || !connected.contains(s2))
               return;
            if (emit(s1, s2))
               connected.insert(s1 | s2);
         });
      }
   }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
#endif