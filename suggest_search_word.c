#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define M 997 //1000' en yakin asal sayi
#define MM 996

int editDistance(char *word, char *wrongWord, int limit)
{
    // Levenshtein Edit Distance algoritmasi kullanilarak
    // verilen kelimenin, diger kelimelerle arasindaki degisiklik farkini buluyoruz
    // derste verilen bagintilar ve pseudo code kullanilarak yazilmistir
    int length1 = strlen(word);
    int length2 = strlen(wrongWord);
    int wrongTable[length1 + 1][length2 + 1], i, j, del, insert, sub, min;
    char c1, c2;
    int control;
    for (i = 0; i <= length1; i++)
    {
        wrongTable[i][0] = i;
    }
    for (i = 0; i <= length2; i++)
    {
        wrongTable[0][i] = i;
    }
    for (i = 1; i <= length1; i++)
    {
        control = wrongTable[i][0];
        c1 = word[i - 1];
        for (j = 1; j <= length2; j++)
        {

            c2 = wrongWord[j - 1];
            if (c1 == c2)
            {
                wrongTable[i][j] = wrongTable[i - 1][j - 1];
            }
            else
            {
                min = wrongTable[i - 1][j] + 1;
                insert = wrongTable[i][j - 1] + 1;
                sub = wrongTable[i - 1][j - 1] + 1;
                if (insert < min)
                    min = insert;
                if (sub < min)
                    min = sub;
                wrongTable[i][j] = min;
            }
            if (wrongTable[i][j] < control)
                control = wrongTable[i][j];
        }
        // satirdaki sayilar belirlenen limit'ten buyukse return ediyoruz
        // ki istemedigimiz kosullari kontrol etmeden cikalim
        if (control > limit)
            return -1;
    }
    // edit distance ı kullanmak icin return ediyoruz
    return wrongTable[length1][length2];
}

