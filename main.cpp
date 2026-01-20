#include <iostream>
#include <string>
#include <windows.h>
#define MAX 100000
#define MIN 100
std::string s1(MAX,'\0');
std::string s2(MAX,'\0');
std::string buffer(MIN,'\0');
int bufferpos=0;
int s1pos=0;
int s2pos=0;
void passBuffer()
{
    int j=0;
    while(s1pos<MAX && j<MIN){
        s1[s1pos]=buffer[j];
        s1pos++;j++;
    }
    if(j<MIN) std::cout << "s1 exceeded0";
    for(int i=0;i<MIN;i++){
        buffer[i]='\0';
    }
    bufferpos=0;
}
void addatcurrpos(char c){
    if(bufferpos==MIN){
        passBuffer();
    }
    buffer[bufferpos++] = c;
}
void print(){
    system("cls");
    if(s1pos>0)std::cout <<  s1;
    // int i=0;
    // while(i<s1pos){
    //     std::cout << s1[i];
    //     i++;
    // }
    // i=0;
    // while(i<bufferpos){
    //     std::cout << buffer[i];
    //     i++;
    // }
    if(bufferpos>0)std::cout << buffer;
    std::cout << '|';
    // i=0;
    // while(i<s2pos){
    //     std::cout << s2[i];
    //     i++;
    // }
    if(s2pos>0)std::cout << s2 ;
    
}
// void read()
// {
//     char c = std::getchar();
//     while(c != 'q')
//     {
//         addatcurrpos(c);
//         print();
//         c = std::cin.get();
//     }

// }
void left(){
    char k;
    if(bufferpos>0){
        k = buffer[--bufferpos];
        buffer[bufferpos]='\0';
    }
    else if(s1pos == 0)return;
    else {
        k=s1[--s1pos];
        s1[s1pos]='\0';
    }
    std::string s3(MAX,'\0');
    s3[0] = k;
    int i=1,j=0;
    while(i<MAX && j<MAX && j<s2pos)
    {
        s3[i]=s2[j];
        i++;j++;
    }
    s2 = s3;
    s2pos++;
}
void right(){
    char k;
    if(s2pos==0) return;
    if(bufferpos<MIN){
        buffer[bufferpos++]=s2[0];
    }
    else {
        passBuffer();
        buffer[bufferpos++]=s2[0];
    }
    std::string s3(MAX,'\0');
    int i=0,j=1;
    while(i<MAX && j<MAX && j<s2pos)
    {
        s3[i]=s2[j];
        i++;j++;
    }
    s2 = s3;
    s2pos--;
    if(bufferpos==MIN){
        passBuffer();
    }
}
void mode()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hIn,&mode);
    mode &= ~ (ENABLE_LINE_INPUT) ;
    mode &= ~ (ENABLE_ECHO_INPUT) ;
    mode &= ~ (ENABLE_VIRTUAL_TERMINAL_INPUT) ;

    SetConsoleMode(hIn,mode);
    INPUT_RECORD rec;
    DWORD read;
    while (true) {
        ReadConsoleInput(hIn, &rec, 1, &read);
        if (rec.EventType == KEY_EVENT) {
            auto& k = rec.Event.KeyEvent;

            if (k.bKeyDown) {
                if (k.uChar.AsciiChar) {
                    char c = rec.Event.KeyEvent.uChar.AsciiChar;
                    if (c == 'q') break;
                    if (c != 0){
                        addatcurrpos(c);
                        print();
                    }
                } else {
                    switch (k.wVirtualKeyCode) {
                        case VK_LEFT: left();print();break;
                        case VK_RIGHT:right();print();break;
                        case VK_UP:
                        case VK_DOWN:
                            break;
                    }
                }
            }
        }
    }
    DWORD mode2;
    GetConsoleMode(hIn,&mode2);
    mode &=  (ENABLE_LINE_INPUT) ;
    mode &=  (ENABLE_ECHO_INPUT) ;
    mode &=  (ENABLE_VIRTUAL_TERMINAL_INPUT) ;
    SetConsoleMode(hIn,mode2);
}
int main(){
    s1[0]='h';
    s1[1]='o';
    s1[2]='o';
    bufferpos = 0;
    s1pos = 3;
    mode();
}