#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern int Nn, Nl1, Nl2, Ne;

int stiffness_matrix(void)
{
    FILE *fp_ins, *fp_in2, *fp_in3, *fp_in4, *fp_out1, *fp_out2;
    char *infile1, *infile2, *infile3, *infile4, *outfile1, *outfile2;

    float epsilon0 = 1., f = 0.;
    float phi1 = 1, phi2 = 0;
    
    float x[Nn], y[Nn];
    int nd1[Nl1], nd2[Nl2], ns[3][Ne];

    float K[Nn][Nn], B[Nn], Ke[3][3], Be[3];
    float be[3], ce[3], Deltae;

    int i, j, m, e;

    infile1 = "coordinate.txt";
    infile2 = "microstrip.txt";
    infile3 = "shield.txt";
    infile4 = "air.txt";
    outfile1 = "A.txt";
    outfile2 = "b.txt";
    if ((fp_ins = fopen(infile1, "r")) == NULL)
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
        fscanf(fp_ins, "%f %f", &x[i], &y[i]);                                  // 读取节点坐标
    }
    for (i = 0; i < Nl1; i++)                                                   // Nl1: 微带边界节点数目
    {
        fscanf(fp_in2, "%d", &nd1[i]);                                          // 读取微带边界节点      
    }
    for (i = 0; i < Nl2; i++)                                                   // Nl2: 屏蔽金属带边界节点数目
    {
        fscanf(fp_in3, "%d", &nd2[i]);                                          // 屏蔽金属带边界节点
    }
    for (e = 0; e < Ne; e++)                                                    // Ne: 空气三角单元数目
    {
        fscanf(fp_in4, "%d %d %d", &ns[0][e], &ns[1][e], &ns[2][e]);            // 读取空气单元节点
    }
    fclose(fp_ins);
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
    for (e = 0; e < Ne; e++)                                                    // 空气处刚度矩阵计算
    {
        i = ns[0][e]-1;                                                         // 单元编码映射至全局编码
        j = ns[1][e]-1;
        m = ns[2][e]-1;
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
                // 单元刚度矩阵元素
                Ke[i][j] = epsilon0*(be[i]*be[j]+ce[i]*ce[j])/(4*Deltae);
                // 单元刚度矩阵组合为总刚度矩阵
                K[ns[i][e]-1][ns[j][e]-1] = K[ns[i][e]-1][ns[j][e]-1]+Ke[i][j];                
            }
            // 单元激励向量元素
            Be[i] = Deltae*f/3.;
            // 单元激励向量组合为总激励向量
            B[ns[i][e]-1] = B[ns[i][e]-1]+Be[i];            
        }
    }
    
    // Dirichlet 边界条件处理
    for (i = 0; i < Nl1; i++)                                                   // 微带边界条件处理
    {
        B[nd1[i]-1] = phi1;                                                     // B(nd(i)) = phi(i)
        K[nd1[i]-1][nd1[i]-1] = 1.;                                             // K(nd(i), nd(i)) = 1

        for (j = 0; j < Nn; j++)
        {
            if (j != nd1[i]-1)
            {
                B[j] = B[j]-K[j][nd1[i]-1]*phi1;                                // B(j) = B(j)-K(j, nd(i))*p(i)
                K[j][nd1[i]-1] = 0.;                                            // K(j, nd(i)) = 0
                K[nd1[i]-1][j] = 0.;                                            // K(nd(i), j) = 0
            }            
        }
    }
    for (i = 0; i < Nl2; i++)                                                   // 屏蔽金属带边界条件处理
    {
        B[nd2[i]-1] = phi2;                                                     // B(nd(i)) = phi(i)
        K[nd2[i]-1][nd2[i]-1] = 1.;                                             // K(nd(i), nd(i)) = 1

        for (j = 0; j < Nn; j++)
        {
            if (j != nd2[i]-1)
            {
                B[j] = B[j]-K[j][nd2[i]-1]*phi2;                                // B(j) = B(j)-K(j, nd(i))*p(i)
                K[j][nd2[i]-1] = 0.;                                            // K(j, nd(i)) = 0
                K[nd2[i]-1][j] = 0.;                                            // K(nd(i), j) = 0
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