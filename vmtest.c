#include "types.h"
#include "stat.h"
#include "user.h"

int global_var = 100;

int
main(void)
{
  int local_var = 200;
  int *heap_var;

  heap_var = (int*)malloc(sizeof(int));
  if(heap_var == 0){
    printf(1, "vmtest: malloc failed\n");
    exit();
  }
  *heap_var = 300;

  printf(1, "Parent: global VA = 0x%x, value = %d\n",
         &global_var, global_var);
  printf(1, "Parent: local  VA = 0x%x, value = %d\n",
         &local_var, local_var);
  printf(1, "Parent: heap   VA = 0x%x, value = %d\n",
         heap_var, *heap_var);

  printf(1, "\n--- Page-table mappings ---\n");
  vmmap((uint)&global_var);
  vmmap((uint)&local_var);
  vmmap((uint)heap_var);

  printf(1, "\n--- Fork test ---\n");
  if(fork() == 0){
    global_var = 111;
    local_var = 222;
    *heap_var = 333;

    printf(1, "Child: global = %d, local = %d, heap = %d\n",
           global_var, local_var, *heap_var);
    exit();
  }

  wait();
  printf(1, "Parent after child: global = %d, local = %d, heap = %d\n",
         global_var, local_var, *heap_var);

  free(heap_var);
  exit();
}
