#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define M 997 //1000' en yakin asal sayi
#define MM 996

typedef struct element
{
    int key;
    char *word;
    char *document[500];
    int docIndex;
} ELEMENT;
int allCount = 0;
void writeToFile(ELEMENT **hashTable, int count)
{ //hashtablei dosyaya yazdirmak icin gereken fonksiyon
    //ustune yazmak istedigimiz dosyayi aciyoruz
    //ilk satira loadfactor hesaplamak icin hashtableda kac eleman var onu tutuyoruz
    //M zaten sabit
    //eger hashtable bossa - nullsa - yazdiriyoruz dosyaya
    //bos degilse yeni bir stringe ilk elemani kelime
    //virgulden sonrasi dokuman isimleri olacak sekilde ekliyoruz
    //virgul ve dokuman isimleri eklerken string.h kutuphanesinin
    //strcat fonksiyonunu kullaniyoruz
    //fprintf ile dosyaya yazdiriyoruz stringi
    //en sonda actigimiz dosyayi kapatiyoruz
    FILE *hashFile = fopen("17011022.txt", "w");
    fprintf(hashFile, "%d\n", count);
    int i, j;
    for (i = 0; i < M; i++)
    {
        if (!hashTable[i])
            fprintf(hashFile, "-\n");
        else
        {
            char *obj = (char *)malloc(sizeof(char) * 500);
            strcpy(obj, hashTable[i]->word);
            for (j = 0; j < hashTable[i]->docIndex; j++)
            {
                strcat(obj, ",");
                strcat(obj, hashTable[i]->document[j]);
            }
            fprintf(hashFile, "%s\n", obj);
        }
    }
    fclose(hashFile);
}

void readFile(FILE *f, ELEMENT **hashTable)
{ //dosyadan okuma islemi yapmak icin gereken fonksiyon
    //eger dosya yoksa return edip cikiyoruz
    //ilk satiri count olarak aliyoruz loadfactor hesaplamasinda kullanmak icin
    //dosyanin bitimine kadar satirlara sirayla bakiyoruz
    //her satiri bir stringe aliyoruz
    //eger string - ise hashtable da i nin denk geldigi elemani null a esitliyorz
    //diger kosulda ise hashtable da i.eleman icin yer aciyoruz
    //kelimeyi yerlestirmek icin kelime icin de yer aciyoruz
    //string de ilk virgule kadar olan kisim kelimemiz
    //string.h kutuphanesinin strtok fonksiyonuyla
    //stringi virgullere boluyoruz
    //ilk virgulden sonrakiler dokuman isimleri olur
    //ve her virgulde dokuman sayisini bulmak icin tuttugumuz indexi
    //bir arttiririz
    //olusturdugumuz elementi ilgili hashtable bolumune esitleme yaparak
    //yerlestiririz
    if (!f)
        return;
    int i = 0;
    fscanf(f, "%d\n", &allCount);
    int index = 0;

    char *newObj = (char *)malloc(sizeof(char) * 500);
    while (!feof(f))
    {
        fgets(newObj, 500, f);
        if (newObj[0] == '-')
        {
            hashTable[i] = NULL;
        }
        else
        {
            hashTable[i] = (ELEMENT *)malloc(sizeof(ELEMENT));
            hashTable[i]->word = (char *)malloc(sizeof(char) * 500);
            char *piece = strtok(newObj, ","); //word
            hashTable[i]->word = piece;
            while (piece != NULL)
            {
                piece = strtok(NULL, ",");
                if (piece != NULL)
                {
                    hashTable[i]->document[index] = (char *)malloc(sizeof(char) * 500);
                    hashTable[i]->document[index] = piece;
                    index++;
                }
            }
            hashTable[i]->docIndex = index;
            index = 0;
            hashTable[i]->key = i;
        }
        i++;
        newObj = (char *)malloc(sizeof(char) * 500);
    }
}

