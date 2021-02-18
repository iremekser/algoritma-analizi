#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n;
    int i, j;
    printf("Mac sayisini giriniz(n) : ");
    scanf("%d", &n);     // kullanicidan toplam oynanacak mac sayisini aliriz
    n = (n / 2) + 1 + 1; // bir takim girilen mac sayisinin yarisindan bir fazlasi kadar mac kazanirsa kazanir demektir
                         // bu hesap icin ve matriste baslangic degerleri tutmak icin bir fazla satir-sutun gerekir
                         // o yuzden (n / 2) + 1 + 1 bagintisini kullaniriz
                         //ilk satir 1 olucak cunku diger takim hic kazanmadiysa biz kazanmisizdir
                         // ilk sutunun tum satirlari 0 olacak cunku diger takim hep kazandiysa biz kazanamamisizdir
    float p = 0.6;
    float **game = (float **)malloc(sizeof(float *) * n); // olasiliklari tutmak icin matrise yer aciyoruz
    for (i = 0; i < n; i++)
        game[i] = (float *)malloc(n * sizeof(float));

    for (i = 1; i < n; i++) // ilk satir ve ilk sutunu belirlenen degerlerle dolduruyoruz
    {
        game[i][0] = 0;
        game[0][i] = 1;
    }

    for (i = 1; i < n; i++)
    {
        for (j = 1; j < n; j++)
        {
            // P(i, j) = p.P(i - 1, j) + (1 - p).P(i, j - 1)
            // bagintiyi uyguluyoruz
            game[i][j] = p * game[i - 1][j] + (1 - p) * game[i][j - 1];
        }
    }
    printf("A Takiminin kazanma ihtimali : %f", game[n - 1][n - 1]);
    return 0;
}