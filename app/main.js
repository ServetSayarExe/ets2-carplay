const { app, BrowserWindow, session, ipcMain, shell, nativeTheme } = require('electron');
const { runSetup } = require('./setup');
const path = require('path');

app.setName('ETS2 CarPlay');
// Oyun penceresinin arkasinda kalinca da cizmeye devam et
app.commandLine.appendSwitch('disable-features', 'CalculateNativeWinOcclusion');
app.commandLine.appendSwitch('disable-renderer-backgrounding');
app.commandLine.appendSwitch('disable-background-timer-throttling');
app.commandLine.appendSwitch('disable-backgrounding-occluded-windows');
app.commandLine.appendSwitch('autoplay-policy', 'no-user-gesture-required');
// Renkler birebir kalsin: oyun eklentisi saf macentayi (FF00FF) seffaf yapar
app.commandLine.appendSwitch('force-color-profile', 'srgb');

let win;
const fromGame = process.argv.includes('--oyundan');
let parkedFrom = null;
function park() {
  if (!win || parkedFrom) return;
  parkedFrom = win.getPosition();
  win.setPosition(-5000, 0);
}
function unpark() {
  if (!win || !parkedFrom) return;
  win.setPosition(parkedFrom[0], parkedFrom[1]);
  parkedFrom = null;
}
function chromeUA() {
  return session.defaultSession.getUserAgent()
    .replace(/\s?Electron\/\S+/i, '')
    .replace(/\s?ets2-carplay\/\S+/i, '');
}

function createWindow() {
  win = new BrowserWindow({
    show: !fromGame,
    width: 1440, height: 810, useContentSize: true,
    resizable: false, maximizable: false, minimizable: false, fullscreenable: false,
    frame: false, backgroundColor: '#000000',
    title: 'ETS2 CarPlay',
    icon: path.join(__dirname, 'assets', 'icon.png'),
    webPreferences: {
      webviewTag: true,
      backgroundThrottling: false,
      contextIsolation: false,
      nodeIntegration: true
    }
  });
  // Pencere basligi hep ayni kalsin (eklenti pencereyi kaybetmesin)
  win.on('page-title-updated', e => e.preventDefault());
  win.setTitle('ETS2 CarPlay');
  win.loadFile('index.html');
  win.webContents.on('did-finish-load', () => { if (updReady) win.webContents.send('update', { ready: true, version: updReady }); });
  if (fromGame) {
    win.once('ready-to-show', () => { park(); win.showInactive(); });
  }

  // Kucultulen pencere goruntu vermez (eklenti "zero client area" der).
  // Bu yuzden kucultmek yerine pencereyi ekranin disina park ediyoruz.
  win.on('minimize', () => { park(); win.showInactive(); });
  win.on('focus', () => unpark());

  // Farenin geri / ileri tuslari
  win.on('app-command', (e, cmd) => {
    if (cmd === 'browser-backward') win.webContents.send('nav', 'back');
    if (cmd === 'browser-forward') win.webContents.send('nav', 'forward');
  });
}

app.whenReady().then(() => {
  // Siteler koyu temada acilsin (YouTube, Google...) -> kamyon ekraninda beyaz alan parlamasin
  nativeTheme.themeSource = 'dark';
  const ua = chromeUA();
  app.userAgentFallback = ua;
  const part = session.fromPartition('persist:carplay');
  part.setUserAgent(ua);
  part.setPermissionRequestHandler((wc, perm, cb) => cb(true));
  createWindow();
  setupUpdater();
  watchGame();
  setupAdblock(part);
});

// Reklam engelleyici: uBlock Origin / EasyList listeleriyle calisan Ghostery motoru.
// Listeler haftada bir yenilenir, acik/kapali tercihi ayarlar.json'da durur.
const fsp = require('fs').promises;
const prefFile = () => path.join(app.getPath('userData'), 'ayarlar.json');
function readPrefs() { try { return JSON.parse(require('fs').readFileSync(prefFile(), 'utf8')); } catch (e) { return {}; } }
function writePrefs(p) { try { require('fs').writeFileSync(prefFile(), JSON.stringify(p)); } catch (e) {} }
let blocker = null, adSession = null, adOn = readPrefs().adblock !== false;
async function setupAdblock(part) {
  adSession = part;
  try {
    const { ElectronBlocker } = require('@ghostery/adblocker-electron');
    const cache = path.join(app.getPath('userData'), 'reklam-listeleri.bin');
    try { const st = await fsp.stat(cache); if (Date.now() - st.mtimeMs > 7 * 864e5) await fsp.unlink(cache); } catch (e) {}
    blocker = await ElectronBlocker.fromPrebuiltAdsAndTracking(fetch, { path: cache, read: fsp.readFile, write: fsp.writeFile });
    if (adOn) blocker.enableBlockingInSession(part);
    if (win && !win.isDestroyed()) win.webContents.send('adblock-ready');
  } catch (e) { blocker = null; console.error('reklam engelleyici baslatilamadi', e); }
}
ipcMain.handle('adblock-get', () => ({ on: adOn, ready: !!blocker }));
ipcMain.on('adblock-set', (e, on) => {
  adOn = !!on; const p = readPrefs(); p.adblock = adOn; writePrefs(p);
  if (!blocker || !adSession) return;
  try { if (adOn) { if (!blocker.isBlockingEnabled(adSession)) blocker.enableBlockingInSession(adSession); } else if (blocker.isBlockingEnabled(adSession)) blocker.disableBlockingInSession(adSession); } catch (err) {}
});

