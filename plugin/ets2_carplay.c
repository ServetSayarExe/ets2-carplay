/*
 * ETS2 CarPlay - Smart Dash ekran eklentisi
 * by servetsayarexe
 *
 * Ne yapar:
 *  - Oyunun D3D11 Present ve CreateTexture2D fonksiyonlarina (vtable) baglanir.
 *  - Modun stream dokusunu (1440x810, sol ust pikseli 33 22 11 FF) taniyip
 *    guncellenebilir hale getirir.
 *  - Ayri bir is parcaciginda "ETS2 CarPlay" penceresinin goruntusunu alir,
 *    her karede o dokuya yazar.
 *  - Saf macenta (FF00FF) pikseller seffaf olur -> altta oyunun haritasi gorunur.
 *  - Sol ust piksel rgb(P, 0x5A, 0xA5) ise P/255 parlaklik olarak uygulanir.
 *
 * Menu yok, ayar yok. SDK, CRT ya da harici kutuphane kullanmaz.
 */

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef int                i32;
typedef unsigned long long u64;
typedef long long          i64;
typedef long               HRESULT;
typedef int                BOOL;
typedef void*              HANDLE;
typedef void*              HWND;
typedef void*              HDC;
typedef void*              HGDIOBJ;
typedef void*              HBITMAP;
typedef unsigned short     wchar;

#define WINAPI __stdcall
#define NULL ((void*)0)

#define TEX_W 1440
#define TEX_H 810

/* ---------------- Win32 ---------------- */
typedef struct { i32 left, top, right, bottom; } RECT;
typedef struct { u8 b[40]; } CRITICAL_SECTION; /* x64: 40 bytes */
typedef struct { u16 wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds; } SYSTEMTIME;
typedef struct {
    u32 biSize; i32 biWidth; i32 biHeight; u16 biPlanes; u16 biBitCount;
    u32 biCompression; u32 biSizeImage; i32 biXPelsPerMeter; i32 biYPelsPerMeter;
    u32 biClrUsed; u32 biClrImportant;
} BITMAPINFOHEADER;
typedef struct { BITMAPINFOHEADER bmiHeader; u32 bmiColors[1]; } BITMAPINFO;

__declspec(dllimport) HANDLE WINAPI CreateThread(void*, u64, u32 (WINAPI*)(void*), void*, u32, u32*);
__declspec(dllimport) void   WINAPI Sleep(u32);
__declspec(dllimport) void   WINAPI InitializeCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) void   WINAPI EnterCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) void   WINAPI LeaveCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) void   WINAPI DeleteCriticalSection(CRITICAL_SECTION*);
__declspec(dllimport) BOOL   WINAPI VirtualProtect(void*, u64, u32, u32*);
__declspec(dllimport) void*  WINAPI VirtualAlloc(void*, u64, u32, u32);
__declspec(dllimport) BOOL   WINAPI VirtualFree(void*, u64, u32);
__declspec(dllimport) u32    WINAPI WaitForSingleObject(HANDLE, u32);
__declspec(dllimport) BOOL   WINAPI CloseHandle(HANDLE);
__declspec(dllimport) HANDLE WINAPI CreateFileA(const char*, u32, u32, void*, u32, u32, HANDLE);
__declspec(dllimport) BOOL   WINAPI WriteFile(HANDLE, const void*, u32, u32*, void*);
__declspec(dllimport) BOOL   WINAPI ReadFile(HANDLE, void*, u32, u32*, void*);
__declspec(dllimport) u32    WINAPI GetEnvironmentVariableW(const wchar*, wchar*, u32);
__declspec(dllimport) u32    WINAPI GetFileAttributesW(const wchar*);
__declspec(dllimport) u32    WINAPI GetEnvironmentVariableA(const char*, char*, u32);
__declspec(dllimport) u64    WINAPI GetTickCount64(void);
__declspec(dllimport) void   WINAPI GetLocalTime(SYSTEMTIME*);
__declspec(dllimport) u32    WINAPI SetFilePointer(HANDLE, i32, i32*, u32);

__declspec(dllimport) HWND WINAPI CreateWindowExW(u32, const wchar*, const wchar*, u32, i32, i32, i32, i32, HWND, void*, void*, void*);
__declspec(dllimport) BOOL WINAPI DestroyWindow(HWND);
__declspec(dllimport) HWND WINAPI FindWindowW(const wchar*, const wchar*);
__declspec(dllimport) BOOL WINAPI IsWindowVisible(HWND);
__declspec(dllimport) BOOL WINAPI IsIconic(HWND);
__declspec(dllimport) BOOL WINAPI GetClientRect(HWND, RECT*);
__declspec(dllimport) BOOL WINAPI PrintWindow(HWND, HDC, u32);
__declspec(dllimport) HDC  WINAPI GetDC(HWND);
__declspec(dllimport) i32  WINAPI ReleaseDC(HWND, HDC);

