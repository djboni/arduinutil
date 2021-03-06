/*
Arduinutil ConvIntStr - Conversions from integer to printable strings


Copyright 2016 Djones A. Boni

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef MISC_CONVINTSTR_H_
#define MISC_CONVINTSTR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

char conv_digit2char(uint8_t digit);
char *conv_ul2str(char *str, uint8_t size, uint32_t val, uint8_t base);
char *conv_fillstr(char *str, uint8_t num, char ch);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MISC_CONVINTSTR_H_ */
