int func(void);
int func2(int x, int y, int z);
int one(void);

int func(void)
{
  return 0;
}

int func2(int x, int y, int z)
{
  return x + y + z;
}

int one(void)
{
  return 1;
}

int main(int arvc, char *argv[])
{
  func2(one(), one(), one()); 
}