__declspec(dllimport) HDC     WINAPI CreateCompatibleDC(HDC);
__declspec(dllimport) HBITMAP WINAPI CreateDIBSection(HDC, const BITMAPINFO*, u32, void**, HANDLE, u32);
__declspec(dllimport) HGDIOBJ WINAPI SelectObject(HDC, HGDIOBJ);
__declspec(dllimport) BOOL    WINAPI DeleteObject(HGDIOBJ);
__declspec(dllimport) BOOL    WINAPI DeleteDC(HDC);
__declspec(dllimport) BOOL    WINAPI StretchBlt(HDC, i32, i32, i32, i32, HDC, i32, i32, i32, i32, u32);
__declspec(dllimport) i32     WINAPI SetStretchBltMode(HDC, i32);
__declspec(dllimport) BOOL    WINAPI GdiFlush(void);

/* ---------------- D3D11 / DXGI (minimal) ---------------- */
typedef struct { u32 Data1; u16 Data2, Data3; u8 Data4[8]; } GUID;
typedef struct { u32 Numerator, Denominator; } DXGI_RATIONAL;
typedef struct { u32 Width, Height; DXGI_RATIONAL RefreshRate; u32 Format, ScanlineOrdering, Scaling; } DXGI_MODE_DESC;
typedef struct { u32 Count, Quality; } DXGI_SAMPLE_DESC;
typedef struct {
    DXGI_MODE_DESC BufferDesc; DXGI_SAMPLE_DESC SampleDesc; u32 BufferUsage; u32 BufferCount;
    HWND OutputWindow; BOOL Windowed; u32 SwapEffect; u32 Flags;
} DXGI_SWAP_CHAIN_DESC;
typedef struct {
    u32 Width, Height, MipLevels, ArraySize, Format; DXGI_SAMPLE_DESC SampleDesc;
    u32 Usage, BindFlags, CPUAccessFlags, MiscFlags;
} D3D11_TEXTURE2D_DESC;
typedef struct { const void* pSysMem; u32 SysMemPitch; u32 SysMemSlicePitch; } D3D11_SUBRESOURCE_DATA;

typedef struct IUnk { void** vt; } IUnk; /* any COM object: first field = vtable */

__declspec(dllimport) HRESULT WINAPI D3D11CreateDeviceAndSwapChain(void*, u32, void*, u32, const u32*, u32, u32,
    const DXGI_SWAP_CHAIN_DESC*, IUnk**, IUnk**, u32*, IUnk**);

static const GUID IID_ID3D11Device = { 0xdb6f6ddb, 0xac77, 0x4e88, { 0x82, 0x53, 0x81, 0x9d, 0xf9, 0xbb, 0xf1, 0x40 } };

#define VT_RELEASE          2
#define VT_SC_GETDEVICE     7
#define VT_SC_PRESENT       8
#define VT_SC_PRESENT1      22
#define VT_DEV_CREATETEX2D  5
#define VT_DEV_GETCTX       40
#define VT_CTX_UPDATESUB    48

typedef HRESULT (*PFN_Present)(IUnk*, u32, u32);
typedef HRESULT (*PFN_Present1)(IUnk*, u32, u32, const void*);
typedef HRESULT (*PFN_CreateTex2D)(IUnk*, const D3D11_TEXTURE2D_DESC*, const D3D11_SUBRESOURCE_DATA*, IUnk**);
typedef u32     (*PFN_Release)(IUnk*);
typedef u32     (*PFN_AddRef)(IUnk*);
typedef HRESULT (*PFN_QI)(IUnk*, const GUID*, void**);
typedef void    (*PFN_GetCtx)(IUnk*, IUnk**);
typedef void    (*PFN_UpdateSub)(IUnk*, IUnk*, u32, const void*, const void*, u32, u32);

/* ---------------- SCS SDK (minimal) ---------------- */
typedef void (*scs_log_t)(i32 type, const char* msg);
typedef struct {
    const char* game_name; const char* game_id; u32 game_version; u32 _pad; scs_log_t log;
} scs_sdk_init_params_v100;

