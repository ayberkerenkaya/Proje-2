#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESAJ_UZUNLUK 1024  // Bir log sat�r�n�n en fazla ka� karakter olabilece�i

// Loglar� tutacak ba�l� liste d���m� yap�s�
typedef struct LogKaydi {
    char zaman[20];       // Logun tarihi ve saati
    char seviye[10];      // Log seviyesi (INFO, ERROR, WARNING)
    char mesaj[MAX_MESAJ_UZUNLUK];  // Log i�eri�i
    struct LogKaydi *sonraki; // Ba�l� listedeki bir sonraki log kayd�
} LogKaydi;

// Ba�l� listeye yeni log ekleme fonksiyonu
void logEkle(LogKaydi **ilkLog, const char *zaman, const char *seviye, const char *mesaj) {
    LogKaydi *yeniLog = (LogKaydi *)malloc(sizeof(LogKaydi)); // Bellekten yeni bir d���m olu�tur
    if (!yeniLog) { // Bellek hatas� kontrol�
        perror("Bellek tahsis hatas�");
        exit(EXIT_FAILURE);
    }

    strcpy(yeniLog->zaman, zaman);   // Tarih-saat bilgisini ata
    strcpy(yeniLog->seviye, seviye); // Log seviyesini ata
    strcpy(yeniLog->mesaj, mesaj);   // Log mesaj�n� ata
    yeniLog->sonraki = NULL;         // Yeni logun sonraki de�eri ba�lang��ta bo� olsun

    if (*ilkLog == NULL) {  // E�er listede hi� log yoksa yeni log ba�a eklenir
        *ilkLog = yeniLog;
    } else {  // E�er listede loglar varsa sona eklenir
        LogKaydi *gecici = *ilkLog;
        while (gecici->sonraki) {
            gecici = gecici->sonraki;
        }
        gecici->sonraki = yeniLog;
    }
}

// Syslog dosyas�n� okuyup loglar� ba�l� listeye ekleyen fonksiyon
void syslogOku(LogKaydi **ilkLog, const char *dosyaAdi) {
    FILE *dosya = fopen(dosyaAdi, "r"); // Dosyay� okuma modunda a�
    if (!dosya) { // Dosya a��lamazsa hata ver
        perror("Syslog dosyas� a��lamad�");
        exit(EXIT_FAILURE);
    }

    char satir[MAX_MESAJ_UZUNLUK]; // Sat�rlar� tutacak de�i�ken
    while (fgets(satir, sizeof(satir), dosya)) { // Sat�r sat�r oku
        char zaman[20] = "Tarih_Bilinmiyor";
        char seviye[10] = "Bilinmiyor";
        char mesaj[MAX_MESAJ_UZUNLUK];

        // Sat�r� par�alara ay�rarak zaman�, seviyeyi ve mesaj� al
        scanf(satir, "%19s %9s %1023[^"]", zaman, seviye, mesaj);
        
        // Loglar� listeye ekle
        logEkle(ilkLog, zaman, seviye, mesaj);
    }
    fclose(dosya); // Dosyay� kapat
}

// Ba�l� listedeki loglar� ekrana yazd�ran fonksiyon
void loglariGoster(LogKaydi *ilkLog) {
    while (ilkLog) { // Liste boyunca t�m loglar� ekrana yazd�r
        printf("[%s] (%s) %s\n", ilkLog->zaman, ilkLog->seviye, ilkLog->mesaj);
        ilkLog = ilkLog->sonraki;
    }
}

// Ba�l� listeyi temizleyerek belle�i serbest b�rakan fonksiyon
void loglariTemizle(LogKaydi *ilkLog) {
    LogKaydi *gecici;
    while (ilkLog) {
        gecici = ilkLog;
        ilkLog = ilkLog->sonraki;
        free(gecici); // Bellekten kald�r
    }
}

// Ana fonksiyon (program�n ba�lang�� noktas�)
int main() {
    LogKaydi *logListesi = NULL; // Log listesi ba�lang��ta bo�

    // Syslog dosyas�n� oku ve ba�l� listeye ekle
    syslogOku(&logListesi, "/var/log/syslog");

    // Kay�tlar� ekrana yazd�r
    loglariGoster(logListesi);

    // Belle�i temizle
    loglariTemizle(logListesi);

    return 0;
}

