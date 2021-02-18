#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main()
{
    srand(time(NULL));
    int n = 1000, i, j;
    int *arr = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) // diziye 0-1000 arasında random deger atilir
    {
        arr[i] = rand() % n;
    }
    int diff = abs(arr[1] - arr[0]); // diff'in baslangici olması icin ilk elemanların farki alindi
    int newDiff;                     // difference kontrolu icin newDiff
    int indexF, indexS;              // en yakın olan indexleri tutmak icin tanımlama
    for (i = 0; i < n - 1; i++)
    {
        for (j = 1; j < n; j++)
        {
            if (i != j)
            {
                newDiff = abs(arr[i] - arr[j]); // fark alinip
                if (newDiff < diff)             // eski diff'ten kucuk mu kontrolu yapildi
                {
                    diff = newDiff; // kucukse diff degisir
                    indexF = i;     // bulundugumuz indisler tutulur output icin
                    indexS = j;
                }
            }
        }
    }
    printf("minimum difference is %d, between %d and %d.", diff, arr[indexF], arr[indexS]);
    return 0;
}