// Oyun kapaninca uygulama da kapansin (oyun bir kez calisirken goruldukten sonra)
function watchGame() {
  const { execFile } = require('child_process');
  let seen = fromGame, gone = 0;
  setInterval(() => {
    execFile('tasklist', ['/FI', 'IMAGENAME eq eurotrucks2.exe', '/NH', '/FO', 'CSV'], { windowsHide: true }, (err, out) => {
      if (err) return;
      const running = /eurotrucks2\.exe/i.test(out || '');
      if (running) { seen = true; gone = 0; }
      else if (seen && ++gone >= 2) app.quit();
    });
  }, 4000);
}

// Otomatik guncelleme: GitHub'daki yeni surumu arka planda indirir. Kurulum yalnizca kullanici
// "Guncelle"ye basinca ya da uygulama kapanirken olur (bir sonraki acilista yeni surum gelir).
let updater = null, updReady = null, updState = { state: 'idle' };
function updSend(s) { updState = s; if (win && !win.isDestroyed()) win.webContents.send('upd-status', s); }
function setupUpdater() {
  if (!app.isPackaged) { updState = { state: 'dev' }; return; }
  try { updater = require('electron-updater').autoUpdater; } catch (e) { updState = { state: 'error' }; return; }
  updater.autoDownload = true;
  updater.autoInstallOnAppQuit = true;
  updater.on('checking-for-update', () => { if (!updReady) updSend({ state: 'checking' }); });
  updater.on('update-not-available', () => { if (!updReady) updSend({ state: 'none' }); });
  updater.on('update-available', (i) => updSend({ state: 'downloading', version: i.version, percent: 0 }));
  updater.on('download-progress', (p) => updSend({ state: 'downloading', version: updState.version, percent: Math.round(p.percent || 0) }));
  updater.on('update-downloaded', (i) => {
    updReady = i.version;
    updSend({ state: 'ready', version: i.version });
    if (win) win.webContents.send('update', { ready: true, version: i.version });
  });
  updater.on('error', () => { if (!updReady) updSend({ state: 'error' }); });
  const check = () => { if (!updReady && updState.state !== 'downloading') updater.checkForUpdates().catch(() => {}); };
  setTimeout(check, 8000);
  setInterval(check, 3 * 60 * 60 * 1000);
}
ipcMain.handle('upd-get', () => ({ ...updState, current: app.getVersion() }));
ipcMain.on('upd-check', () => {
  if (!updater) { updSend(updState); return; }
  if (updReady) { updSend({ state: 'ready', version: updReady }); return; }
  if (updState.state === 'downloading') return;
  updater.checkForUpdates().catch(() => updSend({ state: 'error' }));
});
ipcMain.on('update-install', () => { if (updater && updReady) setImmediate(() => updater.quitAndInstall(true, true)); });

// Webview icindeki yeni pencere acma isteklerini ayni ekranda ac
app.on('web-contents-created', (e, contents) => {
  if (contents.getType() === 'webview') {
    contents.setWindowOpenHandler(({ url }) => { contents.loadURL(url); return { action: 'deny' }; });
    contents.on('before-input-event', (ev, input) => {
      if (input.type !== 'keyDown') return;
      if (input.alt && input.key === 'ArrowLeft') { win.webContents.send('nav', 'back'); ev.preventDefault(); }
      if (input.alt && input.key === 'ArrowRight') { win.webContents.send('nav', 'forward'); ev.preventDefault(); }
      if (input.alt && input.key === 'Home') { win.webContents.send('nav', 'home'); ev.preventDefault(); }
      if (input.control && input.key.toLowerCase() === 'l') { win.webContents.send('nav', 'url'); ev.preventDefault(); }
    });
  }
});

ipcMain.on('win', (e, cmd) => {
  if (!win) return;
  if (cmd === 'min') { park(); win.blur(); }
  if (cmd === 'close') win.close();
});

ipcMain.handle('setup', () => { try { return runSetup(app); } catch (e) { return { error: e.message }; } });
// Kamyon ekrani ayarlari: eklenti bu dosyayi saniyede bir okur
ipcMain.on('screencfg', (e, c) => {
  try {
    const n = (v, lo, hi, d) => { v = parseInt(v, 10); return isNaN(v) ? d : Math.max(lo, Math.min(hi, v)); };
    const dir = path.join(app.getPath('documents'), 'Euro Truck Simulator 2');
    require('fs').mkdirSync(dir, { recursive: true });
    const txt = `parlaklik=${n(c.dim,5,100,100)}\nbeyaz=${n(c.w,0,100,85)}\nfps=${n(c.fps,10,60,60)}\nsol=${n(c.l,0,25,2)}\nust=${n(c.t,0,25,2)}\nsag=${n(c.r,0,25,0)}\nalt=${n(c.b,0,25,4)}\n`;
    require('fs').writeFileSync(path.join(dir, 'ets2_carplay.cfg'), txt);
  } catch (x) {}
});
ipcMain.on('open-dir', (e, dir) => { if (dir) { try { require('fs').mkdirSync(dir, { recursive: true }); } catch (x) {} shell.openPath(dir); } });

// Tek kopya calissin
if (!app.requestSingleInstanceLock()) app.quit();
else app.on('second-instance', () => { if (win) { if (win.isMinimized()) win.restore(); win.focus(); } });

app.on('window-all-closed', () => app.quit());
