// ETS2 CarPlay - ilk acilista otomatik kurulum
// 1) Smart Dash modunu Belgeler\Euro Truck Simulator 2\mod klasorune koyar
// 2) Oyunun "infotainment" kontrolunu (bos ise) F8 tusuna atar
// 3) PrismTextureStreamer eklentisinin kurulu olup olmadigini kontrol eder
const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const MOD_NAME = 'CarPlay_Scania_SmartDash.scs';
const PLUGIN_NAME = 'ets2_carplay.dll';

function docsDir(app) {
  return path.join(app.getPath('documents'), 'Euro Truck Simulator 2');
}

function resFile(app, name) {
  const packed = path.join(process.resourcesPath || '', name);
  if (app.isPackaged && fs.existsSync(packed)) return packed;
  return path.join(__dirname, 'assets', name);
}
function modSource(app) { return resFile(app, MOD_NAME); }

function gameRunning() {
  try {
    const out = execSync('tasklist /FI "IMAGENAME eq eurotrucks2.exe" /NH', { encoding: 'utf8', windowsHide: true });
    return /eurotrucks2\.exe/i.test(out);
  } catch (e) { return false; }
}

function steamLibraries() {
  const libs = new Set();
  let steam = null;
  try {
    const out = execSync('reg query "HKCU\\Software\\Valve\\Steam" /v SteamPath', { encoding: 'utf8', windowsHide: true });
    const m = out.match(/SteamPath\s+REG_SZ\s+(.+)/i);
    if (m) steam = m[1].trim().replace(/\//g, '\\');
  } catch (e) {}
  const guesses = [steam, 'C:\\Program Files (x86)\\Steam', 'C:\\Program Files\\Steam'].filter(Boolean);
  for (const g of guesses) {
    libs.add(g);
    try {
      const vdf = fs.readFileSync(path.join(g, 'steamapps', 'libraryfolders.vdf'), 'utf8');
      for (const m of vdf.matchAll(/"path"\s+"([^"]+)"/g)) libs.add(m[1].replace(/\\\\/g, '\\'));
    } catch (e) {}
  }
  return [...libs];
}

function gameDir() {
  for (const lib of steamLibraries()) {
    const d = path.join(lib, 'steamapps', 'common', 'Euro Truck Simulator 2');
    if (fs.existsSync(path.join(d, 'bin', 'win_x64', 'eurotrucks2.exe'))) return d;
  }
  return null;
}

function installMod(app) {
  const src = modSource(app);
  if (!fs.existsSync(src)) return { ok: false, msg: 'Mod dosyası uygulamada bulunamadı' };
  const dstDir = path.join(docsDir(app), 'mod');
  const dst = path.join(dstDir, MOD_NAME);
  try {
    fs.mkdirSync(dstDir, { recursive: true });
    const a = fs.readFileSync(src);
    const same = fs.existsSync(dst) && Buffer.compare(a, fs.readFileSync(dst)) === 0;
    if (!same) fs.writeFileSync(dst, a);
    return { ok: true, msg: same ? 'Mod kurulu' : 'Mod kuruldu / güncellendi' };
  } catch (e) {
    return { ok: false, msg: 'Mod kopyalanamadı: ' + e.message };
  }
}

function bindF8(app) {
  const base = docsDir(app);
  const files = [];
  for (const sub of ['profiles', 'steam_profiles']) {
    const dir = path.join(base, sub);
    if (!fs.existsSync(dir)) continue;
    for (const p of fs.readdirSync(dir)) {
      const f = path.join(dir, p, 'controls.sii');
      if (fs.existsSync(f)) files.push(f);
    }
  }
  if (!files.length) return { ok: false, msg: 'Oyun profili bulunamadı (oyunu bir kez aç)' };
  if (gameRunning()) {
    const done = files.every(f => /mix infotainment `[^`]*keyboard\./.test(fs.readFileSync(f, 'utf8')));
    return { ok: done, msg: done ? 'F8 tuşu atanmış' : 'Oyun açık — kapatıp uygulamayı yeniden aç' };
  }
  let changed = 0, already = 0, busy = 0;
  for (const f of files) {
    let s = fs.readFileSync(f, 'utf8');
    if (/mix infotainment `[^`]*keyboard\./.test(s)) { already++; continue; }
    if (/keyboard\.f8\?/.test(s)) { busy++; continue; }
    const n = s.replace(/"mix infotainment `semantical\.infotainment\?0`"/, '"mix infotainment `keyboard.f8?0 | semantical.infotainment?0`"');
    if (n !== s) {
      try { fs.copyFileSync(f, f + '.carplay.bak'); } catch (e) {}
      fs.writeFileSync(f, n); changed++;
    }
  }
  if (busy && !changed && !already) return { ok: false, msg: 'F8 başka bir işe atanmış — Ayarlar › Tuşlar › Infotainment' };
  return { ok: true, msg: changed ? 'F8 tuşu atandı' : 'F8 tuşu atanmış' };
}

function installPlugin(app) {
  const g = gameDir();
  if (!g) return { ok: false, msg: 'ETS2 klasörü bulunamadı', dir: null };
  const dir = path.join(g, 'bin', 'win_x64', 'plugins');
  const src = resFile(app, PLUGIN_NAME);
  const dst = path.join(dir, PLUGIN_NAME);
  const notes = [];
  try {
    fs.mkdirSync(dir, { recursive: true });
    // Eski PrismTextureStreamer eklentisi ayni isi yapar ve cakisir: devre disi birak (silmeden)
    for (const n of fs.readdirSync(dir)) {
      if (/^PrismTextureStreamer.*\.dll$/i.test(n)) {
        if (gameRunning()) { notes.push('eski eklenti için oyunu kapat'); continue; }
        try { fs.renameSync(path.join(dir, n), path.join(dir, n + '.devre_disi')); notes.push('eski eklenti devre dışı'); } catch (e) {}
      }
    }
    const a = fs.readFileSync(src);
    const same = fs.existsSync(dst) && Buffer.compare(a, fs.readFileSync(dst)) === 0;
    if (!same) {
      if (gameRunning()) return { ok: false, msg: 'Ekran eklentisi için oyunu kapatıp uygulamayı yeniden aç', dir };
      fs.writeFileSync(dst, a);
      return { ok: true, msg: 'Ekran eklentisi kuruldu' + (notes.length ? ' (' + notes.join(', ') + ')' : ''), dir };
    }
    return { ok: true, msg: 'Ekran eklentisi kurulu', dir };
  } catch (e) {
    return { ok: false, msg: 'Eklenti kurulamadı: ' + e.message, dir };
  }
}

function runSetup(app) {
  const res = { mod: installMod(app), keys: bindF8(app), plugin: installPlugin(app) };
  return res;
}

module.exports = { runSetup };
