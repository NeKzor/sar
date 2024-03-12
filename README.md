# SAR for Strata Source

> [!IMPORTANT]
> This is based on SAR version 1.11 (2020) unreleased

## Supported Games

* [Portal 2: Community Edition](https://portal2communityedition.com/)
* [Portal: Revolution](https://www.portalrevolution.com)

## Installation

### Windows

* Go to the game's bin folder `Portal Revolution\bin\win64`
* Rename `audio_minimp3.dll` to `audio_minimp3_original.dll` (this is required and does not count as a backup)
* Extract the downloaded files into the folder
* Launch game, open developer console and test if SAR loaded

### Linux

* Go to the game's bin folder `Portal Revolution\bin\linux64`
* Extract files
* Load SAR with gdb:
  * `sudo gdb`
  * `source sar.py`
  * `sar_load revolution`

## Features

Partially ported from SAR 1.12.

* Vphys HUD
  * `sar_vphys_hud`
  * `sar_vphys_hud_font`
  * `sar_vphys_hud_precision`
  * `sar_vphys_hud_x`
  * `sar_vphys_hud_y`
  * `sar_vphys_setgravity`
  * `sar_vphys_setangle` - NEW: Optional component argument.
  * `sar_vphys_setspin` - NEW: Optional component argument.
  * `sar_vphys_setasleep`
* Event commands
  * `sar_on_load`
  * `sar_on_load_clear`
  * `sar_on_load_list`
  * `sar_on_session_end`
  * `sar_on_session_end_clear`
  * `sar_on_session_end_list`

## Known Issues

* Demo playback crashes
* Autocomplete shows empty entries
* Autostrafer is broken
* Old speedrun timer requires rules
* HUD uses old font system:
  * `sar_hud_default_font_index`
  * `sar_ihud_font_index`
  * `sar_vphys_hud_font`
* `sar_exit` is required to exit the game

## License

[MIT License](./LICENSE)
