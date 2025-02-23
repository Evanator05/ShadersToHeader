#include <stdio.h>
#include <stdlib.h>
#include "dirent.h"

#define MAXNAME 260

int getFileCount(void);
char** getFileNames(int count);
void createShaderHeader(int fileCount, char **files);
void parseName(char name[], char output[], int size);

int main() {
    unsigned int fileCount = getFileCount();
    if (fileCount == -1) {
        return 1;
    }

    char **files = getFileNames(fileCount);
    if (!files) {
        return 1;
    }

    createShaderHeader(fileCount, files);

    return 0;
}

int getFileCount() {
    DIR *input;
    input = opendir("shaders/");
    if (!input) return -1; // if couldnt open dir return an error

    unsigned int fileCount = 0;
    struct dirent *entry;
    while((entry = readdir(input)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        fileCount++;
    }
    closedir(input);
    return fileCount;
}

char** getFileNames(int count) {
    DIR *input;
    unsigned int filesCounted = 0;
    char** files = (char**)malloc(count * sizeof(char*));

    input = opendir("shaders/");
    if (!input) return NULL; // if couldnt open dir return an error

    unsigned int fileCount = 0;
    struct dirent *entry;
    while((entry = readdir(input)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        files[filesCounted] = (char*)malloc(strlen(entry->d_name) + 1);
        strcpy(files[filesCounted], entry->d_name);
        filesCounted++;
    }
    closedir(input);

    return files;
}

void parseName(char name[], char output[], int size) {
    // create the variable name
    for(int j = 0; name[j]; j++) {
        output[j] = name[j];
        if (output[j] == '.') {
            output[j] = 0;
            break;
        }
    }
}

void createShaderHeader(int fileCount, char **files) {
    FILE *output = fopen("shaders.h", "w");// create output shader.h

    fprintf(output, "namespace Shaders {\n"); // open shader namespace
    for (int i = 0; i < fileCount; i++) { // add each file to the shader

        // create the variable name
        char name[MAXNAME];
        parseName(files[i], name, MAXNAME);

        fprintf(output, "const char* ");
        fprintf(output, name);
        fprintf(output, " = R\"(\n");

        // load file
        char path[255];
        sprintf(path, "shaders/%s", files[i]);
        FILE *input = fopen(path, "r");

        // load string
        size_t size = 5000;
        char string[size];
        while (fgets(string, size, input)) {
            fprintf(output, string);
        }
        
        fclose(input);

        fprintf(output, ")\";\n");
    }

    fprintf(output, "}"); // close shader namespace

    // cleanup memory
    for (int i = 0; i < fileCount; i++) {
        free(files[i]);
    }
    free(files);

    fflush(output);
    fclose(output);
    free(output);
}