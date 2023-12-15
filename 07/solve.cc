#include "../aoc.h"
#include <array>
#include <iostream>
#include <unordered_map>

using ulong = unsigned long;
enum class HandType { HIGH, PAIR, TWOPAIR, THREE, HOUSE, FOUR, FIVE, };
enum class Card { BAD, JOKER, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE };

template <Card jcard> struct Hand {
   HandType type;
   std::array<Card, 5> cards {};
   ulong bid {};
   Hand(std::istream &);
   auto operator <=> (const Hand &r) const = default;
};

template <Card jcard> Card
parseCard(std::istream &is) {
   char ch;
   is >> ch;
   switch (ch) {
      case '2'...'9': return Card(int(Card::TWO) + ch - '2');
      case 'T': return Card::TEN;
      case 'J': return jcard;
      case 'Q': return Card::QUEEN;
      case 'K': return Card::KING;
      case 'A': return Card::ACE;
      default: return Card::BAD;
   }
}

// parse (and classify a hand)
template<Card j> Hand<j>::Hand(std::istream &is) {
   std::unordered_map<Card, int> counts;
   std::array<char, 6> ofkind {};
   int jokers = 0;
   for (int i = 0; i < 5; ++i) {
      Card card = parseCard<j>(is);
      if (card == Card::JOKER) {
         jokers++;
      } else {
         counts[card]++;
      }
      cards[i] = card;
   }
   for (auto &[_, count] : counts)
      ofkind[count]++;
   for (int i = 4; jokers != 0 && i > 0; --i) {
      if (ofkind[i] == 0)
         continue;
      ofkind[i]--;
      ofkind[i+jokers]++;
      jokers = 0;
   }
   ofkind[jokers]++;
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

template <Card j> std::vector<Hand<j>> parse(std::istream &is) {
   std::vector<Hand<j>> hands;
   while (is)
      hands.emplace_back(is);
   hands.pop_back(); // kill the one added at EOF.
   return hands;
};

template <Card j> void solve(std::istream &is, std::ostream &os) {
   auto data = parse<j>(is);
   std::sort(data.begin(), data.end());
   ulong total = 0;
   for (size_t i = 0; i < data.size(); ++i)
      total += data[i].bid * (i+1);
   os << total << "\n";
}

aoc::Case p1("part1", [](std::istream &is, std::ostream &os) { solve<Card::JACK>(is, os); });
aoc::Case p2("part2", [](std::istream &is, std::ostream &os) { solve<Card::JOKER>(is, os); });
