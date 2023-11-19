#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

extern int Nn, Ne, Ns, Nl;

int stiffness_matrix(void)
{
    FILE *fp_in1, *fp_in2, *fp_in3, *fp_in4, *fp_out1, *fp_out2;
    char *infile1, *infile2, *infile3, *infile4, *outfile1, *outfile2;

    float x[Nn], y[Nn];
    int n[3][Ne], ns[2][Ns], nd[Nl];

    // 方程参数
    float alphax = 1., alphay = 1.;
    float beta = 0., f = 1.;
    float gamma = 1., q = 1., p = 1.;

    float K[Nn][Nn], B[Nn], Ke[3][3], Be[3], Ks[2][2], Bs[2];
    float be[3], ce[3], Deltae, delta_ij, ls;

    int i, j, m, e, s;

    infile1 = "coordinate.txt";
    infile2 = "elements.txt";
    infile3 = "Neumann_edge.txt";
    infile4 = "Dirichlet_edge.txt";
    outfile1 = "A.txt";
    outfile2 = "b.txt";
    if ((fp_in1 = fopen(infile1, "r")) == NULL)
    {
        printf("Can't open %s\n", infile1);
        exit(EXIT_FAILURE);
    }
    if ((fp_in2 = fopen(infile2, "r")) == NULL)
    {
        printf("Can't open %s\n", infile2);
        exit(EXIT_FAILURE);
    }
    if ((fp_in3 = fopen(infile3, "r")) == NULL)
    {
        printf("Can't open %s\n", infile3);
        exit(EXIT_FAILURE);
    }
    if ((fp_in4 = fopen(infile4, "r")) == NULL)
    {
        printf("Can't open %s\n", infile4);
        exit(EXIT_FAILURE);
    }
    if ((fp_out1 = fopen(outfile1, "w")) == NULL)
    {
        printf("Can't open %s\n", outfile1);
        exit(EXIT_FAILURE);
    }
    if ((fp_out2 = fopen(outfile2, "w")) == NULL)
    {
        printf("Can't open %s\n", outfile2);
        exit(EXIT_FAILURE);
    }

    // 网格数据读取
    for (i = 0; i < Nn; i++)                                                    // Nn: 节点数目
    {
        fscanf(fp_in1, "%f %f", &x[i], &y[i]);                                  // 读取节点坐标
    }
    for (e = 0; e < Ne; e++)                                                    // Ne: 三角单元数目
    {
        fscanf(fp_in2, "%d %d %d", &n[0][e], &n[1][e], &n[2][e]);               // 读取单元节点
    }
    for (s = 0; s < Ns; s++)                                                    // Ns: Neumann 边界线段单元数目
    {
        fscanf(fp_in3, "%d %d", &ns[0][s], &ns[1][s]);                          // 读取 Neumann 边界节点
    }
    for (i = 0; i < Nl; i++)                                                    // Nl: Dirichlet 边界节点数目
    {
        fscanf(fp_in4, "%d", &nd[i]);                                           // 读取 Dirichlet 边界节点
    }
    fclose(fp_in1);
    fclose(fp_in2);
    fclose(fp_in3);
    fclose(fp_in4);
        
    // 刚度矩阵计算
    for (i = 0; i < Nn; i++)                                                    // 刚度矩阵初始化
    {
        for (j = 0; j < Nn; j++)
        {
            K[i][j] = 0.;
        }
        B[i] = 0.;        
    }
    for (e = 0; e < Ne; e++)
    {
        i = n[0][e]-1;                                                          // 单元编码映射至全局编码
        j = n[1][e]-1;
        m = n[2][e]-1;
        be[0] = y[j]-y[m];
        be[1] = y[m]-y[i];
        be[2] = y[i]-y[j];
        ce[0] = x[m]-x[j];
        ce[1] = x[i]-x[m];
        ce[2] = x[j]-x[i];
        Deltae = (be[0]*ce[1]-be[1]*ce[0])/2.;                                  // 三角单元面积

        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (i == j)                                                     // delta_ij
                {
                    delta_ij = 1.;
                }
                else
                {
                    delta_ij = 0.;
                }
                // 单元刚度矩阵元素
                Ke[i][j] = (alphax*be[i]*be[j]+alphay*ce[i]*ce[j])/(4*Deltae)+beta*(1.+delta_ij)*Deltae/12.;
                // 单元刚度矩阵组合为总刚度矩阵
                K[n[i][e]-1][n[j][e]-1] = K[n[i][e]-1][n[j][e]-1]+Ke[i][j];
            }
            // 单元激励向量元素
            Be[i] = Deltae*f/3.;
            // 单元激励向量组合为总激励向量
            B[n[i][e]-1] = B[n[i][e]-1]+Be[i];
        }
    }

    // Neumann 边界条件处理
    for (s = 0; s < Ns; s++)
    {
        i = ns[0][s]-1;                                                         // 单元编码映射至全局编码
        j = ns[1][s]-1;
        ls = sqrt(pow(x[i]-x[j], 2)+pow(y[i]-y[j], 2));                         // 线段单元长度
        // 单元刚度矩阵元素
        Ks[0][0] = gamma*ls/3.;
        Ks[0][1] = gamma*ls/6.;
        Ks[1][0] = Ks[0][1];
        Ks[1][1] = Ks[0][0];
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
            {
                // 单元刚度矩阵组合至总刚度矩阵
                K[ns[i][s]-1][ns[j][s]-1] = K[ns[i][s]-1][ns[j][s]-1]+Ks[i][j];
            }
            // 单元激励向量组合到总激励向量
            B[ns[i][s]-1] = B[ns[i][s]-1]+q*ls/2.;
        }
    }

    // Dirichlet 边界条件处理
    for (i = 0; i < Nl; i++)
    {
        B[nd[i]-1] = p;                                                         // B(nd(i)) = p(i)
        K[nd[i]-1][nd[i]-1] = 1.;                                               // K(nd(i), nd(i)) = 1

        for (j = 0; j < Nn; j++)
        {
            if (j != nd[i]-1)
            {
                B[j] = B[j]-K[j][nd[i]-1]*p;                                    // B(j) = B(j)-K(j, nd(i))*p(i)
                K[j][nd[i]-1] = 0.;                                             // K(j, nd(i)) = 0
                K[nd[i]-1][j] = 0.;                                             // K(nd(i), j) = 0
            }            
        }        
    }

    // 矩阵方程输出
    for (i = 0; i < Nn; i++)
    {
        for (j = 0; j < Nn; j++)
        {
            fprintf(fp_out1, "%f\t", K[i][j]);
        }
        fprintf(fp_out1, "\n");
        fprintf(fp_out2, "%f\n", B[i]);
    }
    fclose(fp_out1);
    fclose(fp_out2);

    return 0;
}