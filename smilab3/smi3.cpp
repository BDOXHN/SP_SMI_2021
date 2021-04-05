#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main()
{
    srand(time(0));
    int i1, j1;
    int mas1[5][5];
    int mas2[4][4];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            mas1[i][j] = rand() % 9 + 1;
            cout << mas1[i][j] << " ";;
        }
        cout << "\n";
    }

    cout << "\n";
    i1 = rand() % 5; //stroka
    j1 = rand() % 5; //stolbec
    cout << "string[0-4]: " << i1 << "\n";
    cout << "collum[0-4]: " << j1 << "\n\n";
    i1 = i1 * 20; // izbavlyaem sebya ot tancev c umnozheniem v assemblere
    j1 = j1 * 4;

    int* arr1 = &mas1[0][0];
    int* arr1end = &mas1[5][5];
    int* arr2 = &mas2[0][0];
    asm(
        "movl %[arr1], %%ebx\n\t" //first element of first matrix
        "movl %[i1], %%ecx\n\t" //znachenie i1 v ecx
        "addl %%ecx, %%ebx\n\t"
        "movl %%ebx, %%ecx\n\t" //smeshenie dlya  i1 zony
        
        "movl %[arr1], %%ebx\n\t" //first element of first matrix
        "movl %[j1], %%eax\n\t"
        "addl %%eax, %%ebx\n\t" // j1 -> eax; i1 -> ecx;
        "movl %%ebx, %%eax\n\t" //smeshenie dlya  j1 znacheniya

        "movl %[arr1], %%ebx\n\t" //first element of first matrix
        "movl %[arr2], %%edx\n\t" //first element of second matrix

        "loop1:\n\t" //proverka i1 zony
        "cmpl %%ecx, %%ebx\n\t"
        "je iloop\n\t"

        "loop2:\n\t" //proverka j2 znacheniya
        "cmpl %%eax, %%ebx\n\t"
        "je jloop\n\t"

        "loop3:\n\t" //cheknut konec massiva
        "push %%eax\n\t"
        "movl %[arr1end], %%eax\n\t"
        "cmpl %%eax, %%ebx\n\t"
        "jg end_asm \n\t" //esli tekushi > granica matrix1, zavershaem
        "push %%eax\n\t" 
        "movl (%%ebx), %%eax\n\t" //matrix1 znach to eax
        "movl %%eax, (%[arr2])\n\t"
        "pop %%eax\n\t"
        "addl $4, %%ebx\n\t" //smeshaem ukazatel matrix1 tekushii
        "cmpl %%eax, %%ebx\n\t" //proveryaem vishli li za granicu konca mat1
        "jg end_asm\n\t" //esli tekushi > granica matrix1, zavershaem
        "addl $4, %%edx\n\t" //esli ne vishli, smeshaem ukazatel 
        "pop %%eax\n\t"
        "jmp loop1\n\t"

        "iloop:\n\t" //i1 zona
        "addl $20, %%ebx\n\t"
        "cmpl %%eax, %%ebx\n\t"
        "jg fixj\n\t"
        "jmp loop2\n\t"

        "fixj:\n\t"
        "addl $20, %%eax\n\t"
        "jmp loop2\n\t"

        "jloop:\n\t" //j2 znachenie
        "addl $4, %%ebx\n\t"
        "addl $20, %%eax\n\t"
        "jmp loop1\n\t"

        "end_asm:\n\t"
        :
        :[i1]"m"(i1), [j1]"m"(j1),
        [arr1]"m"(arr1), [arr1end]"m"(arr1end), [arr2]"d"(arr2)
        :"%ebx", "%ecx", "%eax"  
    );

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << mas2[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}
