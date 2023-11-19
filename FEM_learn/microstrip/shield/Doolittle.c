#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern int Nn;

int Doolittle(char *outfile)
{
    FILE *fp_in1, *fp_in2, *fp_out;
    char *infile1, *infile2;

    float A[Nn][Nn], b[Nn];

    int i, j, r, k;

    infile1 = "A.txt";
    infile2 = "b.txt";
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

    // Doolittle 分解(A = LU)
    for (i = 1; i < Nn; i++)                                // U(1, j) = A(1, j) 不需操作
    {
        A[i][0] = A[i][0]/A[0][0];                          // L(i, 1) = A(i, 1)/U(1, 1)
    }
    for (r = 1; r < Nn; r++)
    {
        for (j = r; j < Nn; j++)
        {
            for (k = 0; k <= r-1; k++)
            {
                A[r][j] = A[r][j]-A[r][k]*A[k][j];          // U(r, j) = A(r, j)-sum_{k=1}^{r-1}L(r, k)*U(k, j)
            }
        }
        for (i = r+1; i < Nn; i++)
        {
            for (k = 0; k <= r-1; k++)
            {
                A[i][r] = A[i][r]-A[i][k]*A[k][r];
            }
            A[i][r] = A[i][r]/A[r][r];                      // L(i, r) = (A(i, r)-sum_{k=1}^{r-1}L(i, k)*U(k, r))/U(r, r)
        }
    }

    // 求解 Ly = b
    for (i = 1; i < Nn; i++)                                // y(1) = b(1)
    {
        for (k = 0; k <= i-1; k++)
        {
            b[i] = b[i]-A[i][k]*b[k];                       // y(i) = b(i)-sum_{k=1}^{i-1}L(i, k)*y(k)
        }
    }
    // 求解 Ux = y
    b[Nn-1] = b[Nn-1]/A[Nn-1][Nn-1];                        // x(n) = y(n)/U(n, n)
    for (i = Nn-2; i >= 0; i--)
    {
        for (k = i+1; k < Nn; k++)
        {
            b[i] = b[i]-A[i][k]*b[k];
        }
        b[i] = b[i]/A[i][i];                                // x(i) = (y(i)-sum_{k=i+1}^{n}U(i, k)*x(k))/U(i, i)
    }

    // 输出结果 x
    for (i = 0; i < Nn; i++)
    {
        fprintf(fp_out, "%f\n", b[i]);
    }
    fclose(fp_out);    

    return 0;
}
