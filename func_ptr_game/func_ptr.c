#include <stdio.h>

struct Enemy {
  int isBoss;
  char *name;
};

struct Cutscene {
  void (*play_tape)(/*imagine that this plays either the boss_cutscene or the minion_cutscene*/);
};

void boss_cutscene() { printf("boss cutscene playing \n"); }
void minion_cutscene() { printf("3rd person POV loop swapping \n"); }

void boss_particles() { printf("exploding gold \n"); }
void minion_particles() { printf("exploding silver \n"); }

// the kill_logic function accepts the enemy and a function pointer
struct Cutscene kill_logic(struct Enemy *character, void (*effect_func)()) {
  // declare the scene struct so we can use it!
  struct Cutscene scene;

  // 1. run the effect function passed into the arguments
  effect_func();

  // 2. select the right scene based on the boss flag
  scene.play_tape = (character->isBoss ? boss_cutscene : minion_cutscene);

  return scene;
}

int main() {
  // define the array of 1 function pointer slot
  // it accepts a struct Enemy pointer and a function pointer argument
  struct Cutscene (*f[1])(struct Enemy *, void (*)());

  // assign kill logic to slot 0
  f[0] = kill_logic;

  // create a boss enemy
  struct Enemy dragon = {.isBoss = 0, .name = "Boss Guy"};

  // pass the matching particle effect dynamically based on the target!
  // left calls kill_logic, right executes the play_tape callback
  // just imagine that it is the *play_tape ahah
  f[0](&dragon, (dragon.isBoss ? boss_particles : minion_particles))
      .play_tape();

  return 0;
}
