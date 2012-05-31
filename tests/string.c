#include <assert.h>
#include "../include/string.h"

int
main(void)
{
    assert(strlen("") == 0);
    assert(strlen("Fedux") == 5);

    assert(strcmp("Fedux", "Fedux") == 0);
    assert(strcmp("A", "B") < 0);
    assert(strcmp("B", "A") > 0);

    char dst1[6] = {0};
    assert(strcmp("Fedux", strcpy(dst1, "Fedux")) == 0);
    assert(strcmp("A", "B") < 0);

    char cmp1[5] = {'\0', 'A', '\0', 'B', 'C'};
    char cmp2[5] = {'\0', 'A', '\0', 'B', 'X'};
    assert(memcmp((void *)cmp1, (void *)cmp2, 2) == 0);
    assert(memcmp((void *)cmp1, (void *)cmp2, 5) < 0);

    char dst2[5] = {'?', '?', '?', '?', '\0'};
    char cmp[5] = {'F', 'e', 'd', '?', '\0'};
    assert(strcmp("Fed?", strncpy(dst2, "Fedux", 3)) == 0);
    assert(memcmp(cmp, dst2, 5) == 0);

    char mem1[5] = {'?', '?', '?'};
    char mem2[5] = {'F', 'F', 'F'};
    memset(mem1, 'F', 3);
    assert(memcmp(mem1, mem2, 3) == 0);

    return 0;
}