/* ---------------- CRT replacements ---------------- */
void* memset(void* d, int c, u64 n) { u8* p = (u8*)d; while (n--) *p++ = (u8)c; return d; }
void* memcpy(void* d, const void* s, u64 n) { u8* a = (u8*)d; const u8* b = (const u8*)s; while (n--) *a++ = *b++; return d; }
int _fltused = 0;

/* ---------------- state ---------------- */
static scs_log_t      g_scslog;
static HANDLE         g_logf;
static void**         g_scVt;            /* swapchain vtable */
static void**         g_devVt;           /* device vtable */
static PFN_Present     o_Present;
static PFN_Present1    o_Present1;
static PFN_CreateTex2D o_CreateTex2D;
static IUnk*          g_ctx;             /* immediate context */
static IUnk* volatile g_tex;             /* our stream texture (we hold a ref) */
static volatile i32   g_texRGBA;         /* 1 = R8G8B8A8 order, 0 = B8G8R8A8 */
static CRITICAL_SECTION g_cs;
static u8*            g_front;           /* TEX_W*TEX_H*4, ready for upload */
static u8*            g_back;
static volatile u32   g_seq, g_uploaded;
static volatile i32   g_run;
static HANDLE         g_thread;
static volatile i32   g_presentSeen, g_texFound;
static u32            g_dim = 256; /* varsayilan %100 */
static u8             g_lut[256];
static u32            g_lutDim = 0, g_lutWhite = 999;
static volatile i32   g_white = 85;  /* beyazlari yumusat 0..100 */
static volatile i32   g_fps = 30;    /* 10..30 */
/* parlak tonlari bastir: diz noktasina kadar dogrusal, sonrasi azaltilmis egimle; sonra parlaklik */
static void build_lut(u32 dim, i32 white) {
    i32 knee = 230 - white * 11 / 10;          /* 230 .. 120 */
    i32 slope = 1000 - white * 7;              /* binde: 1000 .. 300 */
    for (i32 x = 0; x < 256; x++) {
        i32 y = x <= knee ? x : knee + (x - knee) * slope / 1000;
        g_lut[x] = (u8)(((u32)y * dim) >> 8);
    }
    g_lutDim = dim; g_lutWhite = (u32)white;
}
static char           g_cfgPath[512];

/* Uygulama parlakligi Belgeler\Euro Truck Simulator 2\ets2_carplay.cfg dosyasina yazar (10-100) */
static void cfg_init(void) {
    u32 n = GetEnvironmentVariableA("USERPROFILE", g_cfgPath, 400);
    if (!n || n > 400) { g_cfgPath[0] = 0; return; }
    const char* tail = "\\Documents\\Euro Truck Simulator 2\\ets2_carplay.cfg";
    int i = 0; while (tail[i]) { g_cfgPath[n + i] = tail[i]; i++; } g_cfgPath[n + i] = 0;
}
static void lg(const char* msg);
static volatile i32 g_inset[4] = { 2, 2, 0, 4 }; /* sol, ust, sag, alt (yuzde) */
static int kv_match(const char* b, u32 i, u32 r, const char* k) {
    u32 j = 0; while (k[j]) { if (i + j >= r || b[i + j] != k[j]) return 0; j++; } return (int)j;
}
static int cfg_read(int* dim) {
    if (!g_cfgPath[0]) return 0;
    HANDLE f = CreateFileA(g_cfgPath, 0x80000000 /*GENERIC_READ*/, 7, NULL, 3 /*OPEN_EXISTING*/, 0x80, NULL);
    if (f == (HANDLE)(i64)-1) return 0;
    char b[256]; u32 r = 0; ReadFile(f, b, 255, &r, NULL); CloseHandle(f);
    static const char* keys[7] = { "parlaklik=", "sol=", "ust=", "sag=", "alt=", "beyaz=", "fps=" };
    int vals[7] = { -1, -1, -1, -1, -1, -1, -1 };
    u32 i = 0;
    while (i < r) {
        int which = -1, kl = 0;
        for (int k = 0; k < 7; k++) { kl = kv_match(b, i, r, keys[k]); if (kl) { which = k; break; } }
        if (which < 0 && i == 0) { which = 0; kl = 0; } /* eski bicim: sadece sayi */
        if (which >= 0) {
            i += (u32)kl; int v = 0, any = 0;
            while (i < r && b[i] >= '0' && b[i] <= '9') { v = v * 10 + (b[i] - '0'); any = 1; i++; }
            if (any) vals[which] = v;
        }
        while (i < r && b[i] != '\n') i++;
        i++;
    }
    if (vals[0] >= 0) { int v = vals[0]; if (v < 5) v = 5; if (v > 100) v = 100; *dim = v; }
    for (int k = 1; k < 5; k++) if (vals[k] >= 0) { int v = vals[k]; if (v > 30) v = 30; g_inset[k - 1] = v; }
    if (vals[5] >= 0) g_white = vals[5] > 100 ? 100 : vals[5];
    if (vals[6] >= 0) g_fps = vals[6] < 10 ? 10 : (vals[6] > 30 ? 30 : vals[6]);
    return 1;
}

