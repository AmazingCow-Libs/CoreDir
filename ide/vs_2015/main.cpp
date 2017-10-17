
#include "./include/CoreDir.h"
#include "CoreFS.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{

    CoreDir::CreateDirectory("../../Ola");
    CoreDir::Move           ("../../Ola", "../../Teste");
    CoreDir::Delete("../../Teste");


    int a = 10;
}
