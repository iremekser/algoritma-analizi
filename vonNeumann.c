#include <stdio.h>
#include <stdlib.h>
void vonNeumann(int **arr, int x, int y, int n, int k)
{
	int i;
    if (k == 0) // eger matrisin ortasındaki satira bakıyosak bu if kolu calisir
    {           // ve recursive'den cikis icin return islemi vardir
        for (i = 0; i <= n; i++)
        {
            arr[n + 1][n + 1 + i] = 1;
            arr[n + 1][n + 1 - i] = 1;
        }
        return;
    }
    // girilen ilk x ve y degerleri icin degisim islemi gerceklestirilir 0 -> 1
    // daha sonrasinda simetrigi olan x ve y degerinin de degisim islemi gerceklesir 0 -> 1
    // main'den k = n olarak gelir fonk.
    // girilen n degeriyle k degeri arasinda ne kadar fark varsa o kadar saga ve sola 0 -> 1 islemi yapilacak demektir
    // ornegin n = 3 iken k = 2 ise 1 (n - k) kadar her tarafa 0 -> 1 islemi yapilmalidir
    // ve yapilan islemler x + 2 * k seklinde yazilarak da simetrisine de 0 -> 1 islemi yapilir
    // her seferinde k azaltilir
    // x arttirilir, satir olarak ilerletilir
    // baslangictan ortaya kadar 1'ler artacagi icin k azalirken x artar
    arr[x][y] = 1;
    arr[x + 2 * k][y] = 1;
    for (i = 1; i <= n - k; i++)
    {
        arr[x][y + i] = 1;
        arr[x][y - i] = 1;
        arr[x + 2 * k][y - i] = 1;
        arr[x + 2 * k][y + i] = 1;
    }
    vonNeumann(arr, x + 1, y, n, k - 1);
}
int main()
{
    int n;
    int count = 0, countRow, i, j;
    printf("n : ");
    scanf("%d", &n); //islem yapilmasi istenen n kullanicidan alinir
    // dinamik bellekle matris olusturma
    // girilen n degerine gore matris boyutu (2*n+3) X (2*n+3) olmalidir
    int **arr = (int **)malloc((2 * n + 3) * sizeof(int *));
    for (i = 0; i < 2 * n + 3; i++)
    {
        arr[i] = (int *)malloc((2 * n + 3) * sizeof(int));
    }

    for (i = 0; i < 2 * n + 3; i++)
    {
        for (j = 0; j < 2 * n + 3; j++)
        {
            arr[i][j] = 0; // acilan matrisin ici 0'larla doldurma
        }
    }

    vonNeumann(arr, 1, n + 1, n, n); // fonksiyonu cagirma
    for (i = 0; i < 2 * n + 3; i++)
    {
        countRow = 0;
        for (j = 0; j < 2 * n + 3; j++)
        {
            if (arr[i][j] == 1)
            {
                count += 1;
                countRow += 1;
            }
            printf("%d ", arr[i][j]); // matrisi ekrana yazdirma
        }
        printf(" -> %d black cells in %d. row \n", countRow, i + 1);
    }
    printf("%d black cells in total", count);
    return 0;
}
