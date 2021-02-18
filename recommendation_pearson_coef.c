#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node
{
    char *name;
    int votes[50];
} Node;

void splitLines(char *string, Node **user, int index)
{
    // csv dosyasindaki ; ile ayrilmis sayilari ayirmak icin kullanilan fonksiyon
    // ilk ;'e kadar olan kisim userName i verir structtaki name'e aliriz
    // daha sonrakiler kitaplar icin verilen puanlardir
    // string seklinde alindigi icin '0' dan cikarilir deger
    // bos oldugu durumlarda - bir sayi geldigi icin
    // cikarma isleminden sonra - sonuc cikarsa 0 koyariz yerine structtaki oylara
    user[index] = (Node *)malloc(sizeof(Node));
    user[index]->name = (char *)calloc(50, sizeof(char));
    int ln = strlen(string);
    char *name;
    int num, i = 0, j = 0, l = 0, c = 0, first = 0;

    while (string[j] != ';')
    {
        j++;
    }
    while (j - i > 0)
    {
        user[index]->name[l] = string[i];
        i++;
        l++;
    }
    i++;
    j++;

    while (j < ln - 1)
    {
        if (string[j] != ';')
        {
            int vote = (int)(string[j] - '0');
            if (vote < 0)
                vote = 0;
            user[index]->votes[c] = vote;
            c++;
        }
        j++;
    }
}

int splitBooks(char *string, char **books)
{
    // csvdeki ilk satirdaki kitaplari ayirmak icin kullanilir
    // kitaplar char **books'da tutulur
    int count = 0;
    char *piece = strtok(string, ";"); //word
    while (piece != NULL)
    {
        piece = strtok(NULL, ";");
        if (piece != NULL)
        {
            books[count] = (char *)calloc(100, sizeof(char));
            strcpy(books[count], piece);
            count++;
        }
    }
    books[count - 1][strlen(books[count - 1]) - 1] = '\0';
    return count;
}

int userToIndex(char *userName, Node **user, int countOfNewUsers)
{
    // kullanicinin girdigi kullanici adinin
    // structtaki kacinci indexe geldigini bulmak icin gereken fonksiyon
    int i = 0;
    while (i < countOfNewUsers && strcasecmp(user[i]->name, userName))
        i++;
    if (i == countOfNewUsers)
        return -1;
    return i;
}

void averageVote(Node **user, int indexOfUser, int step, int countBooks, float *rA, float *rB)
{
    // sim fonksiyonunda a ve b'nin ortak okudugu kitaplarin
    // ortalamasini bulmak icin kullanilan fonksiyon
    // rA - rB : a ve b kullanicilarinin ortak okudugu kitaplar icin hesaplanan ortalama
    // rA ve rB ' nin degerlerini pointerlarla degistiriyoruz
    int a, b, c = 0;
    float sumA = 0, sumB = 0;
    *rA = 0, *rB = 0;
    int j;
    for (j = 0; j < countBooks; j++)
    {
        a = user[indexOfUser]->votes[j];
        b = user[step]->votes[j];
        if (a != 0 && b != 0)
        {
            sumA += a;
            sumB += b;
            c++;
        }
    }
    *rA = sumA / c;
    *rB = sumB / c;
}

float sim(Node **user, int indexOfUser, int step, int countBooks)
{
    // dosyada yazan formulu kullanarak similarity hesabi yapilir
    // averageVote ile rA ve rB hesaplanir
    // daha sonrasinda formul icin gereken matematiksel islemler yapilir
    // hesaplama yapilirken payda kisminin sifir oldugu durumlar da cikmistir
    // bu durumda payda kismina cok kucuk bır sayi eklenip isleme alinmistir (0.0001 gibi)
    int a, b, c = 0, j;
    float sumA, sumB, sumAB, sqrtAB, rA = 0, rB = 0, coef;
    if (step != indexOfUser)
    {
        averageVote(user, indexOfUser, step, countBooks, &rA, &rB);
        sumAB = 0, sumA = 0, sumB = 0, sqrtAB = 0, coef = -2;
        for (j = 0; j < countBooks; j++)
        {
            a = user[indexOfUser]->votes[j];
            b = user[step]->votes[j];
            if (a != 0 && b != 0)
            {
                sumAB += ((a - rA) * (b - rB));
                sumA += pow(a - rA, 2);
                sumB += pow(b - rB, 2);
            }
        }
        float bottom = (sqrt(sumA) * sqrt(sumB));
        if (bottom == 0)
            bottom += 0.0001;
        coef = sumAB / bottom;
        return coef;
    }
    else
        return -2;
}

void pearsonCoef(Node **user, int indexOfUser, int countOfUsers, float *coefs, int countBooks)
{
    // bir okuyucunun diger okuyuculara benzerliginin pearson coefficient
    // kullanilarak hesaplanmasi icin count kadar cagirildigi fonksiyon
    int i;
    for (i = 0; i < countOfUsers; i++)
    {
        coefs[i] = sim(user, indexOfUser, i, countBooks);
    }
}