unsigned long hornerCalculation(char *doc)
{
    //gelen kelimenin uzunlugu kadar for donup
    //buyuk ve kucuk farkini gozetmeksizin hesaplamayi yapariz
    //aldigimiz kelimeyle A ya da a arasindaki farki bulursak
    //harfimizin sirasina dolayisiyla harfimize ulasiriz
    //sonucu return ederiz
    int i, n = strlen(doc);
    unsigned long calc = 0;
    int place;
    for (i = 0; i < n; i++)
    {
        if (*(doc + i) >= 'A' && *(doc + i) <= 'Z')
            place = (*(doc + i)) - 'A';
        else if (*(doc + i) >= 'a' && *(doc + i) <= 'z')
            place = (*(doc + i)) - 'a';
        calc += place * pow(10, i);
    }
    return calc;
}

int hash(unsigned long docValue, int i)
{ //double hashing
    //soruda verildigi gibi double hash islemini yaparız
    //M 997 secilir yani 1000e en yakin asal sayi
    //hash sonucunu return ederiz
    int h1 = docValue % M;
    int h2 = 1 + (docValue % MM);
    int h = (h1 + (i * h2)) % M;
    return h;
}

int searchWord(ELEMENT **hashTable, char *doc)
{ //istenen kelimeyi aramak icin gereken fonksiyon
    //kelimenin hash sonucunu bulduktan sonra
    //indexteki eleman bos degilse ve kelimeye esit degilse
    //indexi artirmaya devam ediyoruz
    //eger hepsine bakip ciktiysak ya da baktigimiz index null a esitse
    //-1 return edip kelimenin olmadigini belirtiyoruz
    //diger kosulda docHash i return ediyoruz.
    int i = 0;
    unsigned long docValue = hornerCalculation(doc);
    int a = doc[0];
    int docHash = hash(docValue, a);
    while (i < M && hashTable[docHash] && strcasecmp(hashTable[docHash]->word, doc))
    {
        i++;
        a++;
        docHash = hash(docValue, a);
        printf("%d. kez docHash yeniden hesaplanıyor..r\n", i);
    }

    if (hashTable[docHash] == NULL || (i == M))
        return -1;
    else
        return docHash;
}

float loadFactor()
{
    //hashtabledaki toplam bulunan eleman sayisini
    //toplam tablonun boyutuna bolerek
    //tablonun doluluk oranini buluyoruz
    return (float)allCount / (float)M;
}

void setHashTable(ELEMENT **hashTable, char *doc, char *docName, int *count)
{
    //kelime icin gereken hashleme islemlerinden sonra
    //dochash e ait hashtable elementi varsa ve aradidigimiz kelimeye esit degilse
    //indexleri arttiriyoruz
    //kontrollerimizde eger hashtable[dochash] nullsa yani yoksa
    //elementimizi olusturuyoruz
    //dokuman adimizi ekliyoruz
    //loadfactoru yazdiriyoruz
    //diger turlu kelime varsa daha onceden
    //dokumanlarina yenisini ekliyoruz
    //loadfactor 1 i gecerse set etme islemini durduruyoruz
    //eklenemeyen kelimeleri yazdiriyoruz
    int k = 0;
    int x = 0;
    int c = 0;
    int i;
    unsigned long docValue = hornerCalculation(doc);
    int a = doc[0];
    int docHash = hash(docValue, a);
    while (k < M && hashTable[docHash] && strcasecmp(hashTable[docHash]->word, doc))
    {
        k++;
        a++;
        docHash = hash(docValue, a);
    }
    if (!hashTable[docHash])
    {
        ELEMENT *element = (ELEMENT *)malloc(sizeof(ELEMENT));
        element->key = docHash;
        element->word = (char *)malloc(sizeof(char) * 500);
        strcpy(element->word, doc);
        element->document[0] = (char *)malloc(sizeof(char) * 500);
        strcpy(element->document[0], docName);
        element->docIndex = 1;
        hashTable[docHash] = element;
        c += 1;
        printf("eklenen kelime : %s loadFactor : %f\n", hashTable[docHash]->word, loadFactor());
        if (loadFactor() > 0.8)
            printf("loadFactor 0.8i gecti\n");
    }
    else if (loadFactor() == 1.0)
    {
        printf("durduruldu.\n");
        printf("eklenemeyen kelime : %s", doc);
    }
    else if (k == M)
        printf("tablo dolu");
    else
    {
        for ( i = 0; i < hashTable[docHash]->docIndex; i++)
        {
            if (!strcmp(hashTable[docHash]->document[i], docName))
                x = 1;
        }
        if (x == 0)
        {
            hashTable[docHash]->document[hashTable[docHash]->docIndex] = (char *)malloc(sizeof(char) * 500);
            strcpy(hashTable[docHash]->document[hashTable[docHash]->docIndex], docName);
            hashTable[docHash]->docIndex++;
        }
    }
    *count = c;
    allCount += c;
}

