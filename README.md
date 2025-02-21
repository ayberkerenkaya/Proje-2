# Syslog Kayıt Okuyucu

Bu proje, bir sistemin `/var/log/syslog` dosyasını okuyarak log kayıtlarını bir bağlı liste yapısında saklayan ve ekrana yazdıran bir C programıdır.

## Özellikler
- **Syslog dosyasını okur** ve log kayıtlarını bir bağlı listeye ekler.
- **Bağlı liste yapısını kullanır**, loglar dinamik olarak saklanır.
- **Log seviyelerini ve mesajlarını ayıklar** ve ekrana yazdırır.
- **Bellek yönetimi içerir**, kullanılan bellek program sonunda serbest bırakılır.

## Fonksiyonlar

- **logEkle(LogKaydi **ilkLog, const char *zaman, const char *seviye, const char *mesaj)**
  - Yeni bir log kaydı oluşturur ve bağlı listeye ekler.

- **syslogOku(LogKaydi **ilkLog, const char *dosyaAdi)**
  - Verilen dosya adını kullanarak logları okur ve listeye ekler.

- **loglariGoster(LogKaydi *ilkLog)**
  - Bağlı listedeki log kayıtlarını ekrana yazdırır.

- **loglariTemizle(LogKaydi *ilkLog)**
  - Bellekte tutulan log kayıtlarını temizleyerek bellek sızıntısını önler.

## Gereksinimler

- Linux veya Unix tabanlı bir işletim sistemi
- C derleyicisi (GCC önerilir)
- `/var/log/syslog` dosyasına erişim izni

## Not
- Program yalnızca `/var/log/syslog` dosyasını okur. Farklı bir log dosyasıyla çalıştırmak için `syslogOku` fonksiyonundaki dosya adı değiştirilmelidir.
- Syslog formatı sistemden sisteme değişebilir. Gerekirse `sscanf` işlemi uyarlanmalıdır.

## Lisans
Bu proje açık kaynak olup herhangi bir lisans altında dağıtılmamaktadır.

