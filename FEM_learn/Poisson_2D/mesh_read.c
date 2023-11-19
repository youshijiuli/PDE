#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define len 80
extern int Nn, Ne, Ns, Nl;


int mesh_read(char *infile)
{
    FILE *fp_in, *fp_out1, *fp_out2, *fp_out3, *fp_out4;
    char *outfile1, *outfile2, *outfile3, *outfile4;
    char str[len];

    float x, y, z;
    int num_node, num_ele, num_type, num_par, num_phy , num_mod;
    int node_1, node_2, node_3;
    int i, j, num, count;


    outfile1 = "coordinate.txt";                                            // 输出节点坐标文件
    outfile2 = "Dirichlet_edge.txt";                                        // 输出 Dirichlet 边界文件
    outfile3 = "Neumann_edge.txt";                                          // 输出 Neumann 边界文件
    outfile4 = "elements.txt";                                              // 输出单元文件
    if ((fp_in = fopen(infile, "r")) == NULL)                               // 读取网格文件
    {
        printf("Can't open %s\n", infile);
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
    if ((fp_out3 = fopen(outfile3, "w")) == NULL)
    {
        printf("Can't open %s\n", outfile3);
        exit(EXIT_FAILURE);
    }
    if ((fp_out4 = fopen(outfile4, "w")) == NULL)
    {
        printf("Can't open %s\n", outfile4);
        exit(EXIT_FAILURE);
    }

    while (fgets(str, len, fp_in) != NULL)
    {
        if (strcmp(str, "$Nodes\n") == 0)
        {
            Nn = atoi(fgets(str, len, fp_in));
            for (i = 0; i < Nn; i++)
            {
                fscanf(fp_in, "%d %f %f %f", &num_node, &x, &y, &z);        // 读取节点坐标
                fprintf(fp_out1, "%f\t%f\n", x, y);                                                    
            }
            fclose(fp_out1);
        }

        if (strcmp(str, "$Elements\n") == 0)
        {
            num = atoi(fgets(str, len, fp_in));
            Ne = 0; Ns = 0; Nl = 0;
            do
            {
                count = ftell(fp_in);
                fscanf(fp_in, "%d %d %d %d %d %d %d", &num_ele, &num_type, &num_par, &num_phy, &num_mod, &node_1, &node_2);
                if ((num_phy == 5) || (num_phy == 6))
                {
                    fprintf(fp_out2, "%d\t%d\n", node_1, node_2);           // 读取 Dirichlet 边界节点
                    Nl++;
                }
                else if ((num_phy == 7) || (num_phy == 8))
                {
                    fprintf(fp_out3, "%d\t%d\n", node_1, node_2);           // 读取 Neumann 边界节点
                    Ns++;
                }
                else
                {
                    fseek(fp_in, count, SEEK_SET);                          // 读取指针返回至上一行
                }
            } while (num_type == 1);                                        // 两个节点组成的线段
            fclose(fp_out2);
            fclose(fp_out3);
            Ne = num - Nl - Ns;
            for (i = 0; i < Ne; i++)                                        // 三个节点组成的三角形
            {
                fscanf(fp_in, "%d %d %d %d %d %d %d %d", &num_ele, &num_type, &num_par, &num_phy, &num_mod, &node_1, &node_2, &node_3);
                fprintf(fp_out4, "%d\t%d\t%d\n", node_1, node_2, node_3);   // 读取单元节点
            }
            fclose(fp_out4);
        }
    }
    fclose(fp_in);

    // 统计 Dirichlet 边界节点数目
    count = 0;
    int nd[2*Nl];
    infile = "Dirichlet_edge.txt";
    if ((fp_in = fopen(infile, "r")) == NULL)
    {
        printf("Can't open %s\n", infile);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < 2*Nl; i++)
    {
        fscanf(fp_in, "%d", &num);
        j = 0;
        while ((j <= count) && (num != nd[j]))
        {
            j++;
        }
        if (j == count + 1)
        {
            nd[count] = num;
            count++;
        }
    }
    fclose(fp_in);
    Nl = count;

    // 重新输出 Dirichlet 边界节点
    outfile1 = "Dirichlet_edge.txt";
    if ((fp_out1 = fopen(outfile1, "w")) == NULL)
    {
        printf("Can't open %s\n", outfile1);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < Nl; i++)
    {
        fprintf(fp_out1, "%d\n", nd[i]);
    }
    fclose(fp_out1);

    return 0;
}