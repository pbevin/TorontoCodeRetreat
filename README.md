Toronto Code Retreat Jan 16, 2014

Secret Santa

Part 1: Assign people to a "Secret Santa" gift giving:
  - Nobody must be assigned to themself;
  - Everybody must give a gift to someone;
  - Everybody must receive a gift from someone.

Part 2: Support additional constraints:
  - Spouses can't be assigned to each other;
  - If Alice gave to Bob last year, don't assign Bob to her again this year.
  - Don't have Alice giving to Bob and Bob giving to Alice.

This C solution constructs a graph of people and who they can give to.
The solver automatically removes everyone from their own set of possible
givers, and also randomly generates a set of spousal pairings to make
the problem more interesting.  It recursively backtracks through possible
pairings, keeping track of whether each person has been assigned as a giver,
a receiver, or both.

Usage:
 ./santa alice bob charlie dave eddie freddie

