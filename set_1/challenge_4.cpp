#include "bytes.h"
#include "bitwise_xor.h"
#include "data_analysis/load_character_frequencies.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>


std::ostream& operator<<(std::ostream& out, const std::basic_string<uint8_t>& s) {
  for (auto b : s) {
    if (std::isprint(b)) {
      out << b;
    }
    else if (b == 10) {
      out << '\n';
    }
    else {
      out << "\033[1m" << (unsigned)b << "\033[0m";
    }
  }
  return out;
}

/**
 * Collect the character frequency distribution of a given
 * byte string.
 */
void get_frequency_distribution(
  const std::basic_string<uint8_t>& S,
  std::array<double, 256>& Freq)
{
  Freq.fill(0.0);
  for (auto b : S) {
    ++Freq[b];
  }
  std::transform(
    Freq.begin(), Freq.end(), Freq.begin(),
    [total=S.size()](auto c) {
      return c / total;
    });
}


/**
 * Populate an array of indices @ordering in such a way that
 * i --> Freq[ordering[i]] becomes monotonically decreasing.
 */
void get_frequency_ordering(const std::array<double, 256>& Freq,
                            std::array<size_t, 256>& Ord)
{
  std::iota(Ord.begin(), Ord.end(), 0);
  std::sort(Ord.begin(), Ord.end(), [&Freq](auto a, auto b) {
    return Freq[a] > Freq[b];
  });
}


/**
 * Compute the L2 distance squared between two mappings,
 * i.e.
 *   L2_distance_2(f, g)
 *         = (1 / 256) * Sum_{x} |f(x) - g(x)|^2.
 */
double L2_distance_2(const std::array<double, 256>& F,
                     const std::array<double, 256>& G)
{
  constexpr auto ReductionOp = std::plus<>{};
  constexpr auto TransformOp = [](auto Fx, auto Gx) {
    return (Fx - Gx) * (Fx - Gx);
  };

  return 0.00380625 * std::transform_reduce(
    F.begin(), F.end(), G.begin(), 0.0, ReductionOp, TransformOp);
}


/**
 * Precompose a function (i.e. array<double>) with
 * a permutation of the domain (i.e. array<size_t>)
 */
std::array<double, 256> precompose(
  const std::array<double, 256>& F, const std::array<size_t, 256>& O)
{
  std::array<double, 256> FO;
  std::transform(O.begin(), O.end(), FO.begin(), [&](size_t x) {
    return F[x];
  });
  return FO;
}


/**
 * From a Frequency distribution, construct a functional
 * testing for similitude between an arbitrary byte string's
 * frequency distribution and the original frequency distribution,
 * modulo permuations of the alphabet.
 *
 * More precisely, given the underlying frequency (probability)
 * distribution F and some byte string S with associated
 * frequency distribution F_S, we compute
 *
 *              inf                         || F(x) - S_F(p(x)) ||^2
 * {all bijections : [0..256]-->[0..256]}                         L^2
 *
 */
struct L2_Frequency_functional {

  L2_Frequency_functional(const std::array<double, 256>& Freq)
    : m_FreqOrd{}
  {
    std::array<size_t, 256> Ord;
    get_frequency_ordering(Freq, Ord);
    m_FreqOrd = precompose(Freq, Ord);
  }

  double operator()(const std::basic_string<uint8_t>& S) {
    std::array<double, 256> o_Freq;
    std::array<size_t, 256> o_Ord;

    get_frequency_distribution(S, o_Freq);
    get_frequency_ordering(o_Freq, o_Ord);

    std::array<double, 256> o_FreqOrd = precompose(o_Freq, o_Ord);

    return L2_distance_2(m_FreqOrd, o_FreqOrd);
  }

  std::array<double, 256> m_FreqOrd;
};


template< typename OutputIterator >
void compute_rankings(
  const std::array<double, 256>& ExpectedFreq,
  const std::vector<std::basic_string<uint8_t>>& Ss,
  OutputIterator out)
{
  L2_Frequency_functional L2Fun (ExpectedFreq);
  std::transform(Ss.begin(), Ss.end(), out, [&L2Fun](const auto& S) {
    return L2Fun(S);
  });
}




int main(int argc, char *argv[]) {

  // The challenge data
  std::vector<std::basic_string<uint8_t>> Ss;
  {
    // Buffer for reading input
    std::vector<std::string> SsHex;

    const std::string fn = DATA_DIR "/set_1/challenge_4.txt";
    std::ifstream ifs{ fn };
    if (not ifs) {
      std::cerr << "Failed to open input file " << fn << std::endl;
      return EXIT_FAILURE;
    }

    while (std::getline(ifs, SsHex.emplace_back())) {
      Ss.emplace_back(bytes::hex::decode(SsHex.back()));
    }
  }

  std::cerr << "Successfully loaded the input file" << std::endl;

  // Character frequencies from Harry Potter books
  std::array<double, 256> Freq;
  Freq.fill(0.0);
  bool okay = data::load_character_frequencies(Freq);

  if (not okay) {
    std::cerr << "Failed to load character frequencies" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<double> Scores;
  compute_rankings(Freq, Ss, std::back_inserter(Scores));

  size_t Cand = std::distance(Scores.begin(), std::min_element(Scores.begin(), Scores.end()));

  std::array<double, 256> CandFreq;
  get_frequency_distribution(Ss[Cand], CandFreq);

  uint8_t CandSPC = std::distance(CandFreq.begin(), std::max_element(CandFreq.begin(), CandFreq.end()));
  uint8_t CandKey = CandSPC ^ 0x20;


  std::basic_string<uint8_t> SDecoded =
    bytes::bitwise_xor(&CandKey, 1, &Ss[Cand][0], Ss[Cand].size());


  std::cout << "Found: " << Cand << " -- " << bytes::hex::encode(Ss[Cand])
            << "\nWith key " << (unsigned)CandKey
            << " which decodes to \n  " << SDecoded << std::endl;

  return 0;
}
