#include <link.h>
#include <string_view>

bool IsLibraryLoaded(const char *libName)
{
    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps)
        return false;

    char line[512];
    bool found = false;

    while (fgets(line, sizeof(line), maps))
    {
        if (strstr(line, libName))
        {
            found = true;
            break;
        }
    }

    fclose(maps);
    return found;
}