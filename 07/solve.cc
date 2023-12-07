#include "../aoc.h"
#include <array>
#include <iostream>
#include <unordered_map>


using ulong = unsigned long;

enum class HandType { HIGH, PAIR, TWOPAIR, THREE, HOUSE, FOUR, FIVE, };
enum class Card {
   BAD, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
};


struct Hand {
   std::array<Card, 5> cards {};
   HandType type;
   ulong bid {};
};

std::istream &
operator >> (std::istream &is, Card &c) {
   char ch;
   is >> ch;
   switch (ch) {
      case '2': c = Card::TWO; break;
      case '3': c = Card::THREE; break;
      case '4': c = Card::FOUR; break;
      case '5': c = Card::FIVE; break;
      case '6': c = Card::SIX; break;
      case '7': c = Card::SEVEN; break;
      case '8': c = Card::EIGHT; break;
      case '9': c = Card::NINE; break;
      case 'T': c = Card::TEN; break;
      case 'J': c = Card::JACK; break;
      case 'Q': c = Card::QUEEN; break;
      case 'K': c = Card::KING; break;
      case 'A': c = Card::ACE; break;
      default: c = Card::BAD;
   }
   return is;
}

std::istream &
operator >> (std::istream &is, Hand &h) {
   std::unordered_map<Card, int> counts;
   std::array<char, 6> ofkind {};
   for (int i = 0; i < 5; ++i) {
      is >> h.cards[i];
      counts[h.cards[i]]++;
   }
   for (auto &[ card, count ] : counts) {
      ofkind[count]++;
   }
   if (ofkind[5])
      h.type = HandType::FIVE;
   else if (ofkind[4])
      h.type = HandType::FOUR;
   else if (ofkind[3])
      h.type = ofkind[2] ? HandType::HOUSE : HandType::THREE;
   else if (ofkind[2] == 2)
      h.type = HandType::TWOPAIR;
   else if (ofkind[2] == 1)
      h.type = HandType::PAIR;
   else
      h.type = HandType::HIGH;
   return is >> h.bid;
}

std::vector<Hand> parse(std::istream &is) {
   std::vector<Hand> hands;
   while (is.peek() != -1) {
      Hand h;
      is >> h;
      if (!is)
         return hands;
      hands.push_back(h);
   }
   return hands;
}

void part1(std::istream &is, std::ostream &os) {
   auto data = parse(is);
   std::sort(data.begin(), data.end(), [] (const Hand &l, const Hand &r) {
         if (l.type != r.type)
            return l.type < r.type;
         for (int i = 0; i < 5; ++i)
            if (l.cards[i] != r.cards[i])
               return l.cards[i] < r.cards[i];
         return false; });

   ulong total = 0;
   for (size_t i = 0; i < data.size(); ++i) {
      total += data[i].bid * (i+1);
   }
   os << total << "\n";

}

void part2(std::istream &is, std::ostream &os) {
}

