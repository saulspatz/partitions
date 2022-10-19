#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "types.h"

extern int backup;
extern pthread_mutex_t mutex1;
int solver(RankSet spades, RankSet hearts,RankSet diamonds, RankSet clubs);

int restoreState(State *state, unsigned long stop);
void saveState(State *state);

void dist_8_8_8_1() {
  extern RankSet suit8[];
  RankSet *SPADES_START = suit8;
  RankSet *SPADES_END = suit8 + 1286;

  extern RankSet suit8[];
  RankSet *HEARTS_START = suit8;
  extern RankSet suit8[];
  RankSet *DIAMONDS_START = suit8;
  extern RankSet ranks1[];
  RankSet *CLUBS_START = ranks1;
  RankSet *CLUBS_END = ranks1 + 6;
  RankSet *SYM_START = ranks1 + 6;


  State state;
  int factor;
  double begin, end;
  RankSet *clubs;

  RankSet *diamonds;
  RankSet *hearts;
  RankSet *spades;

  // check whether to continue an interrupted run
  strncpy(state.name, "dist_8_8_8_1", 20);
  int completed = restoreState(&state, SPADES_END-SPADES_START);
  if (completed) return;

  clubs = CLUBS_START + state.cc;

  diamonds = DIAMONDS_START + state.dd;
  hearts = HEARTS_START + state.hh;
  spades = SPADES_START + state.ss;
     

  begin = clock();

  while(1) {
    if (clubs < CLUBS_END) {
      clubs++;
      if (spades==diamonds) factor = 4;
      else if (spades==hearts) factor = 12;
      else if (hearts==diamonds) factor = 12;
      else factor = 24;
      if (clubs < SYM_START) factor *= 2;
    }
    else if (diamonds < hearts) {
      diamonds++;
      clubs = CLUBS_START;
      if (spades==diamonds) factor = 4;
      else if (spades==hearts) factor = 12;
      else if (hearts==diamonds) factor = 12;
      else factor = 24;
      if (clubs < SYM_START) factor *= 2;
    }
    else if (hearts < spades) {
      hearts++;
      diamonds = DIAMONDS_START;
      clubs = CLUBS_START;
      factor = (hearts == spades) ? 12 : 24;
      if (clubs < SYM_START) factor *= 2;
    }
    else if (spades < SPADES_END) {
      spades++;
      hearts = HEARTS_START;
      diamonds  = DIAMONDS_START;
      clubs = CLUBS_START;
      factor = (clubs < SYM_START) ? 24 : 12;
    }
    else break;
    int result = solver(*spades, *hearts, *diamonds, *clubs);
    state.exhaustC += 1;
    state.exhaustD += factor;

    if (result == 1) state.solutions += factor;
    pthread_mutex_lock( &mutex1 );
    if (backup) {
      end = clock();
      state.elapsed += (end-begin)/CLOCKS_PER_SEC;
      state.cc = clubs - CLUBS_START;

      state.dd = diamonds - DIAMONDS_START;
      state.hh = hearts - HEARTS_START;
      state.ss = spades - SPADES_START;
      saveState(&state);
      backup = 0;
      begin = clock();        
      }
      pthread_mutex_unlock( &mutex1 );
  }
  end = clock();
  state.elapsed += (end-begin)/CLOCKS_PER_SEC;
  FILE* out = fopen("../results/counts.csv", "a");
  char buffer[256];
  sprintf(buffer,"%-9s, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %.2f\n",  
        "8-8-8-1", 
        state.exhaustC, 
        state.heurC, 
        state.skipC, 
        state.exhaustD, 
        state.heurD, 
        state.skipD, 
        state.solutions, 
        state.elapsed);
  fputs(buffer, out);
  fclose(out);
  state.cc = clubs - CLUBS_START;

  state.dd = diamonds - DIAMONDS_START;
  state.hh = hearts - HEARTS_START;
  state.ss = spades - SPADES_START + 1;  
  saveState(&state);  // last backup shows calculation complete
}
