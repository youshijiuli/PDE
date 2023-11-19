#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int Nn, Nl1, Nl2, Ne1, Ne2;

int mesh_read(char *infile);
int stiffness_matrix(void);
int Doolittle(char *outfile);

int main(void)
{
    mesh_read("microstrip.msh");                // 处理 gmsh 网格文件
    stiffness_matrix();                         // 刚度矩阵计算
    Doolittle("phi.txt");                       // Doolittle 分解法求矩阵方程

    return 0;
}