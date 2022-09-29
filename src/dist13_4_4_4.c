#include <stdio.h>
#include <locale.h>
#include <time.h>
#include "types.h"

void dist13444() {
  extern RankSet ranks13[];
  RankSet *SPADES_START = ranks13;
  RankSet *SPADES_END = SPADES_START + 0;
  RankSet *SYM_START = SPADES_START + 0;

  extern RankSet suit4[];
  RankSet *HEARTS_START = suit4;
  RankSet *HEARTS_END = HEARTS_START + 714;

  extern RankSet suit4[];
  RankSet *DIAMONDS_START = suit4;
  extern RankSet suit4[];
  RankSet *CLUBS_START = suit4;
  RankSet *spades = SPADES_START;
  RankSet *hearts = HEARTS_START;
  RankSet *diamonds = DIAMONDS_START;
  RankSet *clubs = CLUBS_START-1;
  int factor;
  unsigned long deals = 0L;
  unsigned long classes = 0L;
  double begin, end;
  begin = clock();
  while(1) {
    if (clubs < diamonds) {
      clubs++;
      if (hearts==clubs) factor = 4;
      else if (clubs==diamonds) factor = 12;
      else if (hearts==diamonds) factor = 12;
      else factor = 24;
      if (spades < SYM_START) factor *= 2;
    }
    else if (diamonds < hearts) {
      diamonds++;
      clubs = CLUBS_START;
      factor = (hearts == diamonds) ? 12 : 24;
      if (spades < SYM_START) factor *= 2;
    }
    else if (hearts < HEARTS_END) {
      hearts++;
      clubs = CLUBS_START;
      diamonds = DIAMONDS_START;
      factor = 12;
      if (spades < SYM_START) factor *= 2;
    }
    if (spades < SPADES_END) {
      spades++;
      clubs = CLUBS_START;
      diamonds = DIAMONDS_START;
      hearts = HEARTS_START;
      factor = (spades < SYM_START) ? 8 : 4;
    }
    else break;
    deals += factor;
    classes += 1;
  }
  end = clock();
  double time = (end-begin)/CLOCKS_PER_SEC;
  FILE* out = fopen("counts.log", "a");
  setlocale(LC_ALL, "");
  fprintf(out, "%-9s %'18lu %'18lu %8.2f\n", "13-4-4-4", deals, classes, time);
  fclose(out);
}