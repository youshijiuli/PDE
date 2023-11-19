#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define len 80

int Nn, Ne, Ns, Nl;

int mesh_read(char *inflie);
int stiffness_matrix(void);
int Crout(void);

int main(void)
{   
    mesh_read("rect.msh");                      // 处理 gmsh 网格文件
    stiffness_matrix();                         // 刚度矩阵计算
    Crout();                                    // Crout 分解法求矩阵方程
    
    return 0;
}