
std::string pad(int size) { return std::string(size, ' '); }

std::ostream &operator <<(std::ostream &os, std::tuple< const Node &, int> tup ) {
   auto &[node, indent] = tup;
   auto dent = pad(indent);
   if (node.value)
      os << dent << "value: " << *node.value << "\n";
   os << dent << "arcs:\n";
   for (auto &[chr, node] : node.arcs)
      os << dent << "  " << "'" << chr << "':\n"
         << std::tuple<const Node &, int>(*node, indent + 4);
   return os << dent << ".\n";
}
std::ostream &operator <<(std::ostream &os, const Node &node) {
   return os << std::tuple<const Node &, int>( node, 0 );
}
std::ostream &operator <<(std::ostream &os, const Trie &trie) {
   return os << trie.root;
}