unsigned long hornerCalculation(char *doc)
{ // HW2'de yazdigim hornerCalculation fonksiyonu
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
{ // HW2'de yazdigim double hashing yapmak icin kullanilan fonksiyon
    int h1 = docValue % M;
    int h2 = 1 + (docValue % MM);
    int h = (h1 + (i * h2)) % M;
    return h;
}

int searchTable(char **table, char *word)
{
    // smallDictionary'den aldigimiz kelimeleri hashleyip yerlestirdigimiz tablo icin
    // tabloda arama fonksiyonu
    // hornerCalculation ile hesaplayip hashliyoruz
    // HW2'de yaptigim islemler
    //hashlenmis degeri donduryoruz
    int k = 0;
    unsigned long docValue = hornerCalculation(word);
    int a = word[0];
    int docHash = hash(docValue, a);
    while (k < M && table[docHash] != "-1" && strcasecmp(table[docHash], word))
    {
        k++;
        a++;
        docHash = hash(docValue, a);
    }
    if (k == M)
        return -1;
    return docHash;
}
int searchWrongTable(char ***table, char *word)
{
    // wrongHashTable'da yanlis kelimelere onerilen kelimeleri de tutmamiz istendigi icin
    // bu tabloda arama yaparken kullandigim fonksiyon
    // searchTable fonksiyonuna benzer fakat burda satirdaki dizilerin ilk elemanlarini aliyoruz sadece
    int k = 0;
    unsigned long docValue = hornerCalculation(word);
    int a = word[0];
    int docHash = hash(docValue, a);
    while (k < M && table[docHash][0] != "-1" && strcasecmp(table[docHash][0], word))
    {
        k++;
        a++;
        docHash = hash(docValue, a);
    }
    if (k == M)
        return -1;
    return docHash;
}

void setHashTable(char **hashTable, char *doc)
{
    // hashTable'ı doldururken geldigimiz adreste "-1" varsa
    // "-1" degeri bos oldugu anlamindadir
    // main icerisinde bu sekilde tanimlanmistir
    // hashTable[docHash]. elemana kutuphanemizdeki kelimelerden gelen doc'u ekliyoruz
    int docHash = searchTable(hashTable, doc);
    if (hashTable[docHash] == "-1")
        hashTable[docHash] = doc;
    return;
}

int findSuggestedWords(char **hashTable, char *wrongWord, char **suggested, int limit)
{
    // once edit distance 1 olanlar kontrol edilmek isteniyor
    // hashTable'daki degerlerin "-1" olmadigi yerlerdeki kelimelerle
    // kullanicidan aldigimiz yanlis kelimeyi karsilastiriyoruz
    // distance 1 ise onerilecek kelimeleri tuttugum suggested[] dizisine ekliyoruz
    // eger distance'ı 1 olan kelime yoksa 2 olanlara bakiyoruz
    // bunun kontrolunu k ile yapiyorum
    // k hic ilerletilmediyse 1 yok demektir o zaman distance'lari 2 olan kelimelere de bakmam gerekir
    // hala onerilecek kelime yoksa yani distance 2 olan kelimeler de yoksa FALSE donduruyorum
    // onun disinda ilerlettigim k'yı return ediyorum ki suggested'in uzunlugunu bilip ona gore
    // oneri yapabilelim
    int ed;
    int k = 0;
    int i;
    for (i = 0; i < M; i++)
    {
        if (hashTable[i] != "-1")
        {
            ed = editDistance(hashTable[i], wrongWord, limit);
            if (ed == 1)
            {
                suggested[k] = hashTable[i];
                k++;
            }
        }
    }
    if (k == 0)
    {
        for (i = 0; i < M; i++)
        {
            if (hashTable[i] != "-1")
            {
                ed = editDistance(hashTable[i], wrongWord, limit);
                if (ed == 2)
                {
                    suggested[k] = hashTable[i];
                    k++;
                }
            }
        }
        if (k == 0)
            return -1; // onerilcek kelime yok
    }
    return k;
}

char *searchWrongHashTable(char ***wrongHashTable, char *wrongWord, char **hashTable, int limit)
{
    int count;
    int x, j, i, index;
    // gelen yanlis kelime wrongHashTable'da var mi diye kontrol ediyoruz
    // yanlis kelimenin yerine gecmesini istedigimiz kelime icin de yer aciyoruz
    // onerilecek kelimeleri tutmak icin suggested dizisini aciyoruz
    int docHash = searchWrongTable(wrongHashTable, wrongWord);
    char *wanted = (char *)malloc(sizeof(char) * 100);

    char **suggested = (char **)malloc(sizeof(char *) * 30);
    for (index = 0; index < 30; index++)
        suggested[index] = (char *)malloc(sizeof(char) * 100);
    // eger aranan yanlis kelime wrongHashTable'da yoksa edit Distance kontrolu yaptiririz
    // onerilen kelimeleri yazdiririz ve kullanicidan secim yapmasini isteriz
    if (wrongHashTable[docHash][0] == "-1")
    {
        count = findSuggestedWords(hashTable, wrongWord, suggested, limit);
        if (count != -1)
        {

            printf("'%s' icin onerilen kelimeler\n", wrongWord);
            for (i = 0; i < count; i++)
            {
                printf("%d : %s\n", i + 1, suggested[i]);
            }
            printf("Degistirmek istediginiz kelimenin numarasini girin yoksa -1 girin : ");
            scanf("%d", &x);
            if (x != -1)
            {
                // kullanici onerilen kelimelerden bir kelime secerse yanlis olan kelimeyi wrongHashTable'a ekleriz
                // bu kelimenin oldugu satirdaki diger sutuna ise onerilen kelimeyi ekleriz
                // onerilen kelimeyi stringde duzeltmek ve yazdirmak icin geri dondururuz
                wrongHashTable[docHash][0] = wrongWord;
                wanted = suggested[x - 1];
                j = 1;
                while (wrongHashTable[docHash][j] != "-1")
                    j++;
                wrongHashTable[docHash][j] = wanted;
                return wrongHashTable[docHash][j];
            }
            if (x == -1)
                return "-2";
        }
        else if (count == -1)
            return "-1";
    }
    else if (!strcasecmp(wrongHashTable[docHash][0], wrongWord))
    {
        // kullanicinin gonderdigi yanlis kelime wrongHashTable'da varsa
        // o satirdaki sutunlara bakariz
        // daha once onerilmis ve secilmis olan kelimeyi kullaniciya oneririz
        // kullaniciya secim yaptiririz
        // secimine gore istenen kelimeyi dondururuz
        j = 1;
        while (wrongHashTable[docHash][j] != "-1")
        {
            printf("%d : %s\n", j, wrongHashTable[docHash][j]);
            j++;
        }
        printf("Daha onceden girilmis hatali kelimelerden degistirmek istediginiz kelimenin numarasini girin yoksa -1 girin : ");
        scanf("%d", &x);
        if (x == -1)
            return "-2";
        return wrongHashTable[docHash][x];
    }
}

int searchWord(char **hashTable, char *doc)
{
    // kutuphanemizdeki kelime hashTable'da var mi,
    // ilerletmemiz gereken adimlar var mi bu sorulari kontrol ederiz
    // hashTable'da varsa kelime true dondururuz
    int docHash = searchTable(hashTable, doc);
    if (!strcasecmp(hashTable[docHash], doc))
        return 1;
    return -1;
}

void splitDictionaryForHashTable(char *docName, char **hashTable)
{
    // HW2'de yazdigim split fonksiyonu
    FILE *f;
    char docs[10000];
    f = fopen(docName, "r");
    fgets(docs, 10000, f);

    char *doc;
    int i = 0;
    int j = 0;
    int l;
    int n;
    int c = 0;
    while (docs[j] != '\n')
    {
        if (docs[j] != ' ')
        {
            j++;
        }
        else
        {
            l = 0;
            n = j - i;
            doc = (char *)calloc(100, sizeof(char));

            while (j - i > 0)
            {
                doc[l] = docs[i];
                i++;
                l++;
            }
            c++;
            i++;
            setHashTable(hashTable, doc);
            j++;
        }
    }
    l = 0;
    n = j - i;
    doc = (char *)calloc(100, sizeof(char));

    while (j - i > 0)
    {
        *(doc + l) = docs[i];
        i++;
        l++;
    }
    setHashTable(hashTable, doc);
}
int splitString(char *string, char **words)
{
    // HW2'de yazdigim split fonksiyonu
    // diger split icinde setHashTable ve open-close file islemleri oldugu icin
    // stringi parcalara ayirip verilen diziye doldurmak icin
    // gerekli fonksiyon
    int ln = strlen(string);
    char *doc;
    int i = 0;
    int j = 0;
    int l;
    int n;
    int c = 0;
    while (j < ln)
    {
        if (string[j] != ' ')
        {
            j++;
        }
        else
        {
            l = 0;
            n = j - i;
            doc = (char *)calloc(100, sizeof(char));
            while (j - i > 0)
            {
                doc[l] = string[i];
                i++;
                l++;
            }
            words[c] = doc;
            c++;
            i++;
            j++;
        }
    }
    l = 0;
    n = j - i;
    doc = (char *)calloc(100, sizeof(char));
    while (j - i > 0)
    {
        doc[l] = string[i];
        i++;
        l++;
    }
    words[c] = doc;
    c++;
    return c;
}

int main()
{
    // table'lar icin yer acilir
    char **hashTable = (char **)malloc(sizeof(char *) * M);
    char ***wrongHashTable = (char ***)malloc(sizeof(char **) * M);
    int index;
    char **words = (char **)malloc(sizeof(char *) * 10);
    for (index = 0; index < 10; index++)
        words[index] = (char *)malloc(sizeof(char) * 100);
    int change = 0;
    int i;
    int control;
    int c;
    int limit = 2;
    char string[1000];
    // table'ların icini "-1" ile dolduruyoruz
    // bos - dolu kontrolunu "-1" 'e gore yapiyor olacagiz
    for (i = 0; i < M; i++)
    {
        hashTable[i] = "-1";
        wrongHashTable[i] = (char **)malloc(sizeof(char *) * 10);
        wrongHashTable[i][0] = (char *)malloc(100 * sizeof(char));
        wrongHashTable[i][0] = "-1";
        for (index = 1; index < 9; index++)
        {
            wrongHashTable[i][index] = (char *)malloc(100 * sizeof(char));
            wrongHashTable[i][index] = "-1";
        }
    }
    // kullanicidan stringi alip split fonksiyonuna gondeririz
    printf("Kontrol etmek istediginiz cumleyi girin(orn : it is good) : ");
    gets(string);
    c = splitString(string, words);
    // kutuphanemizdeki kelimeleri hashliyoruz ve tabloya aliyoruz
    splitDictionaryForHashTable("smallDictionary.txt", hashTable);
    for (i = 0; i < c; i++)
    {
        // tek tek stringdeki kelimeler kontrol edilir
        // verilen kosullara gore fonksiyonlar cagirilir
        control = searchWord(hashTable, words[i]);
        if (control == -1)
        {
            change = 1;
            char *newWord = (char *)malloc(sizeof(char) * 100);
            newWord = searchWrongHashTable(wrongHashTable, words[i], hashTable, limit);
            if (newWord != "-1" && newWord != "-2")
                words[i] = newWord;
            else if (newWord == "-2")
            {
                // onerilen kelimelerden secim olmazsa bunu yazdiririz
                change = 2;
                printf("'%s' icin onerilen kelimelerden herhangi bir degisim istegi bulunmamistir.\n", words[i]);
            }
            else if (newWord == "-1")
            {
                change = 3;
                printf("'%s' icin onerilecek kelime yok.\n", words[i]);
            }
        }
    }
    if (change == 0)
        printf("Butun kelimeler dogru girildi.");
    else if (change == 2)
        printf("Onerilen kelimelerden herhangi bir degisim istegi bulunmamistir.");
    else if (change == 3)
        printf("Onerilecek kelime yoktur.");
    else
    {
        printf("Girilen cumlenin duzeltilmis hali : ");
        for (i = 0; i < c; i++)
        {
            printf("%s ", words[i]);
        }
    }
    return 0;
}
