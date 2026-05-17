# LiteQuickUI

A lightweight UI engine powered by **litehtml** and **QuickJS**, designed for modularity, portability, and extensibility.

LiteQuickUI combines HTML/CSS layout (via litehtml) with a fast embedded JavaScript engine (QuickJS).  
Rendering backends, text shaping, and platform‑specific features are provided as **optional modules**,  
allowing you to use only what your project needs.

LiteQuickUI consists of three layers — core, text backend, and graphics backend — and the applicable third‑party licenses depend on which modules you enable.

LiteQuickUI uses a modified version of litehtml (BSD-3-Clause) for engine integration.
All original copyright and license notices are preserved.

---

## ✨ Features

- **HTML/CSS layout engine** — powered by litehtml  
- **JavaScript runtime** — powered by QuickJS  
- **Modular architecture**
  - Core engine has minimal dependencies
  - Rendering backends (DirectX 12, Vulkan, Metal, etc.) are optional
- **Platform‑optimized backends**
  - Windows: Direct2D (planned), DirectWrite (planned) , DirectX 11 (planned), DirectX 12 (Windows SDK — no additional license)
  - Linux:  OpenGL (planned), Vulkan (planned)
  - macOS/iOS: Metal (planned)
- **Flexible text rendering**
  - FreeType / HarfBuzz backend available
  - Supports bundled fonts (e.g., Noto Sans, OFL 1.1)
    - Some projects may require bundled fonts as mandatory resources
    - Other projects may rely on system fonts or platform‑native APIs

---

## 📦 Project Structure
```
litequickui/
 ├─ litequickui              # core + text backend + graphics backend
 ├─ litequickui_core/        # Core engine (litehtml + QuickJS)
 ├─ litequickui_dx12/        # DirectX 12 graphics backend (Windows)
 ├─ litequickui_hb_ft/       # HarfBuzz + FreeType text backend
 ├─ litequickui_test/        # Test application
 └─ external/
      ├─ litehtml/
      ├─ quickjs/
      ├─ freetype/
      ├─ harfbuzz/
      └─ fonts/
            └─ noto_sans/
```

---

## 🧩 Modules

### **LiteQuickUI Core (litequickui-core)**

The core engine depends only on:

- **litehtml** (BSD‑3‑Clause)  
- **QuickJS** (MIT)

The core module does **not** use FreeType, HarfBuzz, or any graphics API, or any bundled fonts.
Therefore:

> **When using only the core module, no additional third‑party licenses apply.**

---

### **Optional Modules**

These modules introduce additional dependencies and licenses.

#### **litequickui-hb-ft**
- FreeType (FTL)
- HarfBuzz (MIT)

Note: The FreeType License (FTL) requires that applications using FreeType
include the FreeType copyright and license notice in their documentation
or distribution package.

#### **litequickui-dx12 (Windows only)**
- DirectX 12 (Microsoft Windows SDK — no redistribution requirements)
- FreeType (FTL)
- HarfBuzz (MIT)

#### **litequickui-test**
- FreeType (FTL)
- HarfBuzz (MIT)
- graphics backend (e.g. DirectX 12)
- Supports bundled fonts (e.g. Noto Sans, OFL 1.1)

Each module’s license applies **only when that module is used**.

### 📄 License

LiteQuickUI is released under the **MIT License**.

### Summary

| Module               | Dependencies                                     | License Impact |
|----------------------|--------------------------------------------------|----------------|
| litequickui          | Aggregated: core + selected text backend + selected graphics backend | ✔ Combined licenses of enabled modules |
| litequickui_core     | litehtml (BSD), QuickJS (MIT)                    | ✔ No additional obligations |
| litequickui_dx12     | DirectX 12 (Microsoft SDK), litequickui_hb_ft    | ✔ Applies when module is used |
| litequickui_hb_ft    | FreeType (FTL), HarfBuzz (MIT)                   | ✔ Applies when module is used |
| litequickui_test     | Depends on which text & graphics backend is used & Noto Sans (OFL 1.1) | ✔ Applies when module is used |

See **THIRD_PARTY_LICENSES.md** for detailed information.

---

## 📚 THIRD_PARTY_LICENSES.md (Summary)

``` markdown
# Third-Party Licenses

These licenses apply only if you use the corresponding optional modules.

## litehtml (BSD-3-Clause)
LiteQuickUI uses a modified version of litehtml. The original license notice is preserved.
License: external/litehtml/LICENSE

## QuickJS (MIT)
License: external/quickjs/LICENSE

## FreeType (FTL)
License: external/freetype/docs/FTL.TXT

Note: The FreeType License requires that any software using FreeType must
include the FreeType copyright and license notice in its documentation
or distribution package.

## HarfBuzz (MIT)
License: external/harfbuzz/COPYING

## Noto Sans (SIL Open Font License 1.1)
License: external/fonts/noto_sans/OFL.txt

Note: Noto Sans is licensed under the SIL Open Font License 1.1,  
which permits embedding, modification, and redistribution under specific conditions.  
The font may not be sold on its own.

## Microsoft DirectX 12
Note: These are part of the Windows SDK and require no additional open-source license.
```

---

## 🚀 Getting Started

(You can add build instructions, CMake options, and examples here.)