# Los Santos — Pure C++ GUI

This recreates the supplied Los Santos HTML page as a native C++ desktop GUI using Qt 6 Widgets. It does not embed a browser or use HTML/CSS/JavaScript.

## Requirements
- C++17 compiler
- CMake 3.16+
- Qt 6 with the Widgets module

## Build
From this folder:

```bash
cmake -S . -B build
cmake --build build --config Release
```

Run the resulting `LosSantos` executable (on Windows, usually in `build/Release/` or `build/` depending on generator).

## Notes
- The visual style and page copy are recreated in native Qt widgets and custom painting.
- “Play Now” and “Play Area” scroll to the launch area.
- “How It Works” scrolls to the setup cards.
- “View Setup Checklist” opens the checklist dialog.
- No GTA V files, cloud stream, or game-streaming backend are included.
- As a native GUI rewrite, typography and responsive layout cannot be pixel-identical to browser rendering.
