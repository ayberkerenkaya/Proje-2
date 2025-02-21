#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESAJ_UZUNLUK 1024  // Bir log satýrýnýn en fazla kaç karakter olabileceði

// Loglarý tutacak baðlý liste düðümü yapýsý
typedef struct LogKaydi {
    char zaman[20];       // Logun tarihi ve saati
    char seviye[10];      // Log seviyesi (INFO, ERROR, WARNING)
    char mesaj[MAX_MESAJ_UZUNLUK];  // Log içeriði
    struct LogKaydi *sonraki; // Baðlý listedeki bir sonraki log kaydý
} LogKaydi;

// Baðlý listeye yeni log ekleme fonksiyonu
void logEkle(LogKaydi **ilkLog, const char *zaman, const char *seviye, const char *mesaj) {
    LogKaydi *yeniLog = (LogKaydi *)malloc(sizeof(LogKaydi)); // Bellekten yeni bir düðüm oluþtur
    if (!yeniLog) { // Bellek hatasý kontrolü
        perror("Bellek tahsis hatasý");
        exit(EXIT_FAILURE);
    }

    strcpy(yeniLog->zaman, zaman);   // Tarih-saat bilgisini ata
    strcpy(yeniLog->seviye, seviye); // Log seviyesini ata
    strcpy(yeniLog->mesaj, mesaj);   // Log mesajýný ata
    yeniLog->sonraki = NULL;         // Yeni logun sonraki deðeri baþlangýçta boþ olsun

    if (*ilkLog == NULL) {  // Eðer listede hiç log yoksa yeni log baþa eklenir
        *ilkLog = yeniLog;
    } else {  // Eðer listede loglar varsa sona eklenir
        LogKaydi *gecici = *ilkLog;
        while (gecici->sonraki) {
            gecici = gecici->sonraki;
        }
        gecici->sonraki = yeniLog;
    }
}

// Syslog dosyasýný okuyup loglarý baðlý listeye ekleyen fonksiyon
void syslogOku(LogKaydi **ilkLog, const char *dosyaAdi) {
    FILE *dosya = fopen(dosyaAdi, "r"); // Dosyayý okuma modunda aç
    if (!dosya) { // Dosya açýlamazsa hata ver
        perror("Syslog dosyasý açýlamadý");
        exit(EXIT_FAILURE);
    }

    char satir[MAX_MESAJ_UZUNLUK]; // Satýrlarý tutacak deðiþken
    while (fgets(satir, sizeof(satir), dosya)) { // Satýr satýr oku
        char zaman[20] = "Tarih_Bilinmiyor";
        char seviye[10] = "Bilinmiyor";
        char mesaj[MAX_MESAJ_UZUNLUK];

        // Satýrý parçalara ayýrarak zamaný, seviyeyi ve mesajý al
        scanf(satir, "%19s %9s %1023[^"]", zaman, seviye, mesaj);
        
        // Loglarý listeye ekle
        logEkle(ilkLog, zaman, seviye, mesaj);
    }
    fclose(dosya); // Dosyayý kapat
}

// Baðlý listedeki loglarý ekrana yazdýran fonksiyon
void loglariGoster(LogKaydi *ilkLog) {
    while (ilkLog) { // Liste boyunca tüm loglarý ekrana yazdýr
        printf("[%s] (%s) %s\n", ilkLog->zaman, ilkLog->seviye, ilkLog->mesaj);
        ilkLog = ilkLog->sonraki;
    }
}

// Baðlý listeyi temizleyerek belleði serbest býrakan fonksiyon
void loglariTemizle(LogKaydi *ilkLog) {
    LogKaydi *gecici;
    while (ilkLog) {
        gecici = ilkLog;
        ilkLog = ilkLog->sonraki;
        free(gecici); // Bellekten kaldýr
    }
}

// Ana fonksiyon (programýn baþlangýç noktasý)
int main() {
    LogKaydi *logListesi = NULL; // Log listesi baþlangýçta boþ

    // Syslog dosyasýný oku ve baðlý listeye ekle
    syslogOku(&logListesi, "/var/log/syslog");

    // Kayýtlarý ekrana yazdýr
    loglariGoster(logListesi);

    // Belleði temizle
    loglariTemizle(logListesi);

    return 0;
}

