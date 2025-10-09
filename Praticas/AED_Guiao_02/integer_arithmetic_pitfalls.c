//
// Tomás Oliveira e Silva, AED, October 2021
//
// explain the program output
//
// try also compiling the program with the -Wsign-compare compilation flag
//

#include <stdio.h>

int main(void)
{
  unsigned int i = 1;
  int j = -1;
  int k = -2147483648;

  printf("original i = %u\n",i);
  printf("original j = %d\n",j);
  printf("original k = %d\n",k);
  // compare i with j
  if(i > j)                     //i > j is false!!!!!! 
                                //warning: comparison of integer expressions of different signedness: ‘unsigned int’ and ‘int’ [-Wsign-compare]
    printf("i > j is true\n");
  else
    printf("i > j is false\n");
  // replace k by its absolute value and print the result
  if(k < 0)
    k = -k;
  printf("absolute value of k = %d\n",k);
  return 0;
}

//Output esperado (antes de executar)
//original i = 1
//original j = -1
//original k = -2147483648
//i > j is true 
//absolute value of k = 2147483648