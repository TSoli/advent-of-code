#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

const std::unordered_map<char, int> cardOrder{
    {'A', 0}, {'K', 1}, {'Q', 2}, {'J', 3},  {'T', 4},  {'9', 5}, {'8', 6},
    {'7', 7}, {'6', 8}, {'5', 9}, {'4', 10}, {'3', 11}, {'2', 12}};

enum class HandOrder { HIGH, PAIR, TWO_PAIR, THREE, FULL, FOUR, FIVE };

HandOrder getHand(std::string_view hand);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: solution.out <filename>\n";
    return 1;
  }

  std::ifstream inf{argv[1]};

  if (!inf) {
    std::cerr << "Could not open file: " << argv[1] << "\n";
    return 2;
  }

  std::string hand;
  int bid;
  std::vector<std::pair<std::string, int>> hands;
  while (inf >> hand) {
    inf >> bid;
    hands.emplace_back(hand, bid);
  }

  // Sort hands by card order
  std::sort(hands.begin(), hands.end(),
            [](const std::pair<std::string, int> &a,
               const std::pair<std::string, int> &b) {
              int i = 0;
              while (a.first[i] == b.first[i] && i < a.first.size()) {
                ++i;
              }
              return cardOrder.at(a.first[i]) > cardOrder.at(b.first[i]);
            });
  // Sort by hand order
  std::stable_sort(hands.begin(), hands.end(),
                   [](const std::pair<std::string, int> &a,
                      const std::pair<std::string, int> &b) {
                     return static_cast<int>(getHand(a.first)) <
                            static_cast<int>(getHand(b.first));
                   });

  int score{0};
  for (int i = 0; i < hands.size(); ++i) {
    score += (i + 1) * hands[i].second;
    std::cout << hands[i].first << " " << hands[i].second << "\n";
  }

  std::cout << "Total winnings: " << score << "\n";
  return 0;
}

HandOrder getHand(std::string_view hand) {
  std::vector<int> cards(cardOrder.size(), 0);
  for (char ch : hand) {
    cards[cardOrder.at(ch)]++;
  }

  std::sort(cards.begin(), cards.end(), std::greater<int>());

  if (cards[0] == 5) {
    return HandOrder::FIVE;
  }
  if (cards[0] == 4) {
    return HandOrder::FOUR;
  }
  if (cards[0] == 3) {
    if (cards[1] == 2) {
      return HandOrder::FULL;
    }
    return HandOrder::THREE;
  }

  if (cards[0] == 2) {
    if (cards[1] == 2) {
      return HandOrder::TWO_PAIR;
    }

    return HandOrder::PAIR;
  }

  return HandOrder::HIGH;
}
