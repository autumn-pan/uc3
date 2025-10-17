#ifndef MY_NUM
#define MY_NUM 1
#endif

#ifdef MY_FUNC_ENABLED
void my_func() { printf("\nMy num is %i", MY_NUM); }
#endif