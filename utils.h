

char* removeBreakLine(char* text){
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
    return text;
}