#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define len 80

extern int Nn, Nl1, Nl2, Ne1, Ne2;

int Dirichlet_count(char *filename, int Nl);                                    // 统计并重新输出 Dirichlet 边界节点

int mesh_read(char *infile)
{
    FILE *fp_in, *fp_out1, *fp_out2, *fp_out3, *fp_out4, *fp_out5;
    char *outfile1, *outfile2, *outfile3, *outfile4, *outfile5;
    char str[len];

    float x, y, z;
    int num_node, num_ele, num_type, num_par, num_phy , num_mod;
    int node_1, node_2, node_3;
    int i, j, num, count;

    infile = "microstrip.msh";
    outfile1 = "coordinate.txt";                                                // 输出节点坐标文件
    outfile2 = "microstrip.txt";                                                // 输出微带边界节点文件
    outfile3 = "shield.txt";                                                    // 输出屏蔽金属带节点文件
    outfile4 = "air.txt";                                                       // 输出空气单元文件
    outfile5 = "media.txt";                                                     // 输出介质单元文件
    if ((fp_in = fopen(infile, "r")) == NULL)                                   // 读取网格文件
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
    if ((fp_out5 = fopen(outfile5, "w")) == NULL)
    {
        printf("Can't open %s\n", outfile5);
        exit(EXIT_FAILURE);
    }

    while (fgets(str, len, fp_in) != NULL)
    {
        if (strcmp(str, "$Nodes\n") == 0)
        {
            Nn = atoi(fgets(str, len, fp_in));
            for (i = 0; i < Nn; i++)
            {
                fscanf(fp_in, "%d %f %f %f", &num_node, &x, &y, &z);            // 读取节点坐标
                fprintf(fp_out1, "%f\t%f\n", x, y);                                                    
            }
            fclose(fp_out1);
        }

        if (strcmp(str, "$Elements\n") == 0)
        {
            num = atoi(fgets(str, len, fp_in));
            Ne1 = 0; Ne1 = 0; Ne2 = 0; Nl1 = 0; Nl2 = 0;

            do
            {
                count = ftell(fp_in);
                fscanf(fp_in, "%d %d %d %d %d %d %d", &num_ele, &num_type, &num_par, &num_phy, &num_mod, &node_1, &node_2);
                if (num_phy == 14)
                {
                    fprintf(fp_out2, "%d\t%d\n", node_1, node_2);               // 读取微带边界节点
                    Nl1++;
                }
                else if (num_phy == 15)
                {
                    fprintf(fp_out3, "%d\t%d\n", node_1, node_2);               // 读取屏蔽金属带边界节点
                    Nl2++;
                }
                else
                {
                    fseek(fp_in, count, SEEK_SET);                              // 读取指针返回至上一行
                }
            } while (num_type == 1);                                            // 两个节点组成的线段
            fclose(fp_out2);
            fclose(fp_out3);

            for (i = 0; i < num-Nl1-Nl2; i++)                                   // 三个节点组成的三角形
            {
                fscanf(fp_in, "%d %d %d %d %d %d %d %d", &num_ele, &num_type, &num_par, &num_phy, &num_mod, &node_1, &node_2, &node_3);
                if (num_phy == 16)
                {
                    fprintf(fp_out4, "%d\t%d\t%d\t\n", node_1, node_2, node_3); // 读取空气单元节点
                    Ne1++;                    
                }
                else
                {
                    fprintf(fp_out5, "%d\t%d\t%d\t\n", node_1, node_2, node_3); // 读取介质单元节点
                    Ne2++;
                }                
            }
            fclose(fp_out4);
            fclose(fp_out5);
        }
    }
    fclose(fp_in);
    
    Nl1 = Dirichlet_count(outfile2, Nl1);
    Nl2 = Dirichlet_count(outfile3, Nl2);


    return 0;
}

int Dirichlet_count(char *filename, int Nl)
{
    int nd[2*Nl];
    FILE *fp;
    int count, num;

    int i, j;

    // 统计 Dirichlet 边界节点数目
    count = 0;
    if ((fp = fopen(filename, "r")) == NULL)
    {
        printf("Can't open %s\n", filename);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < 2*Nl; i++)
    {
        fscanf(fp, "%d", &num);
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
    fclose(fp);
    Nl = count;

    // 重新输出 Dirichlet 边界节点
    if ((fp = fopen(filename, "w")) == NULL)
    {
        printf("Can't open %s\n", filename);
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < Nl; i++)
    {
        fprintf(fp, "%d\n", nd[i]);
    }
    fclose(fp);

    // 返回统计的 Dirichlet 边界节点数目
    return Nl;
}

