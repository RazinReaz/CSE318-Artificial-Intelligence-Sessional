static long int seed = 1;

long int PMrand()
{
    long int hi = seed / (2147483647 / 16807);
    long int lo = seed % (2147483647 / 16807);
    long int test = 16807 * lo - (2147483647 % 16807) * hi;
    if (test > 0)
        seed = test;
    else
        seed = test + 2147483647;
    return seed;
}