/* Oyun acilinca uygulamayi arka planda baslat.
   ShellExecute yerine CreateProcess: ShellExecute COM ister, oyun icindeki is parcaciginda takiliyordu. */
typedef struct { u32 cb; wchar *r, *desk, *title; u32 x, y, xs, ys, xc, yc, fill, flags; u16 show, cbr2; u8* r2; HANDLE in, out, err; } STARTUPINFOW_;
typedef struct { HANDLE proc, thr; u32 pid, tid; } PROCINFO_;
__declspec(dllimport) BOOL WINAPI CreateProcessW(const wchar*, wchar*, void*, void*, BOOL, u32, void*, const wchar*, STARTUPINFOW_*, PROCINFO_*);
static void launch_app(void) {
    static const wchar title[] = { 'E','T','S','2',' ','C','a','r','P','l','a','y',0 };
    if (FindWindowW(NULL, title)) return; /* zaten acik */
    wchar p[600]; u32 n = GetEnvironmentVariableW((const wchar*)L"LOCALAPPDATA", p, 400);
    if (!n || n > 400) return;
    const wchar* tail = (const wchar*)L"\\Programs\\ets2-carplay\\ETS2 CarPlay.exe";
    int i = 0; while (tail[i]) { p[n + i] = tail[i]; i++; } p[n + i] = 0;
    if (GetFileAttributesW(p) == 0xFFFFFFFFu) { lg("uygulama kurulu degil, otomatik baslatilamadi"); return; }
    wchar cmd[700]; int k = 0; cmd[k++] = '"'; for (i = 0; p[i]; i++) cmd[k++] = p[i]; cmd[k++] = '"';
    const wchar* arg = (const wchar*)L" --oyundan"; for (i = 0; arg[i]; i++) cmd[k++] = arg[i]; cmd[k] = 0;
    STARTUPINFOW_ si; memset(&si, 0, sizeof si); si.cb = sizeof si; si.flags = 1 /*USESHOWWINDOW*/; si.show = 4 /*SHOWNOACTIVATE*/;
    PROCINFO_ pi; memset(&pi, 0, sizeof pi);
    if (CreateProcessW(p, cmd, NULL, NULL, 0, 0x00000008 /*DETACHED_PROCESS*/ | 0x00000200 /*NEW_PROCESS_GROUP*/ | 0x01000000 /*BREAKAWAY_FROM_JOB*/, NULL, NULL, &si, &pi)
        || CreateProcessW(p, cmd, NULL, NULL, 0, 0x00000008 | 0x00000200, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.thr); CloseHandle(pi.proc); lg("uygulama otomatik baslatildi");
    } else lg("uygulama baslatilamadi (CreateProcess)");
}

static int s_len(const char* s) { int n = 0; while (s[n]) n++; return n; }
static void log_open(void) {
    char path[512]; u32 n = GetEnvironmentVariableA("USERPROFILE", path, 400);
    if (!n || n > 400) return;
    const char* tail = "\\Documents\\Euro Truck Simulator 2\\ets2_carplay.log";
    int i = 0; while (tail[i]) { path[n + i] = tail[i]; i++; } path[n + i] = 0;
    g_logf = CreateFileA(path, 0x40000000 /*GENERIC_WRITE*/, 1 /*SHARE_READ*/, NULL, 2 /*CREATE_ALWAYS*/, 0x80, NULL);
    if (g_logf == (HANDLE)(i64)-1) g_logf = NULL;
}
static void lg(const char* msg) {
    if (g_logf) {
        SYSTEMTIME t; GetLocalTime(&t);
        char ts[16]; int v[3] = { t.wHour, t.wMinute, t.wSecond };
        for (int i = 0; i < 3; i++) { ts[i * 3] = '0' + v[i] / 10; ts[i * 3 + 1] = '0' + v[i] % 10; ts[i * 3 + 2] = i < 2 ? ':' : ' '; }
        u32 w; WriteFile(g_logf, ts, 9, &w, NULL); WriteFile(g_logf, msg, (u32)s_len(msg), &w, NULL); WriteFile(g_logf, "\r\n", 2, &w, NULL);
    }
}
static void lghex(const char* pre, u64 v) {
    char b[96]; int n = 0; while (pre[n] && n < 70) { b[n] = pre[n]; n++; }
    b[n++] = '0'; b[n++] = 'x';
    for (int i = 15; i >= 0; i--) { int d = (int)((v >> (i * 4)) & 15); b[n++] = (char)(d < 10 ? '0' + d : 'a' + d - 10); }
    b[n] = 0; lg(b);
}

