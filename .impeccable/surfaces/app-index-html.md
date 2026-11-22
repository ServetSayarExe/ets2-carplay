---
version: 1
slug: "app-index-html"
primary_target: "app/index.html"
related_targets: []
---

# ETS2 CarPlay app window (home = game map, browser, settings)

Mode: Operate. Fixed 1440×810 window, streamed into the Scania Smart Dash.

## Scope and task
- Audience: ETS2 players mid-drive; one Alt+Tab to pick media, then glance reading on the truck screen.
- Task: open YouTube / YouTube Music / Google / radio / Kick / news / a pasted link; control now playing; tune the truck-screen image; see setup problems.
- Constraints: magenta #FF00FF and the 1px #1337C4 top-left marker are reserved for the game-map stage; window title and size never change; Turkish UI; script behaviours kept.
- Anti-reference: the previous UIs (navy #08101c / amber #f0a43a tile grid, and the rejected "Truck Stop at Night" navy/sodium grid). No navy ground, no amber accent, no right-column card + bottom tile row arrangement.

## Direction contract

THESIS: The game's own map is the screen. Home is not a launcher with a map tile in it; it is the live navigation, full bleed, with the app's controls floating at its edges like a vertical-video feed's rail and caption. It refuses the category's grid of tiles entirely.

OWN-WORLD: The stage is the game (pure magenta in the window, transparent in the truck). Controls are opaque near-black discs and pills (#0f1013, #191b20) with warm white ink (#f6f5f1); each app keeps one fixed hue on its glyph (YouTube #ff4f45, Music #ff6f9c, Google #79acff, radio #6ddf7c, Kick #5ff22c, news #4fd6c8, last link #d8d4ca). The engaged control fills warm white with black ink; no other accent colour. A soft black scrim rises from the bottom edge for the caption. Familjen Grotesk (500/600/700) for everything; big tight headline, small bold labels.

STORY: The player Alt+Tabs in, sees their route full screen with the greeting and what is playing anchored low-left, taps an app in the right-hand stack or pastes a link in the floating search, and goes back to driving; in the truck the map stays visible almost everywhere.

FIRST VIEWPORT: 96px black rail on the left (clock, date, home, back, forward, refresh, then the playing mini disc and settings at the bottom; off the map, in the browser and settings, the rail also carries YouTube, Music and Google discs so the active site can show its engaged fill). Stage: full bleed game map. Floating search pill top-left (620px, white Open button), update chip and setup-problem chip top-right only when needed. Bottom-left caption: greeting at 60px, one line under it, then the now-playing strip (artwork, title, artist, previous/play/next discs). Right edge: a vertical stack of seven app discs with labels in small black pills (YouTube, Müzik, Google, Radyo, Kick, Haberler, Son link); the disc of the playing or last-opened source carries the engaged fill. The now-playing strip names its source.

FORM: Bolder re-roll 1, dealt challenger pop-culture-shelf-vertical-video-feed-surface (verdict: wins against the round's leader on audience identification and product clarity), chosen by the user over the leader and the cassette j-card. seed:ef99d93389e6dec7. Kept from the declined type specimen: hierarchy by scale contrast alone. Signature move: the engaged-disc fill. The control for what is active or playing fills warm white and its glyph turns black, the way an engaged action fills on a feed rail; views change with a hard snap, never a slide or fade.

FINISH: unreviewed and undocumented is unfinished; this build ends with the finish review, the verdict, DESIGN.md, and every shipping raster carrying its provenance
