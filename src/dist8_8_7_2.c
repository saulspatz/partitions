#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "types.h"

void dist8872() {
  extern RankSet suit8[];
  RankSet *SPADES_START = suit8;
  RankSet *SPADES_END = SPADES_START + 1286;

  extern RankSet suit8[];
  RankSet *HEARTS_START = suit8;
  extern RankSet ranks7[];
  RankSet *DIAMONDS_START = ranks7;
  RankSet *DIAMONDS_END = DIAMONDS_START + 867;
  RankSet *SYM_START = DIAMONDS_START + 848;

  extern RankSet suit2[];
  RankSet *CLUBS_START = suit2;
  RankSet *CLUBS_END = CLUBS_START + 77;

  RankSet *spades = SPADES_START;
  RankSet *hearts = HEARTS_START;
  RankSet *diamonds = DIAMONDS_START;
  RankSet *clubs = CLUBS_START-1;
  int factor;
  unsigned long deals = 0L;
  unsigned long classes = 0L;
  double begin, end;
  begin = clock();
  while (1) {
    if (clubs < CLUBS_END) {
      clubs++;
      factor = (hearts==spades) ? 12 : 24;
      if (diamonds < SYM_START) factor *= 2;
    }
    else if (diamonds < DIAMONDS_END) {
      diamonds++;
      clubs = CLUBS_START;
      factor = (hearts==spades) ? 12 : 24;
      if (diamonds < SYM_START) factor *= 2;
    }
    else if (hearts < spades) {
      hearts++;
      clubs = CLUBS_START;
      diamonds = DIAMONDS_START;
      factor = (hearts==spades) ? 12 : 24;
      if (diamonds < SYM_START) factor *= 2;
    }
    else if (spades < SPADES_END) {
      spades++;
      clubs = CLUBS_START;
      diamonds = DIAMONDS_START;
      hearts = HEARTS_START;
      factor = (diamonds < SYM_START) ? 48 : 24;
    }
    else break;
    deals += factor;
    classes += 1;
  }
  end = clock();
  double time = (end-begin)/CLOCKS_PER_SEC;
  FILE* out = fopen("counts.log", "a");
  setlocale(LC_ALL, "");
  fprintf(out, "%9s %'15lu %'15lu %.2f\n", "8-8-7-2", deals, classes, time);
  fclose(out);
}