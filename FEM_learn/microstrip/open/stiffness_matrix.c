#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

extern int Nn, Ns1, Ns2, Nl1, Nl2, Ne1, Ne2;

FILE *file_open(char *filename, char *mode);                                    // 返回文件指针，若打开文件失败，则提示信息

int stiffness_matrix(void)
{
    FILE *fp_in1, *fp_in2, *fp_in3, *fp_in4, *fp_in5, *fp_in6, *fp_in7, *fp_out1, *fp_out2;

    float epsilon0 = 1, epsilon1 = 3, f = 0;
    float phi1 = 0, phi2 = 1;

    float x[Nn], y[Nn];
    int ns1[2][Ns1], ns2[2][Ns2], nd1[Nl1], nd2[Nl2], n1[3][Ne1], n2[3][Ne2];

    float K[Nn][Nn], B[Nn], Ke[3][3], Be[3], Ks[2][2], Bs[2];
    float be[3], ce[3], Deltae, ls, rho, g, q;    

    int i, j, m, s, e;

    fp_in1 = file_open("coordinate.txt", "r");
    fp_in2 = file_open("open1.txt", "r");
    fp_in3 = file_open("open2.txt", "r");
    fp_in4 = file_open("ground.txt", "r");
    fp_in5 = file_open("microstrip.txt", "r");
    fp_in6 = file_open("air.txt", "r");
    fp_in7 = file_open("media.txt", "r");
    fp_out1 = file_open("A.txt", "w");
    fp_out2 = file_open("b.txt", "w");

    // 网格数据读取
    for (i = 0; i < Nn; i++)                                                    // Nn: 节点数目
    {
        fscanf(fp_in1, "%f %f", &x[i], &y[i]);                                  // 读取节点坐标
    }
    for (s = 0; s < Ns1; s++)                                                   // Ns1: 空气开区域边界线段单元数目
    {
        fscanf(fp_in2, "%d %d", &ns1[0][s], &ns1[1][s]);                        // 读取空气开区域边界节点
    }
    for (s = 0; s < Ns2; s++)                                                   // Ns2: 介质开区域边界线段单元数目
    {
        fscanf(fp_in3, "%d %d", &ns2[0][s], &ns2[1][s]);                        // 读取介质开区域边界节点
    }
    for (i = 0; i < Nl1; i++)                                                   // Nl1: 接地边界节点数目
    {
        fscanf(fp_in4, "%d", &nd1[i]);                                          // 读取接地边界节点
    }
    for (i = 0; i < Nl2; i++)                                                   // Nl2: 微带边界节点数目
    {
        fscanf(fp_in5, "%d", &nd2[i]);                                          // 读取微带边界节点
    }
    for (e = 0; e < Ne1; e++)                                                   // Ne1: 空气三角单元数目
    {
        fscanf(fp_in6, "%d %d %d", &n1[0][e], &n1[1][e], &n1[2][e]);            // 读取空气单元节点
    }
    for (e = 0; e < Ne2; e++)                                                   // Ne2: 介质三角单元数目
    {
        fscanf(fp_in7, "%d %d %d", &n2[0][e], &n2[1][e], &n2[2][e]);            // 读取介质单元节点
    }
    fclose(fp_in1);
    fclose(fp_in2);
    fclose(fp_in3);
    fclose(fp_in4);
    fclose(fp_in5);
    fclose(fp_in6);
    fclose(fp_in7);

    // 刚度矩阵计算
    for (i = 0; i < Nn; i++)                                                    // 刚度矩阵初始化
    {
        for (j = 0; j < Nn; j++)
        {
            K[i][j] = 0.;
        }
        B[i] = 0.;        
    }
    for (e = 0; e < Ne1; e++)                                                   // 空气处刚度矩阵计算
    {
        i = n1[0][e]-1;                                                         // 单元编码映射至全局编码
        j = n1[1][e]-1;
        m = n1[2][e]-1;
        be[0] = y[j]-y[m];
        be[1] = y[m]-y[i];
        be[2] = y[i]-y[j];
        ce[0] = x[m]-x[j];
        ce[1] = x[i]-x[m];
        ce[2] = x[j]-x[i];
        Deltae = fabs((be[0]*ce[1]-be[1]*ce[0])/2.);                            // 三角单元面积
        
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                // 单元刚度矩阵元素
                Ke[i][j] = epsilon0*(be[i]*be[j]+ce[i]*ce[j])/(4*Deltae);
                // 单元刚度矩阵组合为总刚度矩阵
                K[n1[i][e]-1][n1[j][e]-1] = K[n1[i][e]-1][n1[j][e]-1]+Ke[i][j];                
            }
            // 单元激励向量元素
            Be[i] = Deltae*f/3.;
            // 单元激励向量组合为总激励向量
            B[n1[i][e]-1] = B[n1[i][e]-1]+Be[i];            
        }
    }
    for (e = 0; e < Ne2; e++)                                                   // 介质板处刚度矩阵计算
    {
        i = n2[0][e]-1;                                                         // 单元编码映射至全局编码
        j = n2[1][e]-1;
        m = n2[2][e]-1;
        be[0] = y[j]-y[m];
        be[1] = y[m]-y[i];
        be[2] = y[i]-y[j];
        ce[0] = x[m]-x[j];
        ce[1] = x[i]-x[m];
        ce[2] = x[j]-x[i];
        Deltae = fabs((be[0]*ce[1]-be[1]*ce[0])/2.);                            // 三角单元面积
        
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                // 单元刚度矩阵元素
                Ke[i][j] = epsilon1*(be[i]*be[j]+ce[i]*ce[j])/(4*Deltae);
                // 单元刚度矩阵组合为总刚度矩阵
                K[n2[i][e]-1][n2[j][e]-1] = K[n2[i][e]-1][n2[j][e]-1]+Ke[i][j];                
            }
            // 单元激励向量元素
            Be[i] = Deltae*f/3.;
            // 单元激励向量组合为总激励向量
            B[n2[i][e]-1] = B[n2[i][e]-1]+Be[i];
        }
    }

    // Neumann 边界条件处理
    for (s = 0; s < Ns1; s++)                                                   // 空气开区域边界条件处理
    {
        i = ns1[0][s]-1;                                                        // 单元编码映射至全局编码
        j = ns1[1][s]-1;
        ls = sqrt(pow(x[i]-x[j], 2)+pow(y[i]-y[j], 2));                         // 线段单元长度
        rho = sqrt(pow((x[i]+x[j])/2., 2)+pow((y[i]+y[j])/2., 2));              // 线段单元与原点的距离

        // 开域边界条件
        g = epsilon0/(rho*log(rho));
        q = 0.;

        // 单元刚度矩阵元素
        Ks[0][0] = g*ls/3.;
        Ks[0][1] = g*ls/6.;
        Ks[1][0] = Ks[0][1];
        Ks[1][1] = Ks[0][0];
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
            {
                // 单元刚度矩阵组合至总刚度矩阵
                K[ns1[i][s]-1][ns1[j][s]-1] = K[ns1[i][s]-1][ns1[j][s]-1]+Ks[i][j];
            }
            // 单元激励向量组合到总激励向量
            B[ns1[i][s]-1] = B[ns1[i][s]-1]+q*ls/2.;
        }
    }
    for (s = 0; s < Ns2; s++)                                                   // 介质开区域边界条件处理
    {
        i = ns2[0][s]-1;                                                        // 单元编码映射至全局编码
        j = ns2[1][s]-1;
        ls = sqrt(pow(x[i]-x[j], 2)+pow(y[i]-y[j], 2));                         // 线段单元长度
        rho = sqrt(pow((x[i]+x[j])/2., 2)+pow((y[i]+y[j])/2., 2));              // 线段单元与原点的距离

        // 开域边界条件
        g = epsilon1/(rho*log(rho));
        q = 0.;

        // 单元刚度矩阵元素
        Ks[0][0] = g*ls/3.;
        Ks[0][1] = g*ls/6.;
        Ks[1][0] = Ks[0][1];
        Ks[1][1] = Ks[0][0];
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 2; j++)
            {
                // 单元刚度矩阵组合至总刚度矩阵
                K[ns2[i][s]-1][ns2[j][s]-1] = K[ns2[i][s]-1][ns2[j][s]-1]+Ks[i][j];
            }
            // 单元激励向量组合到总激励向量
            B[ns2[i][s]-1] = B[ns2[i][s]-1]+q*ls/2.;
        }
    }

    // Dirichlet 边界条件处理
    for (i = 0; i < Nl1; i++)                                                   // 接地边界条件处理
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
    for (i = 0; i < Nl2; i++)                                                   // 微带边界条件处理
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

FILE *file_open(char *filename, char *mode)
{
    FILE *fp;
    if ((fp = fopen(filename, mode)) == NULL)
    {
        printf("Can't open %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    return fp;
}