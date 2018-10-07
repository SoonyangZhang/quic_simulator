/*
 * quic_printf.h
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#ifndef QUIC_PRINTF_H_
#define QUIC_PRINTF_H_
#include<string>
#include<stdio.h>
#include<memory>
#include<string>
#include<stdarg.h>
#include<vector>
 std::string StringPrintf(const char* format,...);
template <typename... Args>
inline std::string QuicStringPrintf(const Args&... args) {
  return StringPrintf(std::forward<const Args&>(args)...);
}
#endif /* QUIC_PRINTF_H_ */
