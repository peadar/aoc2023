#include "../aoc.h"
#include <array>
#include <iostream>
#include <unordered_map>

using ulong = unsigned long;
enum class HandType { HIGH, PAIR, TWOPAIR, THREE, HOUSE, FOUR, FIVE, };
enum class Card { BAD, WILD, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

template <bool wild> struct Hand {
   HandType type;
   std::array<Card, 5> cards {};
   ulong bid {};
   Hand(std::istream &);
   auto operator <=> (const Hand &r) const = default;
};

std::istream & operator >> (std::istream &is, Card &c) {
   char ch;
   is >> ch;
   switch (ch) {
      case '2'...'9': c = Card(int(Card::TWO) + ch - '2'); break;
      case 'T': c = Card::TEN; break;
      case 'J': c = Card::JACK; break;
      case 'Q': c = Card::QUEEN; break;
      case 'K': c = Card::KING; break;
      case 'A': c = Card::ACE; break;
      default: c = Card::BAD;
   }
   return is;
}

// parse (and classify a hand)
template<bool wild> Hand<wild>::Hand(std::istream &is) {
   std::unordered_map<Card, int> counts;
   std::array<char, 6> ofkind {};
   int jacks = 0;
   for (int i = 0; i < 5; ++i) {
      Card card;
      is >> card;
      if (wild && card == Card::JACK) {
         card = Card::WILD;
         jacks++;
      } else {
         counts[card]++;
      }
      cards[i] = card;
   }
   for (auto &[_, count] : counts)
      ofkind[count]++;
   for (int i = 4; jacks != 0 && i > 0; --i) {
      if (ofkind[i] == 0)
         continue;
      ofkind[i]--;
      ofkind[i+jacks]++;
      jacks = 0;
   }
   ofkind[jacks]++;
   if (ofkind[5])
      type = HandType::FIVE;
   else if (ofkind[4])
      type = HandType::FOUR;
   else if (ofkind[3])
      type = ofkind[2] ? HandType::HOUSE : HandType::THREE;
   else if (ofkind[2] == 2)
      type = HandType::TWOPAIR;
   else if (ofkind[2] == 1)
      type = HandType::PAIR;
   else
      type = HandType::HIGH;
   is >> bid;
}

template <bool wild> std::vector<Hand<wild>> parse(std::istream &is) {
   std::vector<Hand<wild>> hands;
   while (is)
      hands.emplace_back(is);
   hands.pop_back(); // kill the one added at EOF.
   return hands;
};

template <bool wild> void solve(std::istream &is, std::ostream &os) {
   auto data = parse<wild>(is);
   std::sort(data.begin(), data.end());
   ulong total = 0;
   for (size_t i = 0; i < data.size(); ++i)
      total += data[i].bid * (i+1);
   os << total << "\n";
}

void part1(std::istream &is, std::ostream &os) { solve<false>(is, os); }
void part2(std::istream &is, std::ostream &os) { solve<true>(is, os); }
