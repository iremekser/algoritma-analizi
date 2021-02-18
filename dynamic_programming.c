#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int indexOfColor(char **colors, char *letter)
{
    // kullanicinin girdigi kelimelerin colors string dizisindeki index karsiligini bulan fonksiyon
    // strcasecmp fonk fullanılarak esitlik kontrol edilir
    // esitlik saglanana kadar devam edilir
    // esitlik varsa index dondurulur
    // yoksa -1 gonderilir hatali oldugunu belirtmek icin
    int i = 0;
    while (i < 8 && strcasecmp(colors[i], letter))
        i++;
    if (i == 8)
        return -1;
    else if (!strcasecmp(colors[i], letter))
        return i;
}
void printMatrix(int N, int colorMatrix[N][N], char **colors)
{
    // matrisi renk isimleri ve renklerin indexleri ile yazdirmak icin kullanilan fonksiyon
    // her satirda hem isimler hem indexler yazdirilir colors string dizisi sayesinde
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            printf("%s  ", colors[colorMatrix[i][j]]);
        printf("    ");
        for (j = 0; j < N; j++)
            printf("%d  ", colorMatrix[i][j]);
        printf("\n");
    }
}

void shift(int N, int colorMatrix[N][N], int row)
{
    // satirdaki ilk eleman temp'te tutulur
    // digerleri sondan baslayarak bir ileri shift edilir
    // shift isleminde circle olacagi icin mod kullanilir
    // sonuncu eleman ilk elemanin ustune geldigi icin
    // ilk elemani kaybetmemek icin temp'te tutulur
    // en son colorMatrix[row][1]'a temp'te tutulan ilk eleman yerlestirilir
    int temp = colorMatrix[row][0];
    int i;
    for (i = N - 1; i > 0; i--)
        colorMatrix[row][(i + 1) % N] = colorMatrix[row][i];
    colorMatrix[row][1] = temp;
}

int isExist(int N, int colorMatrix[N][N], int row)
{
    // baktıgimiz satirla ust satirlar arasinda aynilik var mi
    // diye kontrol edilen fonksiyon
    // eger aynilik varsa fonksiyondan return ile cikilir
    // yoksa 0 dondurulur
    int i, j;
    for (i = row - 1; i >= 0; i--)
    {
        for (j = 0; j < N; j++)
        {
            if (colorMatrix[row][j] == colorMatrix[i][j])
                return 1;
        }
    }
    return 0;
}

int control(int N, int colorMatrix[N][N], int row, char **colors, int cp)
{
    // bakilan satir N degerine ulastiysa dogrudur ve 1 dondururuz
    // ustteki satirlar kontrol edilip aynilik durumuna bakilir
    // ayniysa satiri saga kaydiririz
    // N-1 degisime ragmen aynilik varsa 0 dondururuz
    // ustteki satirlarla aynilik yoksa alt satir icin control() fonksyonu cagirilir
    int i;
    if (row == N)
        return 1;
    for (i = 0; i < N; i++)
    {
        if (isExist(N, colorMatrix, row))
        {
            // aynilik olmasi durumu
            if (i == N - 1)
                return 0;
            shift(N, colorMatrix, row);
            if (cp == 1)
            {
                printMatrix(N, colorMatrix, colors);
                printf("*************\n");
            }
        }
        else
        {
            // aynilik yoksa
            if (control(N, colorMatrix, row + 1, colors, cp))
                return 1;
        }
    }
    return 0;
}

int main()
{
    // kullanicidan istenen araliklarda N degeri alinir
    int N;
    printf("3 ile 8 arasinda N degerini girin : ");
    scanf("%d", &N);
    while (N < 3 || N > 8)
    {
        printf("Aralik disinda deger girdiniz. Tekrar N degeri girin : ");
        scanf("%d", &N);
    }
    int colorMatrix[N][N];
    int index;
    int cp;
    int i, j;
    // renklerin tanimlanmasi
    char **colors = (char **)malloc(sizeof(char *) * 8);
    for (i = 0; i < 8; i++)
    {
        colors[i] = (char *)malloc(sizeof(char) * 10);
    }
    colors[0] = "kirmizi";
    colors[1] = "mavi";
    colors[2] = "mor";
    colors[3] = "yesil";
    colors[4] = "bej";
    colors[5] = "bordo";
    colors[6] = "beyaz";
    colors[7] = "fildisi";
    printf("Renkler : ");
    for (i = 0; i < 8; i++)
    {
        printf("%s(%d) ", colors[i], i);
    }
    printf("\nRenklerinizi secin\n");
    // kullanicidan renklerin girilmesiyle indexOfColor fonksiyonu cagirilir
    // rengin colors'taki indexi bulunur colorMatrix'e eklenir
    // kullanici hatali giris yaptiysa uyarilir
    // tekrar renk girmesi beklenir
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            char *color = (char *)malloc(sizeof(char) * 10);
            printf("colorMatrix[%d][%d] : ", i, j);
            scanf("%s", color);
            while (indexOfColor(colors, color) == -1)
            {
                printf("Boyle bir renk mevcut degil. colorMatrix[%d][%d] icin tekrar renk giriniz : ", i, j);
                scanf("%s", color);
            }
            colorMatrix[i][j] = indexOfColor(colors, color);
        }
    }
    // kullaniciya cikti secenekleri sunulup secim yapmasi istenir
    printf("Matrisin degisim adimlarini gormek isterseniz 1 istemezseniz 2 giriniz : ");
    scanf("%d", &cp);
    int c = control(N, colorMatrix, 0, colors, cp);
    if (c == 0)
        printf("Girilen degerlerin istenen cozumu yok.");
    else if (c != 0)
    {
        printf("Sonuc Matrisi\n");
        printMatrix(N, colorMatrix, colors);
    }
    return 0;
}
