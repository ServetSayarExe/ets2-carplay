# ETS2 CarPlay

Euro Truck Simulator 2'deki Scania Smart Dash orta ekranına CarPlay tarzı bir arayüz getirir. YouTube, YouTube Music, Animecix, Google, Kick, radyo ve istediğin her linki kamyonun ekranında açarsın. Oyunun kendi haritası da bir dokunuşla geri gelir.

## 3.2 ile gelenler

- **Baştan tasarım:** ana ekran artık oyunun kendi haritası. Kontroller haritanın kenarlarında yüzer: arama sol üstte, uygulamalar sağda, çalan şarkı sol altta.
- **Sürüş modu:** oyun öndeyken kamyon ekranında yalnızca harita, saat ve çalan şarkı kalır. Alt+Tab ile uygulamaya girince bütün kontroller geri gelir.
- **PC penceresi pembe değil:** haritanın saydam alanı pencerede siyah görünür.
- **Daha akıcı:** uygulama öndeyken ekran yakalama yavaşlar, menüler takılmaz; oyuna dönünce tam hıza çıkar. Bekleme süreleri yeniden yazıldı, 60 fps gerçekten 60.
- **Video kenarları:** tam ekran videoda rastgele renkler giderildi, görüntü kenarları siyaha doğru yumuşar.
- **Animecix** eklendi.
- Kurulum durumu ayarlarda, sorun varsa ana ekranda küçük bir uyarı çıkar. Varsayılan kenar boşlukları 2/2/2/3.

## Kurulum

1. [Sürümler](../../releases/latest) sayfasından `ETS2-CarPlay-Kurulum.exe` dosyasını indir.
2. Oyun kapalıyken çalıştır. Uygulama kurulur; mod, ekran eklentisi ve F8 tuşu oyuna kendiliğinden eklenir.
3. Oyunu aç, mod yöneticisinden **CarPlay Scania Smart Dash** modunu etkinleştir.
4. Smart Dash'li bir Scania'da ekranı F8 ile aç. Uygulama oyunla birlikte kendiliğinden açılır.

Windows "bilgisayarınızı korudu" derse: **Ek bilgi → Yine de çalıştır**. Dosya imzasız olduğu için bu uyarı çıkar.

## Kullanım

- Sol raydaki düğmeler: harita (ana ekran), geri, ileri, yenile ve ayarlar. Tarayıcıdayken YouTube, Müzik ve Google da rayda durur.
- **Ana ekran** oyunun kendi navigasyonudur; çalan müzik sol altta devam eder.
- **CarPlay Ekranı** ayarlarında parlaklık, beyazları yumuşatma, akıcılık (fps) ve ekrana sığdırma var. Değişiklikler oyunda bir saniye içinde görünür.
- Kısayollar: `Alt+←` geri, `Alt+→` ileri, `Alt+Home` harita, `Ctrl+L` adres çubuğu.

Yeni sürüm çıkınca uygulama arka planda indirir, ana ekranda **Güncelle** düğmesi çıkar.

## Gereksinimler

- Windows 10/11, ETS2 1.62
- Smart Dash'li Scania (S/R 2024+)
- Oyunun DirectX 11 ile çalışması

## Klasörler

| Klasör | İçerik |
|---|---|
| `app/` | Electron uygulaması (arayüz, otomatik kurulum, güncelleme) |
| `plugin/` | Oyuna görüntü aktaran telemetri eklentisi (`ets2_carplay.dll`, C, CRT'siz) |
| `mod/` | Smart Dash arayüz modu (`.scs` olarak paketlenir) |

Eklentiyi derlemek (clang + lld):

```
clang -target x86_64-pc-windows-msvc -O2 -ffreestanding -fno-builtin -fno-stack-protector -mno-stack-arg-probe -fshort-wchar -c ets2_carplay.c -o ets2_carplay.obj
for d in kernel32 user32 gdi32 d3d11; do llvm-dlltool -m i386:x86-64 -d $d.def -l $d.lib; done
lld-link /dll /noentry /nodefaultlib /machine:x64 /out:ets2_carplay.dll ets2_carplay.obj kernel32.lib user32.lib gdi32.lib d3d11.lib
```

Derlenmiş DLL ve paketlenmiş mod `app/assets/` içinde durur; kurulum dosyası bunları içerir. Eklenti ya da mod değişince ikisini de yeniden üretip `app/assets/` içine koy: GitHub Actions yalnızca Electron uygulamasını derler.

## Yeni sürüm çıkarmak

`app/package.json` içindeki `version`'ı (ve `mod/manifest.sii` içindeki `package_version`'ı) artır, ardından Actions sekmesinden **Sürüm derle ve yayınla** iş akışını çalıştır. Etiketi kendisi oluşturur. Etiketi elle göndereceksen `package.json` ile aynı sürümde olmalı (ör. `v3.1.6`), yoksa iş akışı durur. Kurulum dosyası derlenip sürüme konur, kullanıcıların uygulaması kendini günceller.

Yapan: [@servetsayarexe](https://instagram.com/servetsayarexe)
