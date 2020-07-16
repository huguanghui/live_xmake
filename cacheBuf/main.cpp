#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("[%s %d] hello world!\n", __FUNCTION__, __LINE__);
    return 0;
}
