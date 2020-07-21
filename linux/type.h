#pragma once

#include <linux/input.h>
#include <stdbool.h>
#include <stdio.h>
// clang-format off
#define printevbegin(X) \
    if (type == (X)) { \
        printf(#X" %hu ", type); \
    }
#define printev(X) \
    else if (type == (X)) { \
        printf(#X" %hu ", type); \
    }
#define printevend(X) \
    else { \
        printf(#X" %hu ", type); \
    }
// clang-format on

void printEventType(__u16 type, __u16 code, __s32 value, bool newline) {
  // clang-format off
  printevbegin(EV_SYN)
  printev(EV_KEY)
  printev(EV_REL)
  printev(EV_ABS)
  printev(EV_MSC)
  printev(EV_SW)
  printev(EV_LED)
  printev(EV_SND)
  printev(EV_REP)
  printev(EV_FF)
  printev(EV_PWR)
  printev(EV_FF_STATUS)
  printev(EV_MAX)
  printev(EV_CNT)
  printevend(UNKNOWN EVENT);
  printf("%hu %d", code, value);
  // clang-format on
  if (newline) {
    printf("\n");
  }
}
