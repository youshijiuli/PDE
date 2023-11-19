#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define m 20                                    // 单元数
#define N 21                                    // 总节点数
#define Pi 3.1415926535                         // pi

float exc(float x);                             // 激励函数 f
float x[N], alpha[m], beta[m], f[m];
float p, q, g;

int main(void){
    int i, j;
    float a[N], c[N-1], b[N], phi[N];
    FILE *fp;

    for (i = 0; i <= N-1; i++)                  // 节点坐标
    {
        x[i] = i*0.05;
    }
    for (i = 0; i <= m-1; i++)                  // 方程参数
    {
        alpha[i] = 1.;                          // alpha 参数
        beta[i] = 0.;                           // beta 参数
        f[i] = exc((x[i]+x[i+1])/2.);           // 激励函数 f 数组
    }
        
    // 计算刚度矩阵和 b 矢量
    a[0] = alpha[0]/(x[1]-x[0]) + beta[0]*(x[1]-x[0])/3.;
    b[0] = f[0]*(x[1]-x[0])/2;
    a[N-1] = alpha[m-1]/(x[N-1]-x[N-2]) + beta[m-1]*(x[N-1]-x[N-2])/3.;
    b[N-1] = f[m-1]*(x[N-1]-x[N-2])/2.;
    for (i = 1; i <= N-2; i++)
    {
        a[i] = alpha[i-1]/(x[i]-x[i-1]) + beta[i-1]*(x[i]-x[i-1])/3.
            + alpha[i]/(x[i+1]-x[i]) + beta[i]*(x[i+1]-x[i])/3.;
        b[i] = f[i-1]*(x[i]-x[i-1])/2. + f[i]*(x[i+1]-x[i])/2.;
    }    
    for (i = 0; i <= N-2; i++)
    {
        c[i] = -alpha[i]/(x[i+1]-x[i])+beta[i]*(x[i+1]-x[i])/6.;
    }    

    // x = 0 (x = 1)处强加 Dirichlet 条件
    p = 0.;
    b[0] = p; a[0] = 1;
    b[1] = b[1] - c[0]*p; c[0] = 0;
    b[N-1] = p; a[N-1] = 1;
    b[N-2] = b[N-2] - c[N-2]*p; c[N-2] = 0;
    // // x = L 处设置 Neumann 条件
    // q = 0; g = 0.;
    // a[N-1] = a[N-1] + g;
    // b[N-1] = b[N-1] + q;

    // Gauss 消元
    for (i = 1; i <= N-1; i++)
    {
        a[i] = a[i] - c[i-1]*c[i-1]/a[i-1];
        b[i] = b[i] - c[i-1]*b[i-1]/a[i-1];
    }
    phi[N-1] = b[N-1]/a[N-1];
    for (i = N-2; i >= 0; i--)
    {
        phi[i] = (b[i] - c[i]*phi[i+1])/a[i];
    }

    // 结果输出
    if ((fp = fopen("out.txt", "w")) == NULL)
    {
        printf("Can't open %s\n", "out.txt");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i <= N-1; i++)
    {
        fprintf(fp, "%10.6f%10.6f\n", x[i], phi[i]);
    }
    fclose(fp);
 
    return 0;
}

float exc(float x)
{
    float f;
    f = ((16*Pi*Pi-4)*sin(4*Pi*x)-16*Pi*cos(4*Pi*x))*exp(2*x);
    return f;
}
