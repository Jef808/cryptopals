#include "metrics.h"

#include <iostream>
#include <string>


int main() {
  uint8_t a[] = "this is a test";
  uint8_t b[] = "wokka wokka!!!";

  auto sa = std::basic_string<uint8_t>(a);
  auto sb = std::basic_string<uint8_t>(b);

  auto d1 = bytes::metrics::hamming_distance(sa, sb);

  std::cout << d1 << std::endl;

  auto d2 = bytes::metrics::hamming_distance(
    &a[0],
    &b[0], sa.size()
  );

  std::cout << d2 << std::endl;
}
