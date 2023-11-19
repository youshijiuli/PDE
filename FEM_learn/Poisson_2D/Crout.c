#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

extern int Nn;

int Crout(void)
{
    FILE *fp_in1, *fp_in2, *fp_out;
    char *infile1, *infile2, *outfile;
    
    float A[Nn][Nn], b[Nn];

    int i, j, s, k;

    infile1 = "A.txt";
    infile2 = "b.txt";
    outfile = "x.txt";
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
    if ((fp_out = fopen(outfile, "w")) == NULL)
    {
        printf("Can't open %s\n", outfile);
        exit(EXIT_FAILURE);
    }

    // 读取矩阵 A 和向量 b 
    for (i = 0; i < Nn; i++)
    {
        for (j = 0; j < Nn; j++)
        {
            fscanf(fp_in1, "%f", &A[i][j]);
        }
        fscanf(fp_in2, "%f", &b[i]);
    }
    fclose(fp_in1);
    fclose(fp_in2);

    // Crout 分解(A = LU)
    for (j = 1; j < Nn; j++)                                // L(i, 1) = A(i, 1) 不需操作
    {
        A[0][j] = A[0][j]/A[0][0];                          // U(1, j) = A(1, j)/L(1, 1)
    }
    for (k = 1; k < Nn; k++)
    {
        for (i = k; i < Nn; i++)
        {
            for (s = 0; s <= k-1; s++)
            {
                A[i][k] = A[i][k]-A[i][s]*A[s][k];          // L(i, k) = A(i, k)-sum_{s=1}^{k-1}L(i, s)*U(s, k)
            }            
        }
        for (j = k+1; j < Nn; j++)
        {
            for (s = 0; s <= k-1; s++)
            {
                A[k][j] = A[k][j]-A[k][s]*A[s][j];
            }
            A[k][j] = A[k][j]/A[k][k];                      // U(k, j) = (A(k, j)-sum_{s=1}^{k-1}L(k, s)*U(s, j))/L(k, k)
        }        
    }

    // 求解 Ly = b
    b[0] = b[0]/A[0][0];                                    // y(1) = b(1)/L(1, 1)
    for (i = 1; i < Nn; i++)
    {
        for (k = 0; k <= i-1; k++)
        {
            b[i] = b[i]-A[i][k]*b[k];
        }
        b[i] = b[i]/A[i][i];                                // y(i) = (b(i)-sum_{k=1}^{i-1}L(i, k)*y(k))/L(i, i)
    }
    // 求解 Ux = y
    for (i = Nn-2; i >= 0; i--)                             // x(n) = y(n)
    {
        for (k = i+1; k < Nn; k++)
        {
            b[i] = b[i]-A[i][k]*b[k];                       // x(i) = y(i)-sum_{k=i+1}^{n}U(i, k)*x(k)
        }        
    }

    // 输出结果 x
    for (i = 0; i < Nn; i++)
    {
        fprintf(fp_out, "%f\n", b[i]);
    }
    fclose(fp_out);

    return 0;
}