static int patch_vt(void** slot, void* fn, void** orig) {
    u32 old;
    if (!VirtualProtect(slot, 8, 0x04 /*PAGE_READWRITE*/, &old)) return 0;
    if (orig) *orig = *slot;
    *slot = fn;
    VirtualProtect(slot, 8, old, &old);
    return 1;
}

/* ---------------- texture hook ---------------- */
static volatile i32 g_texLogs;
static void log_tex(const char* pre, const D3D11_TEXTURE2D_DESC* d, const D3D11_SUBRESOURCE_DATA* init) {
    if (g_texLogs > 40) return; g_texLogs++;
    char b[160]; int n = 0; const char* p = pre; while (*p) b[n++] = *p++;
    u32 v[8] = { d->Width, d->Height, d->Format, d->MipLevels, d->Usage, d->BindFlags, d->MiscFlags, (u32)(init && init->pSysMem ? 1 : 0) };
    const char* k[8] = { " w=", " h=", " fmt=", " mips=", " usage=", " bind=", " misc=", " data=" };
    for (int i = 0; i < 8; i++) {
        const char* q = k[i]; while (*q) b[n++] = *q++;
        char t[12]; int m = 0; u32 x = v[i]; do { t[m++] = (char)('0' + x % 10); x /= 10; } while (x);
        while (m) b[n++] = t[--m];
    }
    b[n] = 0; lg(b);
}
static int is_our_texture(const D3D11_TEXTURE2D_DESC* d, const D3D11_SUBRESOURCE_DATA* init, int* rgba) {
    if (!d) return 0;
    if (d->Width != TEX_W || d->Height != TEX_H) return 0;
    log_tex("1440x810 doku:", d, init);
    if (d->ArraySize != 1 || d->SampleDesc.Count > 1) return 0;
    if (d->BindFlags & ~0x8u) return 0;          /* sadece shader resource (render target degil) */
    u32 f = d->Format;
    /* 87 B8G8R8A8_UNORM, 91 *_SRGB, 88 B8G8R8X8, 28 R8G8B8A8_UNORM, 29 *_SRGB */
    if (f == 87 || f == 91 || f == 88) *rgba = 0;
    else if (f == 28 || f == 29) *rgba = 1;
    else return 0;
    if (init && init->pSysMem) {
        const u8* p = (const u8*)init->pSysMem;
        if (*rgba) return p[0] == 0x33 && p[1] == 0x22 && p[2] == 0x11;
        return p[0] == 0x11 && p[1] == 0x22 && p[2] == 0x33;
    }
    return 1; /* veri sonradan yukleniyor: boyut+format yeterli */
}

static HRESULT h_CreateTex2D(IUnk* dev, const D3D11_TEXTURE2D_DESC* desc, const D3D11_SUBRESOURCE_DATA* init, IUnk** out) {
    int rgba = 0;
    if (out && is_our_texture(desc, init, &rgba)) {
        D3D11_TEXTURE2D_DESC d = *desc;
        d.Usage = 0;            /* D3D11_USAGE_DEFAULT (oyun IMMUTABLE ister) */
        d.CPUAccessFlags = 0;
        d.MipLevels = 1;
        HRESULT hr = o_CreateTex2D(dev, &d, init, out);
        lghex("bizim doku olusturuldu hr=", (u64)(u32)hr);
        if (hr >= 0 && *out) {
            ((PFN_AddRef)(*out)->vt[1])(*out);
            IUnk* old = (IUnk*)__atomic_exchange_n((void**)&g_tex, (void*)*out, __ATOMIC_SEQ_CST);
            g_texRGBA = rgba;
            g_uploaded = g_seq - 1; /* yeniden yukle */
            g_texFound = 1;
            if (old) ((PFN_Release)old->vt[VT_RELEASE])(old);
        }
        return hr;
    }
    return o_CreateTex2D(dev, desc, init, out);
}

