#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>


char* to_uri(const int folder, const char* fileName) { // Convert a file path to a URI (that would still need file:// in front of it to be a valid URI)
    char curpath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, curpath); 
    strcat(curpath, "/");
    if (folder == 0) {
        strcat(curpath, fileName);
    }
    for (int i = 0; i < strlen(curpath); i++) { //replace \ with / and spaces with %20
        if (curpath[i] == '\\') {
            curpath[i] = '/';
        }
        if (curpath[i] == ' ') {
            curpath[i] = '%';
            for (int j = strlen(curpath); j > i; j--) {
                curpath[j + 2] = curpath[j];
            }
            curpath[i + 1] = '2';
            curpath[i + 2] = '0';
        }
    }
    return strdup(curpath);
}

int main(int argc, char *argv[]) {
    int folder = 0;
    int sort = 0;
    int amount = 0;

    for (int i = 0; i < argc; i++) { //parse arguments
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--folder") == 0) {
            folder = 1;
            amount++;
        }
        if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--sorted") == 0) {
            sort = 1;
            amount++;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Usage: lsl [-f] [-s] <location>\n");
            return 0;
        }
    }

    if (argc > amount+2) {
        printf("Error: Too many arguments\nUsage: lsl [-f] [-s] <location>\n");
        return 1;
    }

    char *location;

    if (amount < argc - 1) { //parse location
        location = argv[argc - 1];
    }

    else {
        location = ".";
    }

    while (location[0] == '/' || location[0] == '\\') { //lstrip slashes
        location++;
    }

    if (strcmp(location,"")==0) {
        location = ".";
    }
    
    if (SetCurrentDirectoryA(location) == 0) { //cd to location (easier that to deal with path myself)
        printf("Error: Failed to open directory\n");
        return 1;
    }

    WIN32_FIND_DATAA findData;
    HANDLE hFind;

    char *searchPath = ".\\*";

    hFind = FindFirstFileA(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Error: Failed to open directory\n");
        return 1;
    }
    if (sort == 0) { //if not sorted, print files and directories in alphabetical order
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                    char* results = to_uri(0, findData.cFileName);
                    printf("\033]8;;file:///%s\033\\\033[34m%s\033[0m\033]8;;\033\\\n", results, findData.cFileName); //blue clickable link
                    free(results);
                }
            }
            else {
                char* results = to_uri(folder, findData.cFileName);
                printf("\033]8;;file:///%s\033\\%s\033]8;;\033\\\n", results, findData.cFileName); //white clickable link
                free(results);
            }
        } while (FindNextFileA(hFind, &findData));
    }

    else if (sort == 1) { //if sorted, print directories first, then files
        do {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                    char* results = to_uri(0, findData.cFileName);
                    printf("\033]8;;file:///%s\033\\\033[34m%s\033[0m\033]8;;\033\\\n", results, findData.cFileName); //blue clickable link
                    free(results);
                }
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
        hFind = FindFirstFileA(searchPath, &findData);
        do {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                char* results = to_uri(folder, findData.cFileName);
                printf("\033]8;;file:///%s\033\\%s\033]8;;\033\\\n", results, findData.cFileName); //white clickable link
                free(results);
            }
        } while (FindNextFileA(hFind, &findData));
    }

    FindClose(hFind);
    return 0;
}
