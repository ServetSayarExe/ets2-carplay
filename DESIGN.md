---
name: ETS2 CarPlay
description: The game's own map is the screen; the app's controls float at its edges like a feed's rail and caption.
colors:
  stage-key: "#100020"
  map-marker: "#1337c4"
  rail-black: "#000000"
  page: "#0b0c0e"
  disc: "#0f1013"
  disc-raised: "#191b20"
  disc-pressed: "#24272e"
  ink: "#f6f5f1"
  ink-quiet: "#9c998f"
  engaged: "#f6f5f1"
  engaged-ink: "#0f1013"
  status-bad: "#ff7a59"
  status-good: "#7ee2a0"
  app-youtube: "#ff4f45"
  app-music: "#ff6f9c"
  app-google: "#79acff"
  app-radio: "#6ddf7c"
  app-kick: "#5ff22c"
  app-news: "#4fd6c8"
  app-last: "#d8d4ca"
typography:
  display:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "60px"
    fontWeight: 700
    lineHeight: 0.98
    letterSpacing: "-0.03em"
  headline:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "56px"
    fontWeight: 700
    lineHeight: 0.95
    letterSpacing: "-0.03em"
  clock:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "32px"
    fontWeight: 700
    lineHeight: 1
    letterSpacing: "-0.02em"
    fontFeature: "tnum"
  title:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "22px"
    fontWeight: 700
    lineHeight: 1.35
    letterSpacing: "-0.01em"
  body:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "18px"
    fontWeight: 400
    lineHeight: 1.35
  body-strong:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "19px"
    fontWeight: 700
    lineHeight: 1.35
  help:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "15px"
    fontWeight: 400
    lineHeight: 1.45
  label:
    fontFamily: "Familjen Grotesk, Segoe UI, system-ui, sans-serif"
    fontSize: "13px"
    fontWeight: 700
    lineHeight: 1.35
rounded:
  label: "10px"
  preview: "12px"
  panel: "24px"
  full: "9999px"
spacing:
  xs: "6px"
  sm: "10px"
  md: "14px"
  lg: "18px"
  xl: "24px"
  rail: "96px"
components:
  disc:
    backgroundColor: "{colors.disc}"
    textColor: "{colors.ink}"
    rounded: "{rounded.full}"
    size: "56px"
  disc-hover:
    backgroundColor: "{colors.disc-raised}"
  disc-engaged:
    backgroundColor: "{colors.engaged}"
    textColor: "{colors.engaged-ink}"
  app-disc:
    backgroundColor: "{colors.disc}"
    rounded: "{rounded.full}"
    size: "58px"
  app-label:
    backgroundColor: "{colors.disc}"
    textColor: "{colors.ink}"
    typography: "{typography.label}"
    rounded: "{rounded.label}"
    padding: "2px 9px"
  pill:
    backgroundColor: "{colors.disc-raised}"
    textColor: "{colors.ink}"
    rounded: "{rounded.full}"
    height: "46px"
    padding: "0 20px"
  pill-hover:
    backgroundColor: "{colors.disc-pressed}"
  pill-fill:
    backgroundColor: "{colors.engaged}"
    textColor: "{colors.engaged-ink}"
  search-pill:
    backgroundColor: "{colors.disc}"
    textColor: "{colors.ink-quiet}"
    rounded: "{rounded.full}"
    height: "62px"
    width: "620px"
    padding: "8px 8px 8px 22px"
  url-field:
    backgroundColor: "{colors.disc-raised}"
    textColor: "{colors.ink}"
    rounded: "{rounded.full}"
    height: "52px"
    padding: "0 20px"
  chip:
    backgroundColor: "{colors.disc}"
    textColor: "{colors.ink}"
    rounded: "{rounded.full}"
    height: "50px"
    padding: "6px 6px 6px 18px"
  now-playing:
    backgroundColor: "{colors.disc}"
    textColor: "{colors.ink}"
    rounded: "{rounded.full}"
    height: "80px"
    padding: "8px 10px 8px 8px"
  settings-panel:
    backgroundColor: "{colors.disc}"
    textColor: "{colors.ink}"
    rounded: "{rounded.panel}"
    padding: "24px 28px"
  rail:
    backgroundColor: "{colors.rail-black}"
    textColor: "{colors.ink}"
    width: "96px"
    height: "810px"
