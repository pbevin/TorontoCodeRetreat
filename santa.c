#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PEOPLE 20

struct person {
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
assign(struct person *people, struct pair *pair_hat, int npeople, int n) {
  int i, j;

  if (n == npeople) {
    printf("Found a solution:\n");
    for (i = 0; i < n; i++) {
      int giver = pair_hat[i].giver;
      int receiver = pair_hat[i].receiver;
      printf("  %10s => %10s\n", people[giver].name, people[receiver].name);
    }
    printf("\n");
    return 1;
  }

  for (i = 0; i < npeople; i++) {
    if (people[i].is_giver) continue;

    for (j = 0; j < npeople; j++) {
      /* We can reject a recipient for already having a gift,
       * for not being able to receive from the giver, or
       * for already being in the hat with the giver.
       */
      int k;
      int succeeded;
      int already_in_hat = 0;
      if (!people[i].givable[j]) continue;
      if (people[j].is_receiver) continue;

      for (k = 0; k < n; k++) {
        if (pair_hat[k].giver == j && pair_hat[k].receiver == i) {
          already_in_hat = 1;
          break;
        }
      }
      if (already_in_hat) continue;

      pair_hat[n].giver = i;
      pair_hat[n].receiver = j;

      people[i].is_giver = 1;
      people[j].is_receiver = 1;

      succeeded = assign(people, pair_hat, npeople, n+1);
      if (succeeded)
        return 1;

      people[i].is_giver = 0;
      people[j].is_receiver = 0;
    }
  }

  return 0;

}

int
main(int argc, char *argv[]) {
  int i, j, result;
  int npeople = argc - 1;
  struct person people[MAX_PEOPLE];

  if (argc == 1) {
    printf("Usage: %s [name] [name ]...\n", argv[0]);
    exit(1);
  }

  srandom(getpid());

  /* Set up people with names and initial givable[] array */
  for (i = 0; i < argc - 1; i++) {
    strcpy(people[i].name, argv[i+1]);
    for (j = 0; j < npeople; j++) {
      people[i].givable[j] = (i != j); /* Can give to anyone but myself */
    }
    people[i].is_giver = 0;
    people[i].is_receiver = 0;
  }

  /* Marry people off at random. [Warning: polygamy may offend...] */
  for (i = 0; i < npeople; i++) {
    int h, w;
    do {
      h = random() % npeople;
      w = random() % npeople;
    } while (w == h);
    printf("RANDOM MARRIAGE: %s and %s\n", people[h].name, people[w].name);
    people[h].givable[w] = 0;
    people[w].givable[h] = 0;
  }

  /* Find a workable assignment */
  struct pair assignments[MAX_PEOPLE];
  result = assign(people, assignments, npeople, 0);
  if (!result) {
    printf("No possible results\n");
  }

  return 0;
}