void splitDocument(char *docName, ELEMENT **hashTable, int *count)
{
    //dosyadan aldigimiz stringi split etmek icin gereken fonk.
    //dosya formatimiz tek satir ve ozel karaktersiz olmalı
    //en son alt satira geciyor olmali
    //kelimeleri gezerken kelimenin ilk indexi ile
    //son indexini tutup, n = j - i ile kelime icin n kadar yer acip
    //kelime kelime ayirmis oluyoruz stringimizi
    //ayirmis oldugumuz kelimeleri direkt olarak ayirdigimizda
    //sethashtable a gonderiyoruz islemlerden geciriyoruz
    FILE *f;
    char docs[2000];
    f = fopen(docName, "r"); //dokuman
    fgets(docs, 2000, f);
    int k;
    int i = 0;
    int j = 0;
    int l = 0;
    int n;
    int c;
    while (docs[j] != '\n')
    {
        if (docs[j] != ' ')
        {
            j++;
        }
        else
        {
            n = j - i;
            char *doc = (char *)malloc(n * sizeof(char));
            for ( k = i; k < j; k++)
            {
                *(doc + l) = docs[i++];
                l++;
            }
            setHashTable(hashTable, doc, docName, &c);
            j++;
            i = j;
            l = 0;
        }
    }
    n = j - i;
    char *doc = (char *)malloc(n * sizeof(char));
    for ( k = i; k < j; k++)
    {
        *(doc + l) = docs[i++];
        l++;
    }
    setHashTable(hashTable, doc, docName, &c);
    *count = c;
    fclose(f);
}

int main()
{
    //hashtableimizi basta butun elemanlari null olacak
    //sekilde ayarliyoruz
    //hashtable ı tuttugumuz dosyayi aciyoruz
    //dosyadan verileri okuyup hashtable a esitliyoruz
    //kullaniciya secim yaptirarak istenen isleme uygun
    //fonksiyonlari cagiriyoruz
    ELEMENT **hashTable = (ELEMENT **)malloc(sizeof(ELEMENT *) * M);
    int i;
    for (i = 0; i < M; i++)
    {
        hashTable[i] = NULL;
    }
    FILE *f = fopen("17011022.txt", "r+");
    int count = 0;
    int c = 0;
    float loadFactor;
    char docName[500];
    int k = -1;
    int k1;
    char sWord[50];

    readFile(f, hashTable);
    while (k != 2)
    {
        printf("dosya eklemek icin 0 - kelime aramak icin 1 - cikis yapmak icin 2 basiniz : ");
        scanf("%d", &k); //secim
        if (k == 0)
        { //dokuman ekleme
            //dokumanin adini aliyoruz
            //aldigimiz stringi split ediyoruz
            //hashtable i dosyaya yazdiriyoruz
            printf("dosya adini giriniz (orn:deneme.txt) : ");
            scanf("%s", &docName);
            count = 0;
            splitDocument(docName, hashTable, &count);
            c += count;
            writeToFile(hashTable, allCount);
        }
        else if (k == 1)
        { // kelime arama
            //aranmak istenen kelimeyi alip
            //searchWord fonksiyonuna gonderiyoruz
            //eger gelen k1 -1 ise bulunamamis demektir
            //diger kosulda buldugumuz indexteki hashtable in
            //dokumanlarini yazdiriyoruz
            printf("arama yapmak istediginiz kelimeyi giriniz : ");
            scanf("%s", &sWord);
            readFile(f, hashTable);
            k1 = searchWord(hashTable, sWord);
            if (k1 == -1)
                printf("bulunamadi\n");
            else
            {

                for (i = 0; i < hashTable[k1]->docIndex; i++)
                    printf("%s\n", hashTable[k1]->document[i]);
            }
        }
    }
    return 0;
}
