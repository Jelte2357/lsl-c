#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>



int main(int argc, char *argv[])
{
    int folder = 0;
    int sort = 0;
    int amount = 0;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--folder") == 0)
        {
            folder = 1;
            amount++;
        }
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sorted") == 0)
        {
            sort = 1;
            amount++;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            printf("Usage: lsl [-f] [-s] <location>\n");
            return 0;
        }
    }

    if (argc > amount+2) {
        printf("Error: Too many arguments\nUsage: lsl [-f] [-s] <location>\n");
        return 1;
    }

    char *location;

    if (amount < argc - 1)
    {
        location = argv[argc - 1];
    }

    else
    {
        location = ".";
    }

    while (location[0] == '/' || location[0] == '\\')
    {
        location++;
    }

    if (strcmp(location,"")==0)
    {
        location = ".";
    }
    
    if (SetCurrentDirectoryA(location) == 0)
    {
        printf("Error: Failed to open directory\n");
        return 1;
    }

    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    char *searchPath = ".\\*";

    hFind = FindFirstFileA(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Error: Failed to open directory\n");
        return 1;
    }
    if (sort == 0)
    {
        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
                {
                    char curpath [MAX_PATH];
                    GetCurrentDirectoryA(MAX_PATH, curpath);
                    strcat(curpath, "\\");
                    strcat(curpath, findData.cFileName);
                    for (int i = 0; i < strlen(curpath); i++)
                    {
                        if (curpath[i] == '\\')
                        {
                            curpath[i] = '/';
                        }
                    }
                    for (int i = 0; i < strlen(curpath); i++)
                    {
                        if (curpath[i] == ' ')
                        {
                            curpath[i] = '%';
                            for (int j = strlen(curpath); j > i; j--)
                            {
                                curpath[j + 2] = curpath[j];
                            }
                            curpath[i + 1] = '2';
                            curpath[i + 2] = '0';
                        }
                    }
                    printf("\033]8;;file:///%s\033\\\033[34m%s\033[0m\033]8;;\033\\\n", curpath, findData.cFileName);
                }
            }
            else
            {
                char curpath [MAX_PATH];
                GetCurrentDirectoryA(MAX_PATH, curpath);
                strcat(curpath, "\\");
                if (folder == 0) {
                    strcat(curpath, findData.cFileName);
                }
                for (int i = 0; i < strlen(curpath); i++)
                {
                    if (curpath[i] == '\\')
                    {
                        curpath[i] = '/';
                    }
                }
                for (int i = 0; i < strlen(curpath); i++)
                {
                    if (curpath[i] == ' ')
                    {
                        curpath[i] = '%';
                        for (int j = strlen(curpath); j > i; j--)
                        {
                            curpath[j + 2] = curpath[j];
                        }
                        curpath[i + 1] = '2';
                        curpath[i + 2] = '0';
                    }
                }
                printf("\033]8;;file:///%s\033\\%s\033]8;;\033\\\n", curpath, findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData));
    }

    else if (sort == 1)
    {
        do
        {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
                {
                                    char curpath [MAX_PATH];
                GetCurrentDirectoryA(MAX_PATH, curpath);
                strcat(curpath, "\\");
                strcat(curpath, findData.cFileName);
                for (int i = 0; i < strlen(curpath); i++)
                {
                    if (curpath[i] == '\\')
                    {
                        curpath[i] = '/';
                    }
                }
                for (int i = 0; i < strlen(curpath); i++)
                {
                    if (curpath[i] == ' ')
                    {
                        curpath[i] = '%';
                        for (int j = strlen(curpath); j > i; j--)
                        {
                            curpath[j + 2] = curpath[j];
                        }
                        curpath[i + 1] = '2';
                        curpath[i + 2] = '0';
                    }
                }
                printf("\033]8;;file:///%s\033\\\033[34m%s\033[0m\033]8;;\033\\\n", curpath, findData.cFileName);
                }
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
        hFind = FindFirstFileA(searchPath, &findData);
        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                char curpath [MAX_PATH];
                GetCurrentDirectoryA(MAX_PATH, curpath);
                strcat(curpath, "\\");
                if (folder == 0) {
                    strcat(curpath, findData.cFileName);
                }
                for (int i = 0; i < strlen(curpath); i++)
                {
                    if (curpath[i] == '\\')
                    {
                        curpath[i] = '/';
                    }
                }
                for (int i = 0; i < strlen(curpath); i++)
                {
                    if (curpath[i] == ' ')
                    {
                        curpath[i] = '%';
                        for (int j = strlen(curpath); j > i; j--)
                        {
                            curpath[j + 2] = curpath[j];
                        }
                        curpath[i + 1] = '2';
                        curpath[i + 2] = '0';
                    }
                }
                printf("\033]8;;file:///%s\033\\%s\033]8;;\033\\\n", curpath, findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData));
    }

    FindClose(hFind);


    return 0;
}
