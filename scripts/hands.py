'''
Generate programs to count deals in Maverick solitaire,
by generating the equivalence classes one-by-one and
mutliplying by the appropriate factors.

There are 10 patterns that the distribution can have.
This script deals with the 7 patterns where two non-void
suits are of the same length.  See rankSym.py for the
case of distinct lengths. 
'''

from math import factorial

def count(n):
    return factorial(13)//(factorial(n)*factorial(13-n))

def genhands(s, h, d=0, c=0):
    with open(f'../src/dist{s}{h}{d}{c}.c', 'w') as fout:
        fout.write('#include <stdio.h>\n')
        fout.write('#include "types.h"\n')
        name = f'dist{s}{h}{d}{c}'
        fout.write(f'\nvoid {name}() {{\n')
        fout.write(f'  extern RankSet suit{s}[];\n')
        fout.write(f'  extern RankSet suit{h}[];\n')
        fout.write(f'  extern Cards{s} spades{s}[];\n')
        fout.write(f'  extern Cards{h} hearts{h}[];\n')
        if h>d>0: 
            fout.write(f'  extern RankSet suit{d}[];\n')
        if d > c > 0:
            fout.write(f'  extern RankSet suit{c}[];\n')
        fout.write(f'  RankSet *SPADES_START = suit{s};\n')
        fout.write(f'  RankSet *HEARTS_START = suit{h};\n')
        if d:
            fout.write(f'  extern Cards{d} diamonds{d}[];\n')
            fout.write(f'  RankSet *DIAMONDS_START = suit{d};\n')
        if c:
            fout.write(f'  extern Cards{c} clubs{c}[];\n')
            fout.write(f'  RankSet *CLUBS_START = suit{c};\n')
        fout.write(f'  RankSet *SPADES_END  = SPADES_START + {count(s)-1};\n')
        if h != s:
            fout.write(f'  RankSet *HEARTS_END  = HEARTS_START + {count(h)-1};\n')
        if d and d!= h:
            fout.write(f'  RankSet *DIAMONDS_END  = DIAMONDS_START + {count(d)-1};\n')
        if c and c!= d:
            fout.write(f'  RankSet *CLUBS_END  = CLUBS_START + {count(c)-1};\n')
        fout.write('  RankSet *spades = SPADES_START;\n')
        if d:
            fout.write('  RankSet *hearts = HEARTS_START;\n')
        else:
            fout.write('  RankSet *hearts = HEARTS_START-1;\n')
        if d and c:
            fout.write('  RankSet *diamonds = DIAMONDS_START;\n')
        elif d:
            fout.write('  RankSet *diamonds = DIAMONDS_START-1;\n')
        if c:
            fout.write('  RankSet *clubs = CLUBS_START-1;\n')
        fout.write('  int factor;\n')
        fout.write('  long total = 0L;\n')
        fout.write('  RankSet Phony = 0;\n')

        # if s > h > d > c > 0:                                   #abcd
        #     fout.write('  while(1) {\n')
        #     fout.write('    if (clubs < CLUBS_END) {\n')
        #     fout.write('      clubs++;\n')
        #     fout.write('      factor = 24;\n')
        #     fout.write('      goto compute;\n')
        #     fout.write('    }\n')

        #     fout.write('    if (diamonds < DIAMONDS_END) {\n')
        #     fout.write('      diamonds++;\n')
        #     fout.write('      clubs = CLUBS_START;\n')
        #     fout.write('      factor = 24;\n')
        #     fout.write('      goto compute;\n')
        #     fout.write('    }\n')

        #     fout.write('    if (hearts < HEARTS_END) {\n')
        #     fout.write('      hearts++;\n')
        #     fout.write('      clubs = CLUBS_START;\n') 
        #     fout.write('      diamonds = DIAMONDS_START;\n')
        #     fout.write('      factor = 24;\n')
        #     fout.write('      goto compute;\n')
        #     fout.write('    };\n') 

        #     fout.write('    if (spades < SPADES_END) {\n')
        #     fout.write('      spades++;\n')
        #     fout.write('      clubs = CLUBS_START;\n')
        #     fout.write('      diamonds = DIAMONDS_START;\n') 
        #     fout.write('      hearts = HEARTS_START;\n')
        #     fout.write('      factor = 24;\n')
        #     fout.write('    } else break;\n') 

        # if s > h > d > c == 0:                         # abc
        #     fout.write('  while(1) {\n')
        #     fout.write('    if (diamonds < DIAMONDS_END) {\n')
        #     fout.write('      diamonds++;\n')
        #     fout.write('      factor = 24;\n')
        #     fout.write('      goto compute;\n')
        #     fout.write('    }\n')

        #     fout.write('    if (hearts < HEARTS_END) {\n')
        #     fout.write('      hearts++;\n')
        #     fout.write('      diamonds = DIAMONDS_START;\n')
        #     fout.write('      factor = 24;\n')
        #     fout.write('      goto compute;\n')
        #     fout.write('    }\n')

        #     fout.write('    if (spades < SPADES_END) {\n')
        #     fout.write('      spades++;\n')
        #     fout.write('      diamonds = DIAMONDS_START;\n')
        #     fout.write('      hearts= HEARTS_START;\n')
        #     fout.write('      factor = 24;\n')
        #     fout.write('    } else break;\n')

        # if s > h and d == c == 0:
        #     fout.write('  while(1) {\n')                                  # ab
        #     fout.write('    if (hearts < HEARTS_END) {\n')
        #     fout.write('      hearts++;\n')
        #     fout.write('      factor = 12;\n')
        #     fout.write('      goto compute;\n')
        #     fout.write('    }\n')

        #     fout.write('    if (spades < SPADES_END) {\n')
        #     fout.write('      spades++;\n')
        #     fout.write('      hearts = HEARTS_START;\n')
        #     fout.write('      factor = 12;\n')
        #     fout.write('    } else break;\n')
    
        if s == h > d > c > 0:                             # aabc
            fout.write('  while(1) {\n')
            fout.write('    if (clubs < CLUBS_END) {\n')
            fout.write('      clubs++;\n')
            fout.write('      factor = (hearts==spades) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (diamonds < DIAMONDS_END) {\n')
            fout.write('      diamonds++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      factor = (hearts==spades) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (hearts < spades) {\n')
            fout.write('      hearts++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      factor = (hearts==spades) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (spades < SPADES_END) {\n')
            fout.write('      spades++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      diamonds = DIAMONDS_START;\n') 
            fout.write('      hearts = HEARTS_START;\n')
            fout.write('      factor = 24;\n')
            fout.write('    } else break;\n')

        if s > h == d > c > 0:                                    # abbc                     
            fout.write('  while(1) {\n')
            fout.write('    if (clubs < CLUBS_END) {\n')
            fout.write('      clubs++;\n')
            fout.write('      factor = (diamonds==hearts) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (diamonds < hearts) {\n')
            fout.write('      diamonds++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      factor = (diamonds==hearts) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (hearts < HEARTS_END) {\n')
            fout.write('      hearts++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      factor = (diamonds==hearts) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (spades < SPADES_END) {\n')
            fout.write('      spades++;\n')
            fout.write('      clubs = CLUBS_START;\n') 
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      hearts = HEARTS_START;\n')
            fout.write('      factor = 12;\n')
            fout.write('    } else break;\n')

        if s > h > d == c > 0:                             # abcc
            fout.write('  while(1) {\n')
            fout.write('    if (clubs < diamonds) {\n')
            fout.write('      clubs++;\n')
            fout.write('      factor = (clubs==diamonds) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (diamonds < DIAMONDS_END) {\n')
            fout.write('      diamonds++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      factor = 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (hearts < HEARTS_END) {\n')
            fout.write('      hearts++;\n')
            fout.write('      clubs = CLUBS_START;\n') 
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      factor = 12;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (spades < SPADES_END) {\n')
            fout.write('      spades++;\n')
            fout.write('      clubs = CLUBS_START;\n') 
            fout.write('      diamonds = DIAMONDS_START;\n') 
            fout.write('      hearts = HEARTS_START;\n')
            fout.write('      factor = 12;\n')
            fout.write('    } else break;\n')

        if s > h == d > c == 0:                             # abb
            fout.write('  while(1) {\n')
            fout.write('    if (diamonds < hearts) {\n')
            fout.write('      diamonds++;\n')
            fout.write('      factor = (diamonds == hearts) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (hearts < HEARTS_END) {\n')
            fout.write('      hearts++;\n')
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      factor = 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (spades < SPADES_END) {\n')
            fout.write('      spades++;\n')
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      factor = 12;\n')
            fout.write('    } else break;\n')

        if s == h > d > c  == 0:                            #aab
            fout.write('  while(1) {\n')
            fout.write('    if (diamonds < DIAMONDS_END) {\n')
            fout.write('      diamonds++;\n')
            fout.write('      factor = (hearts == spades) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (hearts < spades) {\n')
            fout.write('      hearts++;\n')
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      factor = (hearts == spades) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (spades < SPADES_END) {\n')
            fout.write('      spades++;\n')
            fout.write('      diamonds = DIAMONDS_START;\n') 
            fout.write('      hearts = HEARTS_START;\n')
            fout.write('      factor = 24;\n')
            fout.write('    } else break;\n')

        if s == h == d > c > 0:                             # aaab                     
            fout.write('  while(1) {\n')
            fout.write('    if (clubs < CLUBS_END) {\n')
            fout.write('      clubs++;\n')
            fout.write('      if (spades==diamonds) factor = 4;\n')
            fout.write('      else if (spades==hearts) factor = 12;\n')
            fout.write('      else if (hearts==diamonds) factor = 12;\n')
            fout.write('      else factor = 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (diamonds < hearts) {\n')
            fout.write('      diamonds++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      if (spades==diamonds) factor = 4;\n')
            fout.write('      else if (spades==hearts) factor = 12;\n')
            fout.write('      else if (hearts==diamonds) factor = 12;\n')
            fout.write('      else factor = 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (hearts < spades) {\n')
            fout.write('      hearts++;\n')
            fout.write('      diamonds = DIAMONDS_START;\n') 
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      factor = (hearts == spades) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (spades < SPADES_END) {\n')
            fout.write('      spades++;\n')
            fout.write('      hearts = HEARTS_START;\n') 
            fout.write('      diamonds  = DIAMONDS_START;\n') 
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      factor = 12;\n')
            fout.write('    } else break;\n')
        
        if s > h == d == c > 0:                             # abbb
            fout.write('  while(1) {\n')
            fout.write('    if (clubs < diamonds) {\n')
            fout.write('      clubs++;\n')
            fout.write('      if (hearts==clubs) factor = 4;\n')
            fout.write('      else if (clubs==diamonds) factor = 12;\n')
            fout.write('      else if (hearts==diamonds) factor = 12;\n')
            fout.write('      else factor = 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (diamonds < hearts) {\n')
            fout.write('      diamonds++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      factor = (hearts == diamonds) ? 12 : 24;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')

            fout.write('    if (hearts < HEARTS_END) {\n')
            fout.write('      hearts++;\n')
            fout.write('      clubs = CLUBS_START;\n')
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      factor = 12;\n')
            fout.write('      goto compute;\n')
            fout.write('    }\n')   

            fout.write('    if (spades < SPADES_END) {\n')
            fout.write('      spades++;\n')
            fout.write('      clubs = CLUBS_START;\n') 
            fout.write('      diamonds = DIAMONDS_START;\n')
            fout.write('      hearts = HEARTS_START;\n')
            fout.write('      factor = 4;\n')
            fout.write('    } else break;\n')                            

        fout.write('\ncompute:\n\n    total += factor;\n')
        fout.write('    Phony |= *spades & *hearts')
        if d:
            fout.write(' & *diamonds')
        if c:
            fout.write(' & *clubs')
        fout.write(';\n')       
        fout.write('  }\n\n')
        fout.write(f'  printf("{s}-{h}-{d}-{c}: %ld\\n", total);\n')
        fout.write(f'  fprintf(stderr, "%d", Phony);\n')
        fout.write('}')

genhands(13, 9, 2, 1)
genhands(13,10,2)
genhands(13,12)
genhands(11,11,2,1)
genhands(12,6,6,1)
genhands(13,6,3,3)
genhands(13,6,6)
genhands(12,12,1)
genhands(8,8,8,1)
genhands(13,4,4,4)

