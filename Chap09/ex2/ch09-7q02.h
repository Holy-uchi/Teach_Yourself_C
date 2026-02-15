extern int add(int, int);
extern int sub(int, int);
extern int mul(int, int);
extern int divide(int, int);

typedef int (*CALC_FUNC)(int, int);
extern CALC_FUNC select(char);