/* ---------------- present hook ---------------- */
static void on_present(IUnk* sc) {
    if (!g_devVt) {
        IUnk* dev = NULL;
        if (((PFN_QI)sc->vt[VT_SC_GETDEVICE])(sc, &IID_ID3D11Device, (void**)&dev) >= 0 && dev) {
            ((PFN_GetCtx)dev->vt[VT_DEV_GETCTX])(dev, &g_ctx);
            g_devVt = dev->vt;
            int okp = patch_vt(&g_devVt[VT_DEV_CREATETEX2D], (void*)h_CreateTex2D, (void**)&o_CreateTex2D);
            lg(okp ? "ilk Present: CreateTexture2D kancasi kuruldu" : "ilk Present: CreateTexture2D kancasi KURULAMADI");
            ((PFN_Release)dev->vt[VT_RELEASE])(dev);
            g_presentSeen = 1;
        } else lg("ilk Present: cihaz alinamadi");
        return;
    }
    if (g_texFound == 1) { g_texFound = 2; lg("doku bulundu, goruntu basiliyor"); }
    IUnk* tex = g_tex;
    if (tex && g_ctx && g_seq != g_uploaded) {
        EnterCriticalSection(&g_cs);
        u32 s = g_seq;
        ((PFN_UpdateSub)g_ctx->vt[VT_CTX_UPDATESUB])(g_ctx, tex, 0, NULL, g_front, TEX_W * 4, 0);
        g_uploaded = s;
        LeaveCriticalSection(&g_cs);
    }
}
static volatile i32 g_p0, g_p1;
static HRESULT h_Present(IUnk* sc, u32 sync, u32 flags) { if (!g_p0) { g_p0 = 1; lg("Present cagrildi"); } on_present(sc); return o_Present(sc, sync, flags); }
static HRESULT h_Present1(IUnk* sc, u32 sync, u32 flags, const void* p) { if (!g_p1) { g_p1 = 1; lg("Present1 cagrildi"); } on_present(sc); return o_Present1(sc, sync, flags, p); }

static int install_present_hook(void) {
    static const wchar cls[] = { 'S','T','A','T','I','C',0 };
    HWND w = CreateWindowExW(0, cls, cls, 0, 0, 0, 64, 64, NULL, NULL, NULL, NULL);
    if (!w) { lg("dummy window failed"); return 0; }
    DXGI_SWAP_CHAIN_DESC sd; memset(&sd, 0, sizeof sd);
    sd.BufferDesc.Width = 64; sd.BufferDesc.Height = 64; sd.BufferDesc.Format = 28; /* R8G8B8A8_UNORM */
    sd.SampleDesc.Count = 1; sd.BufferUsage = 0x20; /* RENDER_TARGET_OUTPUT */
    sd.BufferCount = 1; sd.OutputWindow = w; sd.Windowed = 1; sd.SwapEffect = 0; /* DISCARD */
    IUnk *sc = NULL, *dev = NULL, *ctx = NULL; u32 fl = 0;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, 1 /*HARDWARE*/, NULL, 0, NULL, 0, 7, &sd, &sc, &dev, &fl, &ctx);
    if (hr < 0 || !sc) {
        hr = D3D11CreateDeviceAndSwapChain(NULL, 5 /*WARP*/, NULL, 0, NULL, 0, 7, &sd, &sc, &dev, &fl, &ctx);
    }
    int ok = 0;
    if (hr >= 0 && sc) {
        g_scVt = sc->vt;
        ok = patch_vt(&g_scVt[VT_SC_PRESENT], (void*)h_Present, (void**)&o_Present);
        patch_vt(&g_scVt[VT_SC_PRESENT1], (void*)h_Present1, (void**)&o_Present1);
    } else lghex("D3D11CreateDeviceAndSwapChain failed hr=", (u64)(u32)hr);
    if (ctx) ((PFN_Release)ctx->vt[VT_RELEASE])(ctx);
    if (dev) ((PFN_Release)dev->vt[VT_RELEASE])(dev);
    if (sc)  ((PFN_Release)sc->vt[VT_RELEASE])(sc);
    DestroyWindow(w);
    return ok;
}

/* ---------------- capture thread ---------------- */
static void fill_transparent(u8* buf) { memset(buf, 0, (u64)TEX_W * TEX_H * 4); }

static void publish(void) {
    EnterCriticalSection(&g_cs);
    u8* t = g_front; g_front = g_back; g_back = t;
    g_seq++;
    LeaveCriticalSection(&g_cs);
}

