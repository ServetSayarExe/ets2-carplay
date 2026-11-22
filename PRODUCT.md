# Product

<!-- impeccable:product-schema 1 -->

## Platform

web

## Users

Euro Truck Simulator 2 players who drive a Scania S/R (2024+) with the Smart Dash infotainment screen and want music, video and the web on the truck's center screen. The app is shared publicly on GitHub, so it serves players who downloaded it themselves, not only its author.

The primary usage scene is mid-drive: the player Alt+Tabs out of the game, picks something in the app window in a few seconds, and goes back to driving. After that they mostly look at the in-game screen, at glance distance, while driving.

## Product Purpose

Puts a CarPlay-style infotainment screen on the Scania Smart Dash: YouTube, YouTube Music, Netflix, Prime Video, Animecix, Google, radio, live streams, news and any pasted link, with the game's own navigation map one tap away. Success means a player can start what they want within one Alt+Tab, and the truck screen stays readable and calm while driving, at night included.

## Positioning

It is the only way to get a real, working browser and media screen inside the Smart Dash: the app's window is streamed live into the game's own dashboard texture by a native plugin, and the game's navigation shows through wherever the app paints the map key color.

## Operating Context

- Two surfaces show the same pixels: the 1440×810 app window on the PC, and the truck's center screen, which is the same image scaled into the dashboard at glance distance, through the game's lighting and the in-game brightness settings.
- Interaction happens only on the PC window with mouse and keyboard, mostly in brief Alt+Tab visits. Shortcuts: Alt+← back, Alt+→ forward, Alt+Home home, Ctrl+L address bar; mouse back/forward buttons.
- The app starts with the game and closes when the game closes. Until the player opens something, it shows the map view.
- The in-game image is captured from the app window (PrintWindow); large pure-white areas glare on the truck screen at night.

## Capabilities and Constraints

- Views: home, which is the game map itself (the stage is the reserved key colour, so the game's navigation shows through, with the app's controls floating at its edges), browser (two webviews: music and web), and screen settings. There is no separate map view.
- Home/map stage: the app paints the stage key #100020 (near-black on the PC) and a 1px marker (#1337C4) in the window's top-left corner; the plugin makes the key transparent, and black blended over it partially transparent, only when it sees the marker. No other UI element may use the key (or pure magenta, still keyed for older builds), and nothing may cover the top-left pixel on home.
- Fixed 1440×810 frameless window, no resizing; everything must fit and read at that size and when scaled into the dashboard.
- Screen settings: brightness (5-200%), soften whites (0-100), smoothness (10-60 fps), fit-to-screen insets (left/top/right/bottom 0-25%), with a live preview. Values are written to `ets2_carplay.cfg` and apply in-game within a second.
- Setup status: mod, F8 key binding and plugin install state, listed in settings, with a link to the plugins folder when the plugin is missing; home shows a small warning only when something is wrong.
- Now playing: title, artist, source, artwork and play/pause/next/previous for YouTube and YouTube Music, shown on home over the map.
- Auto-update: background download, an Update button on home and a check button in settings.
- Optional player name for the greeting.
- Turkish UI.

## Brand Commitments

- The name stays **ETS2 CarPlay**.
- The map stays the main and largest element of the home screen.
- Navigation (home, back, forward, refresh) stays in a rail on the left.
- Everything else about the look is open: the redesign replaces the current visual world rather than polishing it.
- Author credit: servetsayarexe.

## Evidence on Hand

- Real app shortcuts and URLs: YouTube, YouTube Music, Google, radio.garden, Kick, Google News (Turkish).
- App icon: `app/assets/icon.png`, `app/assets/icon.ico`.
- No screenshots, testimonials, user counts or download numbers exist; do not invent them.

## Product Principles

1. **One Alt+Tab is enough.** Every common action is reachable in one or two clicks from wherever the app is.
2. **Read at a glance, in the cab.** The truck screen is read from the driver's seat while driving; big type, high contrast, no glare.
3. **The game comes first.** The map is the default and the anchor; the app never fights the game's own navigation.
4. **Never break the stream.** Magenta and the map marker are reserved; the window size and title never change.
