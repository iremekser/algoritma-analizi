#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void merge(int *arr, int p, int q, int r)
{
    // divide and conquer mantigiyla merge sort
    // bol, boldugun parcayi digerine karismadan coz
    // sonra digerini coz, bol...
    // once dizi ikiye bolunur
    // elemanlar yalniz kalana kadar bolunur - divide
    // daha sonra siralaya siralaya birlestirilir - merge
    int i, j, k;
    i = p, j = q + 1, k = 0;
    int b[r - p + 1];
    while ((i <= q) && j <= r)
    {
        if (arr[i] <= arr[j])
        {
            b[k] = arr[i];
            k += 1;
            i += 1;
        }
        else
        {
            b[k] = arr[j];
            k += 1;
            j += 1;
        }
    }
    while (i <= q)
    {
        b[k] = arr[i];
        k += 1;
        i += 1;
    }
    while (j <= r)
    {
        b[k] = arr[j];
        k += 1;
        j += 1;
    }
    for (i = p; i <= r; i += 1)
    {
        arr[i] = b[i - p];
    }
}
void mergeSort(int *arr, int p, int r)
{
    int q;
    if (p < r)
    {
        q = (p + r) / 2;
        mergeSort(arr, p, q);     // sort arr[p...q]
        mergeSort(arr, q + 1, r); // sort arr[q+1...r]
        merge(arr, p, q, r);
    }
}
int main()
{
    srand(time(NULL));
    int n = 1000, i;
    int *arr = (int *)malloc(n * sizeof(int));
    for (i = 0; i < n; i++) // diziye 0-1000 arasında random deger atilir
    {
        arr[i] = rand() % n;
    }
    mergeSort(arr, 0, n - 1);        // dizi merge sort ile siralanir
    int diff = abs(arr[1] - arr[0]); // baslangic icin ilk iki eleman arasindaki fark alinir
    int newDiff;                     // difference kontrolu icin newDiff
    int indexF, indexS;              // en yakın olan indexleri tutmak icin tanımlama

    for (i = 1; i < n - 1; i++)
    {
        newDiff = abs(arr[i + 1] - arr[i]); // fark hesaplama
        if (newDiff < diff)                 // elimizde olan difference'tan kucukse yeni hesaplanan newDiff
        {
            diff = newDiff; // degisim islemi olur
            indexF = i;     // hangi elemanlarin oldugunu belirlemek icin indisler tutulur
            indexS = i + 1;
        }
    }
    printf("minimum difference is %d, between %d and %d.", diff, arr[indexF], arr[indexS]);
    return 0;
}
