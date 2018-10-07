/*
 * quic_random.cc
 *
 *  Created on: 2017?10?25?
 *      Author: dell
 */
#include <limits>
#include<Random>
#include "quic_random.h"
struct MT19937 {
private:
    static std::mt19937_64 rng;
public:
    // This is equivalent to srand().
    static void seed(uint64_t new_seed = std::mt19937_64::default_seed) {
        rng.seed(new_seed);
    }

    // This is equivalent to rand().
    static uint64_t get() {
        return rng();
    }

    static double get_double()
    {
        double result=rng();
        return result/std::numeric_limits<uint64_t>::max();

    }
};

std::mt19937_64 MT19937::rng;
class DefaultRandom : public QuicRandom {
 public:
  static DefaultRandom* GetInstance();

  // QuicRandom implementation
  void RandBytes(void* data, size_t len) override;
  uint64_t RandUint64() override;
  void Reseed(const void* additional_entropy, size_t entropy_len) override;

 private:
  DefaultRandom();
  ~DefaultRandom() override {}
  DISALLOW_COPY_AND_ASSIGN(DefaultRandom);
};
DefaultRandom::DefaultRandom()
{
	MT19937::seed();//  at now,just use the default seeds;
}
DefaultRandom* DefaultRandom::GetInstance() {
  static DefaultRandom *instance_=new DefaultRandom();
  return instance_;
}
//
void DefaultRandom::RandBytes(void* data, size_t len) {
  //crypto::RandBytes(data, len);
}

uint64_t DefaultRandom::RandUint64() {
  uint64_t value;
  //RandBytes(&value, sizeof(value));
  value=MT19937::get();
  return value;
}

void DefaultRandom::Reseed(const void* additional_entropy, size_t entropy_len) {
  // No such function exists in crypto/random.h.
}


// static
QuicRandom* QuicRandom::GetInstance() {
  return DefaultRandom::GetInstance();
}


