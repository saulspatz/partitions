#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "types.h"

extern int backup;
extern int interval;
int solver(RankSet spades, RankSet hearts,RankSet diamonds, RankSet clubs);

void sig_handler(int signum);
int restoreState(State *state, unsigned long stop);
void saveState(State *state);

void dist_7_7_7_4() {
  extern RankSet suit7[];
  RankSet *SPADES_START = suit7;
  RankSet *SPADES_END = suit7 + 1715;

  extern RankSet suit7[];
  RankSet *HEARTS_START = suit7;
  extern RankSet suit7[];
  RankSet *DIAMONDS_START = suit7;
  extern RankSet ranks4[];
  RankSet *CLUBS_START = ranks4;
  RankSet *CLUBS_END = ranks4 + 364;
  RankSet *SYM_START = ranks4 + 350;


  State state;
  int factor;
  double begin, end;
  RankSet *clubs;

  RankSet *diamonds;
  RankSet *hearts;
  RankSet *spades;

  // check whether to continue an interrupted run
  strncpy(state.name, "dist_7_7_7_4", 20);
  int completed = restoreState(&state, SPADES_END-SPADES_START);
  if (completed) return;

  clubs = CLUBS_START + state.cc;

  diamonds = DIAMONDS_START + state.dd;
  hearts = HEARTS_START + state.hh;
  spades = SPADES_START + state.ss;
     
  signal(SIGALRM,sig_handler); // Register signal handler
  alarm(interval);             // schedule a backup in an hour
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
    if (backup) {
      end = clock();
      state.elapsed += (end-begin)/CLOCKS_PER_SEC;
      state.cc = clubs - CLUBS_START;

      state.dd = diamonds - DIAMONDS_START;
      state.hh = hearts - HEARTS_START;
      state.ss = spades - SPADES_START;
      saveState(&state);
      begin = clock();        
      }
  }
  end = clock();
  state.elapsed += (end-begin)/CLOCKS_PER_SEC;
  FILE* out = fopen("counts1.log", "a");
  char buffer[256];
  sprintf(buffer,"%-9s, %lu, %lu, %lu, %lu, %lu, %lu, %lu, %.2f\n",  
        "{title}", 
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
  state.ss = spades - SPADES_START;
  saveState(&state);  // last backup shows calculation complete
}
