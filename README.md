# ETS2 CarPlay

Euro Truck Simulator 2'deki Scania Smart Dash orta ekranına CarPlay tarzı bir arayüz getirir. YouTube, YouTube Music, Google, Kick, radyo ve istediğin her linki kamyonun ekranında açarsın. Oyunun kendi haritası da bir dokunuşla geri gelir.

## 3.0 ile gelenler

- **60 fps:** kamyon ekranındaki görüntü saniyede 60 kareye çıktı, videolarda ses ile görüntü daha uyumlu (Ekran → Akıcılık).
- **Daha yakın harita:** Smart Dash haritası yaklaşık 1,7 kat yakınlaştırıldı, hangi yoldan gidileceği daha net.
- **Oyunla birlikte aç-kapa:** uygulama oyun açılınca kendiliğinden açılır, oyun kapanınca kendisi de kapanır.
- **Otomatik güncelleme:** yeni sürümler arka planda iner, ana ekranda "Güncelle" düğmesi çıkar.

## Kurulum

1. [Sürümler](../../releases/latest) sayfasından `ETS2-CarPlay-Kurulum.exe` dosyasını indir.
2. Oyun kapalıyken çalıştır. Uygulama kurulur; mod, ekran eklentisi ve F8 tuşu oyuna kendiliğinden eklenir.
3. Oyunu aç, mod yöneticisinden **CarPlay Scania Smart Dash** modunu etkinleştir.
4. Smart Dash'li bir Scania'da ekranı F8 ile aç. Uygulama oyunla birlikte kendiliğinden açılır.

Windows "bilgisayarınızı korudu" derse: **Ek bilgi → Yine de çalıştır**. Dosya imzasız olduğu için bu uyarı çıkar.

## Kullanım

- Sol raydaki düğmeler: ana menü, geri, ileri, yenile, harita ve uygulamalar.
- **Harita** oyunun kendi navigasyonunu gösterir; çalan müzik köşede küçük kartla devam eder.
- **Ekran** ayarlarında parlaklık, beyazları yumuşatma, akıcılık (fps) ve ekrana sığdırma var. Değişiklikler oyunda bir saniye içinde görünür.
- Kısayollar: `Alt+←` geri, `Alt+→` ileri, `Alt+Home` ana menü, `Ctrl+L` adres çubuğu.

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
lld-link /dll /noentry /nodefaultlib /machine:x64 /out:ets2_carplay.dll ets2_carplay.obj kernel32.lib user32.lib gdi32.lib d3d11.lib.lib
```

Derlenmiş DLL ve paketlenmiş mod `app/assets/` içinde durur; kurulum dosyası bunları içerir.

## Yeni sürüm çıkarmak

`app/package.json` içindeki `version`'ı artır, ardından `v2.5.1` gibi bir etiket gönder. GitHub Actions kurulum dosyasını derleyip sürüme koyar, kullanıcıların uygulaması kendini günceller.

Yapan: [@servetsayarexe](https://instagram.com/servetsayarexe)