static u32 WINAPI capture_thread(void* arg) {
    (void)arg;
    static const wchar title[] = { 'E','T','S','2',' ','C','a','r','P','l','a','y',0 };
    HDC capDC = CreateCompatibleDC(NULL), dstDC = CreateCompatibleDC(NULL);
    HBITMAP capBmp = NULL, dstBmp = NULL; u8 *capBits = NULL, *dstBits = NULL;
    i32 capW = 0, capH = 0;
    BITMAPINFO bi; memset(&bi, 0, sizeof bi);
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biWidth = TEX_W; bi.bmiHeader.biHeight = -TEX_H;
    dstBmp = CreateDIBSection(NULL, &bi, 0, (void**)&dstBits, NULL, 0);
    if (dstBmp) SelectObject(dstDC, dstBmp);
    SetStretchBltMode(dstDC, 4 /*HALFTONE*/);
    int missing = 0, blankSent = 0, lastState = -1;
    u64 tStart = GetTickCount64(), tCfg = 0; int warned = 0, launched = 0;

    while (g_run) {
        u64 t0 = GetTickCount64();
        if (!warned && t0 - tStart > 20000) { warned = 1; lg(g_presentSeen ? "durum: Present tamam" : "durum: 20 sn'dir Present hic cagrilmadi (kanca tutmadi)"); lg(g_tex ? "durum: doku var" : "durum: doku henuz yok"); }
        HWND h = FindWindowW(NULL, title);
        int usable = h && IsWindowVisible(h) && !IsIconic(h);
        RECT rc = { 0, 0, 0, 0 };
        if (usable) { GetClientRect(h, &rc); if (rc.right <= 0 || rc.bottom <= 0) usable = 0; }
        if (usable != lastState) { lg(usable ? "CarPlay penceresi bulundu" : "CarPlay penceresi bekleniyor"); lastState = usable; }

        if (!usable && !launched && g_presentSeen && t0 - tStart > 4000) { launched = 1; launch_app(); }
        if (!usable) {
            /* uygulama kapaliysa harita gorunsun: tamamen seffaf kare gonder */
            if (++missing > 3 && !blankSent && g_tex) { fill_transparent(g_back); publish(); blankSent = 1; }
            Sleep(400);
            continue;
        }
        missing = 0; blankSent = 0;
        if (!g_tex) { Sleep(500); continue; } /* Smart Dash ekrani yuklu degil: yakalama yapma */

        if (rc.right != capW || rc.bottom != capH || !capBmp) {
            if (capBmp) DeleteObject(capBmp);
            bi.bmiHeader.biWidth = rc.right; bi.bmiHeader.biHeight = -rc.bottom;
            capBmp = CreateDIBSection(NULL, &bi, 0, (void**)&capBits, NULL, 0);
            if (!capBmp) { Sleep(500); continue; }
            SelectObject(capDC, capBmp);
            capW = rc.right; capH = rc.bottom;
        }
        if (!PrintWindow(h, capDC, 3 /*PW_CLIENTONLY|PW_RENDERFULLCONTENT*/)) { Sleep(100); continue; }
        const u8* src;
        i32 il = g_inset[0] * TEX_W / 100, it = g_inset[1] * TEX_H / 100, ir = g_inset[2] * TEX_W / 100, ib = g_inset[3] * TEX_H / 100;
        if (capW == TEX_W && capH == TEX_H && !il && !it && !ir && !ib) { GdiFlush(); src = capBits; }
        else {
            if (dstBits) memset(dstBits, 0, (u64)TEX_W * TEX_H * 4); /* kenarlar siyah */
            StretchBlt(dstDC, il, it, TEX_W - il - ir, TEX_H - it - ib, capDC, 0, 0, capW, capH, 0x00CC0020 /*SRCCOPY*/);
            GdiFlush(); src = dstBits;
        }
        if (!src || !g_back) { Sleep(100); continue; }

        /* sol ust piksel: rgb(P, 0x5A, 0xA5) -> parlaklik P */
        /* parlaklik: ayar dosyasindan (saniyede bir) */
        if (t0 - tCfg > 1000) {
            tCfg = t0;
            int v = -1; i32 oi[4] = { g_inset[0], g_inset[1], g_inset[2], g_inset[3] };
            cfg_read(&v);
            if (v > 0) {
                u32 nd = (u32)(v * 256 / 100);
                if (nd != g_dim) { g_dim = nd; lghex("parlaklik ayari (yuzde) = ", (u64)v); }
            }
            if (oi[0] != g_inset[0] || oi[1] != g_inset[1] || oi[2] != g_inset[2] || oi[3] != g_inset[3])
                lghex("kenar bosluklari (sol/ust/sag/alt) = ", ((u64)g_inset[0] << 24) | ((u64)g_inset[1] << 16) | ((u64)g_inset[2] << 8) | (u64)g_inset[3]);
        }
        u32 dim = g_dim;
        if (dim != g_lutDim || (u32)g_white != g_lutWhite) build_lut(dim, g_white);
        int rgba = g_texRGBA;
        const u8* s = src; u8* d = g_back; u32 n = TEX_W * TEX_H;
        for (u32 i = 0; i < n; i++, s += 4, d += 4) {
            u8 b = s[0], g = s[1], r = s[2];
            if (r >= 250 && g <= 6 && b >= 250) { d[0] = d[1] = d[2] = d[3] = 0; continue; }
            r = g_lut[r]; g = g_lut[g]; b = g_lut[b];
            if (rgba) { d[0] = r; d[1] = g; d[2] = b; } else { d[0] = b; d[1] = g; d[2] = r; }
            d[3] = 255;
        }
        /* isaret pikselini komsusuyla doldur */
        g_back[0] = g_back[4]; g_back[1] = g_back[5]; g_back[2] = g_back[6]; g_back[3] = g_back[7];
        publish();

        u64 dt = GetTickCount64() - t0;
        u32 frame = 1000u / (u32)g_fps;
        if (dt < frame) Sleep((u32)(frame - dt));
    }
    if (capBmp) DeleteObject(capBmp);
    if (dstBmp) DeleteObject(dstBmp);
    DeleteDC(capDC); DeleteDC(dstDC);
    return 0;
}

