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

void dist_11_7_4_3() {
  extern RankSet suit11[];
  RankSet *SPADES_START = suit11;
  RankSet *SPADES_END = suit11 + 77;

  extern RankSet ranks7[];
  RankSet *HEARTS_START = ranks7;
  RankSet *HEARTS_END = ranks7 + 867;
  RankSet *SYM_START = ranks7 + 848;

  extern RankSet suit4[];
  RankSet *DIAMONDS_START = suit4;
  RankSet *DIAMONDS_END = suit4 + 714;

  extern RankSet suit3[];
  RankSet *CLUBS_START = suit3;
  RankSet *CLUBS_END = suit3 + 285;


  State state;
  int factor;
  double begin, end;
  RankSet *clubs;

  RankSet *diamonds;
  RankSet *hearts;
  RankSet *spades;

  // check whether to continue an interrupted run
  strncpy(state.name, "dist_11_7_4_3", 20);
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
      factor = hearts < SYM_START ? 48 : 24;
    }
    else if (diamonds < DIAMONDS_END) {
      diamonds++;
      clubs = CLUBS_START;
      factor = hearts < SYM_START ? 48 : 24;
    }
    else if (hearts < HEARTS_END) {
      hearts++;
      diamonds = DIAMONDS_START;
      clubs = CLUBS_START;
      factor = hearts < SYM_START ? 48 : 24;
    }
    else if (spades < SPADES_END) {
      spades++;
      clubs = CLUBS_START;
      diamonds = DIAMONDS_START;
      hearts= HEARTS_START;
      factor = hearts < SYM_START ? 48 : 24;
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
        "11-7-4-3", 
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
