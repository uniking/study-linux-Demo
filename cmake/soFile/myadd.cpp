extern int mysub(int a, int b);
int myadd(int a, int b)
{
	mysub(a, b);
	return a+b;
}
