#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

#define MAXNAME 260
#define MAXLINE 2048

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
    DIR *input = opendir("shaders/");
    if (!input) return NULL; // if couldnt open dir return an error

    unsigned int filesCounted = 0;
    char** files = (char**)malloc(count * sizeof(char*));

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
    for(int j = 0; name[j]; j++) {
        output[j] = name[j];
        if (output[j] == '.') {
            output[j] = '\0';
            break;
        }
    }
    output[size-1] = '\0';
}

void createShaderHeader(int fileCount, char **files) {
    FILE *output = fopen("shaders.h", "w");// create output shader.h

    fprintf(output, "namespace Shaders {\n"); // open shader namespace
    for (int i = 0; i < fileCount; i++) { // add each file to the shader
        // pick the name of the header variable
        char name[MAXNAME];
        parseName(files[i], name, MAXNAME);

        // print the start of the line -> const char* variable_name = R"(
        fprintf(output, "const char* ");
        fprintf(output, "%s", name);
        fprintf(output, " = R\"(\n");

        // load file
        char path[MAXLINE+11];
        snprintf(path, sizeof(path), "shaders/%s", files[i]);
        FILE *input = fopen(path, "r");

        // load string
        char string[MAXLINE];
        while (fgets(string, MAXLINE, input)) { // print each line of the file
            fputs(string, output);
        }
        fputs(")\";\n", output);

        fclose(input);
        
    }

    fputc('}', output); // close shader namespace

    // cleanup memory
    for (int i = 0; i < fileCount; i++) {
        free(files[i]);
    }
    free(files);

    fflush(output);
    fclose(output);
}