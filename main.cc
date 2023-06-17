#include "chat.h"
#include "file_utils.h"

using namespace std;

int main(){
    // Chat chat = Chat();
    char directoryName[] = "meu_diretorio/teste.txt";
    // chat.createFile(directoryName);
    // char* c = chat.readLastLine(directoryName);
    char* c = readLastLine(directoryName);
    // chat.eraseLastLine(directoryName);
    cout << c << endl;
}
