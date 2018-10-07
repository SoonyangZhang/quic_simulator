/*
 * quic_random.h
 *
 *  Created on: 2017?10?25?
 *      Author: dell
 */
#include<cstddef>
#include<stdint.h>
#ifndef QUIC_RANDOM_H_
#define QUIC_RANDOM_H_
class QuicRandom {
 public:
  virtual ~QuicRandom() {}

  // Returns the default random number generator, which is cryptographically
  // secure and thread-safe.
  static QuicRandom* GetInstance();

  // Generates |len| random bytes in the |data| buffer.
  virtual void RandBytes(void* data, size_t len) = 0;

  // Returns a random number in the range [0, kuint64max].
  virtual uint64_t RandUint64() = 0;

  // Reseeds the random number generator with additional entropy input.
  // NOTE: the constructor of a QuicRandom object is responsible for seeding
  // itself with enough entropy input.
  virtual void Reseed(const void* additional_entropy, size_t entropy_len) = 0;
};




#endif /* QUIC_RANDOM_H_ */
