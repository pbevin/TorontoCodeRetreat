#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PEOPLE 10

/* #define DEBUG printf */
#define DEBUG(...)

struct person {
  int id;
  char name[50];
  int givable[MAX_PEOPLE];
  int is_giver;
  int is_receiver;
};


struct pair {
  int giver;
  int receiver;
};

int
assign(struct person *people, struct pair *assignments, int npeople, int n) {
  int i, j;

  DEBUG("Enter level %d\n", n);

  if (n == npeople) {
    printf("Found a solution:\n");
    for (i = 0; i < n; i++) {
      int giver = assignments[i].giver;
      int receiver = assignments[i].receiver;
      printf("  %10s => %10s\n", people[giver].name, people[receiver].name);
    }
    printf("\n");
    return 1;
  }

  for (i = 0; i < npeople; i++) {
    if (people[i].is_giver) { DEBUG("Rejecting giver %d\n", i); continue; }

    for (j = 0; j < npeople; j++) {
      int k;
      int succeeded;
      int already_in_hat = 0;
      if (!people[i].givable[j]) { DEBUG("Rejecting self-giving %d %d\n", i, j); continue; }
      if (people[j].is_receiver) { DEBUG("Rejecting receiver %d\n", j); continue; }

      for (k = 0; k < n; k++) {
        if (assignments[k].giver == j && assignments[k].receiver == i) {
          already_in_hat = 1;
        }
      }
      if (already_in_hat) continue;

      assignments[n].giver = i;
      assignments[n].receiver = j;

      people[i].is_giver = 1;
      people[j].is_receiver = 1;

      DEBUG("Recurse!\n");
      succeeded = assign(people, assignments, npeople, n+1);
      if (succeeded)
        return 1;
      DEBUG("Recurse done!\n");

      people[i].is_giver = 0;
      people[j].is_receiver = 0;
    }
  }
  DEBUG("Exit level %d\n", n);

  return 0;

}

int
main(int argc, char *argv[]) {
  int i, j, result;
  int npeople = argc - 1;
  struct person people[MAX_PEOPLE];

  for (i = 0; i < argc - 1; i++) {
    people[i].id = i;
    strcpy(people[i].name, argv[i+1]);
    for (j = 0; j < npeople; j++) {
      people[i].givable[j] = (i != j);
    }
    people[i].is_giver = 0;
    people[i].is_receiver = 0;
  }

  srandom(getpid());
  for (i = 0; i < npeople; i++) {
    int h, w;
    h = random() % npeople;
    w = random() % npeople;
    if (w == h) { i--; continue; }
    printf("SPOUSES: %s and %s\n", people[h].name, people[w].name);
    people[h].givable[w] = 0;
    people[w].givable[h] = 0;
  }
  people[1].givable[2] = 0;
  people[2].givable[1] = 0;

  struct pair assignments[MAX_PEOPLE];

  result = assign(people, assignments, npeople, 0);
  if (!result) {
    printf("No possible results\n");
  }
}
