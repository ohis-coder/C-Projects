#include <stdio.h>
#define MAX 10

// 1. The Global Arrays (The Ledger)
int key[MAX] = {55, 22, 99, 77, 44};
int next[MAX] = {3, 4, 0, -1, 1};
int head =
    2; // Active Chain: Slot 2 (99) -> Slot 0 (55) -> Slot 3 (77) -> End (-1)

// 2. The Blind Helper Function (The Multi-Layer Melting Tool)
void array_delete_next(int t) { next[t] = next[next[t]]; }

// 3. The Print Function to Verify the Map
void print_list() {
  int current = head;
  while (current != -1) {
    printf("%d ", key[current]);
    current = next[current];
  }
  printf("\n");
}

int main() {
  printf("Original list: ");
  print_list(); // Prints: 99 55 77

  int target_value = 55; // We want to delete 55 (which lives at Slot 0)

  // 4. YOUR BRAIN'S STRATEGY: Initialize the tracker 't' at the head
  int t = head;

  // 5. THE SCANNER: Loop forward until the NEXT slot contains our target value
  // (We also check next[t] != -1 to make sure we don't shoot past the end)
  while (next[t] != -1 && key[next[t]] != target_value) {
    t = next[t]; // Mutate 't' to stride forward along the tracks
  }

  // 6. THE CONFIRMATION: Did we stop because we found it, or did we hit the
  // end?
  if (next[t] != -1) {
    printf("Found target! The node LOOKING at the target is Slot %d\n", t);

    // Pass the previous node 't' into the helper tool!
    array_delete_next(t);
  } else {
    printf("Target value not found in the list.\n");
  }

  printf("List after deleting %d: ", target_value);
  print_list(); // Prints: 99 77

  return 0;
}