---

# Design System: ETS2 CarPlay

## Overview

**Creative North Star: "The Map Is the Screen"**

Home is not a launcher with a map tile in it. It is the game's live navigation, full bleed, with the app's controls floating at its edges the way a vertical-video feed floats its action rail and caption over the footage. The window paints a reserved key colour; the game plugin turns it transparent in the truck, so the route shows through almost everywhere and the app reads as a thin layer of opaque black discs and pills on top of it. The category's grid of tiles is refused entirely.

Everything is near-black and warm white. Each app keeps exactly one fixed hue, and only on its glyph. The single interaction signal is the engaged fill: the control for what is active or playing fills warm white and its glyph turns black. There is no other accent colour. Hierarchy comes from scale contrast alone, one grotesque at three weights, a big tight headline against small bold labels. Density is low and fixed: one 1440×810 window that must also read when scaled into the Smart Dash at glance distance, at night included.

Rejected looks this system replaced: a navy ground with an amber accent in a tile grid, and a right-hand card above a bottom tile row.

**Key Characteristics:**
- The stage is the game: a near-black violet key (#100020) in the window, transparent in the truck.
- Opaque near-black discs and pills, soft-lifted, floating at the map's edges.
- Warm white ink (never pure white over large areas).
- One fixed hue per app, on the glyph only.
- The engaged-disc fill is the only state colour.
- Familjen Grotesk 400/600/700, hierarchy by scale.
- Views change with a hard snap; no slides or fades.

## Colors

A near-black and warm-white system over a reserved key colour, with app hues confined to glyphs and two status dots.

### Primary
- **Warm Bone** (`engaged` / `ink`): the engaged fill and all primary ink. As ink it carries every label, the greeting and values; as a fill it marks the one control that is active or playing. Same value in both roles on purpose: the engaged state is the ink turning into the surface.
- **Disc Black** (`engaged-ink`): glyph and label colour inside an engaged control.

### Neutral
- **Rail Black** (`rail-black`): the 96px left rail and the browser chrome. Pure black so the rail reads as the bezel of the screen, not a panel.
- **Night Page** (`page`): the settings view ground, the window background behind everything.
- **Disc** (`disc`): every floating control at rest: discs, search pill, chips, now-playing strip, app labels, settings panels.
- **Disc Raised** (`disc-raised`): hover on discs; rest state of inner pills, fields and transport buttons sitting inside a disc-coloured container.
- **Disc Pressed** (`disc-pressed`): hover on inner pills and transport buttons, chip action buttons, preview frame stroke, spinner track.
- **Quiet Ink** (`ink-quiet`): date, placeholders, help text, secondary metadata (artist line, version info, unconfirmed status).

### Tertiary (app hues, glyph only)
- **YouTube Red** (`app-youtube`), **Music Pink** (`app-music`), **Google Blue** (`app-google`), **Radio Green** (`app-radio`), **Kick Lime** (`app-kick`), **News Teal** (`app-news`), **Last-Link Stone** (`app-last`): one per app, applied to the glyph's `color` and nothing else. When the disc engages, the glyph turns Disc Black and the hue drops away.

### Status
- **Signal Coral** (`status-bad`) and **Signal Mint** (`status-good`): 10px status dots in the setup row and the home warning chip. Never fills, never text.

### Reserved (not palette)
- **Stage Key** (`stage-key`) and **Map Marker** (`map-marker`): see the Stage Key Rule. They are protocol values for the game plugin, not colours to design with.

### Named Rules
**The Stage Key Rule.** The stage key #100020 (rgb 16,0,32) paints the home stage and nothing else, ever; on the PC it reads as near-black, so the window never shows a bright key colour. Only while the 1px Map Marker sits at the window's top-left pixel (shown only on the map view) does the plugin key it: bare key becomes transparent, and black blended over the key (green 0, red about half of blue) is un-mixed into black at partial alpha. That is why the bottom scrim, a black gradient over the key, becomes a real translucent scrim in the truck. Any other colour with green 0 and red about half of blue under 34 is reserved too. Pure magenta #FF00FF is still keyed for older app builds and must not be used either. Nothing may cover the top-left pixel on the map view.

**The One Fill Rule.** The engaged fill (warm white, black glyph) is the only state colour. At most one control per group carries it: the current view in the rail, the playing or last-opened source in the app stack, the play button while audio plays, the primary action in a pill pair.

**The Glyph-Only Hue Rule.** An app's hue lives on its glyph. Discs, labels and pills stay Disc Black regardless of app.

## Typography

**Display Font:** Familjen Grotesk (variable, 400 to 700, self-hosted Latin and Latin-Ext subsets; with Segoe UI, system-ui)
**Body Font:** the same family.

**Character:** One compact grotesque for everything; tight negative tracking on large sizes, bold small labels, and nothing in between doing decorative work. Hierarchy is scale contrast alone.

### Hierarchy
- **Display** (700, 60px, 0.98, -0.03em): the home greeting in the bottom-left caption. One per screen.
- **Headline** (700, 56px, 0.95, -0.03em): the settings view title.
- **Clock** (700, 32px, 1, -0.02em, tabular figures): the rail clock.
- **Title** (700, 22px, -0.01em): settings group headings.
- **Body Strong** (700, 19px): now-playing title, slider values.
- **Body** (400, 18px to 20px, 1.35): sub-caption under the greeting (19px), settings row labels (18px), search and URL input text (20px / 18px).
- **Pill text** (600, 17px; chips 600, 16px): button and chip labels.
- **Help** (400, 15px, 1.45, max 58ch): explanatory copy in settings, in Quiet Ink.
- **Label** (700, 13px): app labels under the stack discs, always on an opaque Disc pill.

### Named Rules
**The Scale-Only Rule.** Rank is shown by size and weight, never by a second family, colour, uppercase or letterspaced small caps.

## Layout

A fixed 1440×810 frameless canvas: a 96px rail on the left (pure black, full height) and a 1344×810 stage. The window size and title never change, and nothing responds to other sizes.

- **Rail:** clock and date at top, then home, back, forward, refresh discs on an 8px column gap; off the map the rail adds a 36px separator and YouTube, Music and Google discs. A flexible spacer pushes the playing mini disc (64px, artwork) and the settings disc to the bottom.
- **Home stage:** everything floats at the edges with a 24px inset (28px from the bottom). Search pill top-left, conditional chips top-right, caption bottom-left (720px wide: greeting, one sub line, now-playing strip), vertical app stack bottom-right (seven 58px discs with labels, 12px gap). The middle of the stage stays empty for the map.
- **Settings:** 34px/40px padding, a header row, a two-column grid of panels (20px gap), a full-width setup row and a footer, vertically centred.
- **Browser:** a 72px black bar (URL field, paste pill) above the webviews.
- **Rhythm:** gaps step 6, 8, 10, 12, 14, 18, 20, 24px. Edge inset is 24px.

### Named Rules
**The Empty Middle Rule.** On home, controls hug the four edges; the centre of the stage belongs to the route.

## Elevation & Depth

Flat surfaces with one soft ambient lift. Floating controls on the map carry a single dark drop shadow so they separate from any map colour; panels inside the settings page sit flat on the Night Page. Depth on the stage also comes from the bottom scrim, a black gradient rising 360px from the bottom edge (0 to 42% at mid-height to 70% at the bottom) under the caption. The caption text adds a faint black text shadow.

### Shadow Vocabulary
- **Lift** (`box-shadow: 0 10px 24px -10px rgba(0,0,0,.75)`): every floating disc, the search pill, chips, the now-playing strip and the mini disc.
- **Focus ring** (`box-shadow: 0 0 0 2px #f6f5f1` on fields; `outline: 3px solid #f6f5f1; outline-offset: 3px` on buttons): keyboard focus and the search "need" hint (3px ring).

### Named Rules
**The Opaque Backing Rule.** Any label or control over the stage sits on an opaque Disc surface (pill, disc, strip). Only the caption greeting and its sub line ride directly on the stage, and only above the scrim.

## Shapes

Round everything that floats: discs are circles, controls are full pills (radius = half height), artwork is circular. Containers on the settings page use a soft 24px corner; the fit-to-screen preview uses 12px; app labels use a 10px pill. No sharp corners on interactive elements, no hairline borders; separation is by tone and lift.

## Components

### Disc (signature)
The single control language: a 56px circle (58px in the app stack, 52px/60px for transport) in Disc with warm-white glyph and the Lift shadow.
- **Hover:** Disc Raised. **Active:** scale 0.94.
- **Engaged:** fills Warm Bone, glyph turns Disc Black, with a short spring (`engage`, 220ms, scale 0.86 to 1, `cubic-bezier(.2,1.4,.4,1)`). Glyph cut-outs (the YouTube play triangle) follow the fill via a hole colour.
- **Disabled:** transparent, no shadow, dim glyph; used for back and forward when there is nowhere to go.

### Buttons (pills)
- **Shape:** full pill, 46px tall, 20px side padding, 600 weight 17px.
- **Default:** Disc Raised; hover Disc Pressed.
- **Fill (primary):** Warm Bone with Disc Black text ("Aç", ready-to-install update); one per group.

### Search / URL field
- **Home search:** 620×62px pill in Disc with Lift, magnifier glyph, 20px input, then a paste pill and a filled open pill.
- **Browser URL:** full-width 52px pill in Disc Raised with a lock glyph and a loading spinner.
- **Focus:** 2px warm-white ring.

### Chips
Conditional, top-right of home only: 50px Disc pills with a 600 16px label, an optional status dot and an inner 38px action button in Disc Pressed. Shown only when an update is ready or setup has a problem.

### Now-playing strip
An 80px Disc pill: 64px circular artwork, title (700, 19px) and a meta line naming the source in Warm Bone before the artist in Quiet Ink, then previous / play / next transport discs; the play disc carries the engaged fill while audio plays.

### App stack
Seven app discs stacked vertically at the bottom-right edge, each with its fixed glyph hue and a 13px bold label on an opaque Disc pill beneath. The disc of the playing or last-opened source is engaged.

### Settings panels and sliders
24px-corner Disc panels, 24px/28px padding. Slider rows are a 170px label, track and 72px right-aligned tabular value. Track: 6px, Warm Bone fill to the value, grey remainder; thumb: 26px Warm Bone circle with a 5px Disc ring. Setup status uses 10px dots in Signal Mint or Signal Coral.

## Do's and Don'ts

### Do:
- **Do** paint the home stage, and only the home stage, with `stage-key`, and show the 1px `map-marker` at the top-left pixel only on the map view.
- **Do** put every control and label over the stage on an opaque Disc surface with the Lift shadow.
- **Do** use the engaged fill (Warm Bone, Disc Black glyph) for the one active or playing control per group.
- **Do** keep each app's hue on its glyph only.
- **Do** keep the centre of the home stage empty and float controls at a 24px edge inset.
- **Do** rank text by size and weight within Familjen Grotesk.
- **Do** switch views with a hard snap.

### Don't:
- **Don't** use the stage key #100020 (or pure magenta) anywhere except the home stage, and don't cover the top-left pixel on the map view.
- **Don't** introduce a second accent colour; status coral and mint are dots only.
- **Don't** put large pure-white areas on screen; ink is warm white and fills are control-sized.
- **Don't** arrange apps as a tile grid, a right-column card or a bottom tile row, and don't use a navy ground or amber accent.
- **Don't** slide or fade between views.
- **Don't** change the 1440×810 window size or its title.
