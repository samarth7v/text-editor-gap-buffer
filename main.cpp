#include <iostream>
#include <windows.h>
#include <string>
struct gapBuffer{
    char* data;
    size_t start;
    size_t end;
    size_t capacity;
}a;
void expand(){
    struct gapBuffer b;
    b.capacity = 2*a.capacity;
    b.data = new char[b.capacity];
    b.start = a.start;
    size_t k = (b.capacity - a.capacity);
    b.end = a.end + k;
    for(int i =0 ; i< a.start;i++){
        b.data[i]=a.data[i];
    }
    for(int i =b.end ; i< b.capacity;i++){
        b.data[i]=a.data[i-k];
    }
    delete[] a.data;
    a.data = b.data;
    a.start = b.start;
    a.end = b.end;
    a.capacity = b.capacity;

}
void initialize(){
    a.start=0;
    a.end=2;
    a.capacity = 2;
    a.data=new char[a.capacity];
}
void write(char c){
    if(a.start == a.end){
        expand();
    }
    a.data[a.start++]=c;
}
// void clear_screen() {
//     HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

//     CONSOLE_SCREEN_BUFFER_INFO csbi;
//     GetConsoleScreenBufferInfo(hOut, &csbi);

//     DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
//     DWORD written;

//     COORD home = {0, 0};

//     FillConsoleOutputCharacter(hOut, ' ', cellCount, home, &written);
//     FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, home, &written);
//     SetConsoleCursorPosition(hOut, home);
// }

COORD compute_cursor() {
    COORD c{0, 0};
    for (size_t i = 0; i < a.start; ++i) {
        if (a.data[i] == '\n') {
            ++c.Y;
            c.X = 0;
        } else {
            ++c.X;
        }
    }
    return c;
}
void print() {
    std::cout << "\x1b[H";
    std::cout << "\x1b[J";

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD home = {0,0};
    SetConsoleCursorPosition(hOut, home);
    SHORT x = 0, y = 0;
    std::string buffer;
    for (size_t i = 0; i < a.start; ++i) {
        if (a.data[i] == '\n') {
            buffer += "\x1b[K\n";
            ++y;
            x = 0;
        } else {
            buffer += a.data[i];
            ++x;
        }
    }

    for (size_t i = a.end; i < a.capacity; ++i) {
        if (a.data[i] == '\n') {
            buffer += "\x1b[K\n";
        } else {
            buffer += a.data[i];
        }
    }
    buffer+=" ";
    std::cout << buffer;
    COORD cur;
    cur.X = x;
    cur.Y = y;
    SetConsoleCursorPosition(hOut, cur);
}

void left(){
    if(a.start==0) return;
    a.data[--a.end] = a.data[--a.start];
}
void right(){
    if(a.end==a.capacity)return;
    a.data[a.start++] = a.data[a.end++];
}
void escape(){
    exit(0);
}
void back(){
    if(a.start==0)return;
    a.start--;
    print();
}
void del(){
    if(a.end==a.capacity)return;
    a.end++;
    print();
}
void enter(){
    write('\n');
    print();
}
void mode()
{
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hIn,&mode);
    mode &= ~ (ENABLE_LINE_INPUT) ;
    mode &= ~ (ENABLE_ECHO_INPUT) ;
    // mode &= ~ (ENABLE_VIRTUAL_TERMINAL_INPUT) ;

    SetConsoleMode(hIn,mode);
    INPUT_RECORD rec;
    DWORD read;
    HANDLE hOutt = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modet;
    GetConsoleMode(hOutt, &modet);
    modet |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    modet |= ENABLE_PROCESSED_OUTPUT;
    SetConsoleMode(hOutt, modet);
    while (true) {
        ReadConsoleInput(hIn, &rec, 1, &read);
        if (rec.EventType == KEY_EVENT) {
            auto& k = rec.Event.KeyEvent;

            if (k.bKeyDown) {
                bool isSpecial = true;
            
                switch (k.wVirtualKeyCode) {
                    case VK_LEFT:   left(); print(); break;
                    case VK_RIGHT:  right(); print(); break;
                    case VK_ESCAPE: escape(); break;
                    case VK_RETURN: enter(); break; 
                    case VK_DELETE: del(); break; 
                    case VK_BACK:   back(); break;  
                    case VK_UP:     break;
                    case VK_DOWN:   break;
                    default:        isSpecial = false; break;
                }

                if (!isSpecial && k.uChar.AsciiChar >= 32) {
                    if (k.uChar.AsciiChar == 'q') break; // Quit option
                    
                    write(k.uChar.AsciiChar);
                    print();
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
    initialize();
    mode();
}
