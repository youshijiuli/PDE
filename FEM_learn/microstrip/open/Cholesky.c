#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

extern int Nn;


int Cholesky(char *outfile)
{
    FILE *fp_in1, *fp_in2, *fp_out;
    char *infile1, *infile2;
        
    float A[Nn][Nn], b[Nn];
    int i, j, k;

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

    // Cholesky 分解(A = LL')
    A[0][0] = sqrt(A[0][0]);                                                    // L(1, 1) = sqrt(A(1, 1))
    for (i = 1; i < Nn; i++)
    {
        A[i][0] = A[i][0]/A[0][0];                                              // L(i, 1) = A(i, 1)/L(1, 1)
    }
    for (j = 1; j < Nn; j++)
    {
        for (k = 0; k <= j-1; k++)
        {
            A[j][j] = A[j][j]-A[j][k]*A[j][k];
        }
        A[j][j] = sqrt(A[j][j]);                                                // L(j, j) = sqrt(A(j, j)-sum_{k=1}^{j-1}L(j, k)**2)
        for (i = j+1; i < Nn; i++)
        {
            for (k = 0; k <= j-1; k++)
            {
                A[i][j] = A[i][j]-A[i][k]*A[j][k];
            }
            A[i][j] = A[i][j]/A[j][j];                                          // L(i, j) = (A(i, j)-sum_{k=1}^{j-1}L(i, k)*L(j, k))/L(j, j)
        }
    }

    // 求解 Ly = b
    b[0] = b[0]/A[0][0];                                                        // y(1) = b(1)
    for (i = 1; i < Nn; i++)
    {
        for (k = 0; k <= i-1; k++)
        {
            b[i] = b[i]-A[i][k]*b[k];
        }
        b[i] = b[i]/A[i][i];                                                    // y(i) = (b(i)-sum_{k=1}^{i-1}L(i, k)*y(k))/L(i, i)
    }
    // 求解 L'x = y
    b[Nn-1] = b[Nn-1]/A[Nn-1][Nn-1];                                            // x(n) = y(n)/L(n, n)
    for (i = Nn-2; i >= 0 ; i--)
    {
        for (k = i+1; k < Nn; k++)
        {
            b[i] = b[i]-A[k][i]*b[k];
        }
        b[i] = b[i]/A[i][i];                                                    // x(i) = (b(i)-sum_{k=i+1}^{n}L(k, i)*x(k))/L(i, i)
    }

    // 输出结果 x
    for (i = 0; i < Nn; i++)
    {
        fprintf(fp_out, "%f\n", b[i]);
    }
    fclose(fp_out);
    
    return 0;
}