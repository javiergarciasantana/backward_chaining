#include "bwd_chaining.h"

int main(int argc, char* argv[]) {

  std::ifstream input;
  BwdChaining machine(input, argv[1]);

  return 0;
}