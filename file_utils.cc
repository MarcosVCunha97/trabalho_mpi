#include "file_utils.h"

void createDirectory(char *directoryName) {
    if (!fs::exists(directoryName)) {
        fs::create_directory(directoryName);
    }
}

void createFile(char *fileName) {
    if (!fs::exists(fileName)) {
        FILE *file = fopen(fileName, "w");
        fclose(file);
    }
}

void deleteFile(char *fileName) {
    if (fs::exists(fileName)) {
        fs::remove(fileName);
    }
}

char* readFromFile(char *fileName) {
    if (fs::exists(fileName)) {
        FILE *file = fopen(fileName, "r");
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *string = (char *)malloc(fsize + 1);
        fread(string, fsize, 1, file);
        fclose(file);
        string[fsize] = 0;
        return string;
    }
    return NULL;
}

void eraseLastLine(char *fileName) {
    if (fs::exists(fileName)) {
        FILE *file = fopen(fileName, "r+");
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *string = (char *)malloc(fsize + 1);
        fread(string, fsize, 1, file);
        fclose(file);
        string[fsize] = 0;
        char *lastLine = strrchr(string, '\n');
        if (lastLine != NULL) {
            *lastLine = '\0';
        }else{
            string[0] = '\0';
        }
        file = fopen(fileName, "w");
        fprintf(file, "%s", string);
        fclose(file);
    }
}

char* readLastLine(char *fileName) {
    if (fs::exists(fileName)) {
        FILE *file = fopen(fileName, "r+");
        fseek(file, 0, SEEK_END);
        long fsize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *string = (char *)malloc(fsize + 1);
        fread(string, fsize, 1, file);
        fclose(file);
        string[fsize] = 0;
        char *lastLine = strrchr(string, '\n');
        if (lastLine != NULL) {
            removeBreakLine(lastLine);
            return lastLine;
        }else{
            return string;
        }
    }else{
        return NULL;
    }
}

void removeBreakLine(char* text) {
    char* src = text;
    char* dest = text;

    while (*src != '\0') {
        if (*src != '\n') {
            *dest = *src;
            dest++;
        }
        src++;
    }
    *dest = '\0';
}

void writeToFile(char *fileName, char *text) {
    if (fs::exists(fileName)) {
        FILE *file = fopen(fileName, "a");
        if(file != NULL){
            fprintf(file, "\n");
            fprintf(file, "%s", text);
            fclose(file);
        }
    }
}