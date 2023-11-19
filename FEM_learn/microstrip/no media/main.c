#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int Nn, Nl1, Nl2, Ne;

int mesh_read(char *infile);
int stiffness_matrix(void);

int main(void)
{
    mesh_read("microstrip.msh");
    stiffness_matrix();

    

    return 0;
}