void findMaxCoefs(float *coefs, float *maxCoefs, int *maxCoefsIndexs, int k, int indexOfUser, Node **user, int countOfUsers)
{
    // kullanicinin girdigi k degeri kadar maxCoef'e sahip olanlari buldugumuz fonksiyon
    // k icin for donuyoruz
    // coefs'teki ilk elemani max olarak kabul ediyoruz
    // daha sonra diger coefs'lere bakiyoruz max'tan daha buyuk coefs varsa
    // max ile yer degistiriyoruz
    // icteki fordan ciktigimiz zaman elimizde kalan max'ı maxCoefs dizisine atiyoruz
    // index'ini de maxCoefsIndexs' e atiyoruz
    // hangi indextekini aldiysak o indexteki coefs degerine (coefs[index]) cok dusuk bir sayi atiyoruz
    // bir daha karsilastigimizda onu almayalim diye
    printf("\n%s kullanicisina en yakin kullanicilar(k = %d) ve hesaplanan pearson benzerlikleri sirasiyla\n", user[indexOfUser]->name, k);
    int i, j, l = 0;
    float max;
    int index;
    for (j = 0; j < k; j++)
    {
        max = coefs[0];
        index = 0;
        for (i = 1; i < countOfUsers; i++)
        {
            if (max < coefs[i])
            {
                max = coefs[i];
                index = i;
            }
        }
        maxCoefs[j] = max;
        maxCoefsIndexs[j] = index;
        printf("%d. : %s - %f\n", j + 1, user[maxCoefsIndexs[j]]->name, maxCoefs[j]);
        coefs[index] = -255;
    }
}

float countOfBooksRead(Node **user, int indexOfUser, int countBooks)
{
    // index'i verilen kullanicin okudugu kitaplarin ortalamasini bulan fonksiyon
    int i;
    float count = 0, sum = 0;
    for (i = 0; i < countBooks; i++)
        if (user[indexOfUser]->votes[i] == 0)
        {
            sum += user[indexOfUser]->votes[i];
            count++;
        }
    return sum / count;
}

void pred(Node **user, float *maxCoefs, int *maxCoefsIndexs, int indexOfUser, int k, int countBooks, char **books)
{
    // index'i verilen user'in okumadigi kitaplar icin verecegi tahmini puanlar
    // ve kitap onerisinin yapildigi fonksiyon
    // kullanicinin verdigi oylara bakarak 0 olan oylar
    // maxCoefsIndexs'teki kullancilarla karsilastirilir
    // dosyada verilen pred formulu kullanilarak kullaninin okumadigi kitaba
    // verecegi tahmini puan hesaplanir
    // okumadigi kitaplar icin verecegi tahmini puanlar yazdirilir
    // en son onerilecek olan icin temp ile oneriler kontrol edilir her seferinde
    // temp'in en son hali max'tir ve onerilecek kitap olarak yazdirilir
    printf("%s kullanicisinda okunmamis olan kitaplar icin hesaplanan tahmini begenme degerleri\n", user[indexOfUser]->name);
    int i, j, controlCount = 0, tempId = 0;
    float rA = 0, rB = 0, simI = 0, top = 0, bottom = 0, diff = 0, res = 0, temp;
    rA = countOfBooksRead(user, indexOfUser, countBooks);
    for (j = 0; j < countBooks; j++)
    {
        if (user[indexOfUser]->votes[j] == 0)
        {
            for (i = 0; i < k; i++)
            {
                rB = countOfBooksRead(user, maxCoefsIndexs[i], countBooks);
                simI = sim(user, indexOfUser, maxCoefsIndexs[i], countBooks);
                diff = (user[maxCoefsIndexs[i]]->votes[j]) - rB;
                top += diff * simI;
                bottom += simI;
            }
            res = rA + (top / bottom);
            printf("%s : %f\n", books[j], res);
            if (controlCount == 0)
            {
                temp = res;
                tempId = j;
            }
            else if (res > temp)
            {
                temp = res;
                tempId = j;
            }
            controlCount++;
        }
    }
    printf("Sonuc olarak onerilen kitap : %s\n", books[tempId]);
}

int main()
{
    // userlar icin structta yer acilir
    // csv dosyasi okunur
    // ilk satir kitaplar oldugu icin
    // ilk satir alinip splitBooks fonksiyonuna gonderilir
    // daha sonraki satirlar userlardir
    // U ile baslayanlar user oldugu icin countOfUsers ayri olarak da tutulur
    // ve toplam user sayisi da hesaplanir newUser sayisini bulabilmek icin
    // islemler icin gereken fonksiyonlar cagirilir
    Node **user = (Node **)malloc(sizeof(Node *) * 200);
    FILE *stream = fopen("RecomendationDataSet.csv", "r");
    char line[1024];
    fgets(line, 1024, stream);
    char **books = (char **)malloc(sizeof(char *));
    int countBooks = splitBooks(strdup(line), books);
    free(strdup(line));
    int countOfLines = 0, countOfUsers = 0, countOfNewUsers;
    while (fgets(line, 1024, stream))
    {
        splitLines(strdup(line), user, countOfLines);
        if (user[countOfLines]->name[0] == 'U')
            countOfUsers++;
        countOfLines++;
        free(strdup(line));
    }
    countOfNewUsers = countOfLines - countOfUsers;
    char *userName = (char *)malloc(sizeof(char) * 50);
    int k = 3;
    printf("Kitap onerisi yapilacak kullanici adi : ");
    gets(userName);
    printf("Benzer kullanici sayisi(k) : ");
    scanf("%d", &k);
    int indexOfUser = userToIndex(userName, user, countOfLines);
    //******
    float *coefs = (float *)malloc(sizeof(float) * countOfUsers);
    pearsonCoef(user, indexOfUser, countOfUsers, coefs, countBooks);
    float *maxCoefs = (float *)malloc(sizeof(float) * k);
    int *maxCoefsIndexs = (int *)malloc(sizeof(int) * k);
    findMaxCoefs(coefs, maxCoefs, maxCoefsIndexs, k, indexOfUser, user, countOfUsers);
    pred(user, maxCoefs, maxCoefsIndexs, indexOfUser, k, countBooks, books);
    // Tasarladigim tavsiye sistemini olcmek icin test kismi
    // for (int i = 0; i < countOfNewUsers; i++)
    // {
    //     int indexOfUser = countOfUsers + i;
    //     //****** ile isaretlenen kisimdan sonrasini buraya tasidigimiz zaman test edebiliyoruz
    // }
}