/* ---------------- SCS telemetry entry points ---------------- */
__declspec(dllexport) i32 scs_telemetry_init(u32 version, const void* params) {
    (void)version;
    const scs_sdk_init_params_v100* p = (const scs_sdk_init_params_v100*)params;
    if (p) g_scslog = p->log;
    log_open();
    cfg_init();
    lg("ETS2 CarPlay eklentisi basliyor (by servetsayarexe)");
    launch_app(); /* oyunla birlikte hemen ac */
    if (g_scslog) g_scslog(0, "[ETS2 CarPlay] eklenti basliyor");

    InitializeCriticalSection(&g_cs);
    u64 sz = (u64)TEX_W * TEX_H * 4;
    g_front = (u8*)VirtualAlloc(NULL, sz, 0x3000, 0x04);
    g_back  = (u8*)VirtualAlloc(NULL, sz, 0x3000, 0x04);
    if (!g_front || !g_back) { lg("bellek ayrilamadi"); return 0; }
    fill_transparent(g_front); fill_transparent(g_back);
    g_seq = 1; g_uploaded = 1;

    if (install_present_hook()) {
        lg("Present kancasi kuruldu");
        if (g_scslog) g_scslog(0, "[ETS2 CarPlay] hazir");
    } else {
        lg("Present kancasi KURULAMADI");
        if (g_scslog) g_scslog(2, "[ETS2 CarPlay] D3D11 kancasi kurulamadi");
        return 0; /* oyunu bozma, sessizce devre disi */
    }
    g_run = 1;
    g_thread = CreateThread(NULL, 0, capture_thread, NULL, 0, NULL);
    return 0; /* SCS_RESULT_ok */
}

__declspec(dllexport) void scs_telemetry_shutdown(void) {
    g_run = 0;
    if (g_thread) { WaitForSingleObject(g_thread, 2000); CloseHandle(g_thread); g_thread = NULL; }
    if (g_scVt && o_Present)  patch_vt(&g_scVt[VT_SC_PRESENT], (void*)o_Present, NULL);
    if (g_scVt && o_Present1) patch_vt(&g_scVt[VT_SC_PRESENT1], (void*)o_Present1, NULL);
    if (g_devVt && o_CreateTex2D) patch_vt(&g_devVt[VT_DEV_CREATETEX2D], (void*)o_CreateTex2D, NULL);
    IUnk* t = (IUnk*)__atomic_exchange_n((void**)&g_tex, NULL, __ATOMIC_SEQ_CST);
    if (t) ((PFN_Release)t->vt[VT_RELEASE])(t);
    if (g_ctx) { ((PFN_Release)g_ctx->vt[VT_RELEASE])(g_ctx); g_ctx = NULL; }
    lg("eklenti kapatildi");
    if (g_logf) { CloseHandle(g_logf); g_logf = NULL; }
}
