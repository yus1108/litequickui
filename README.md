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
  - Windows: Direct2D (planned), DirectWrite (planned) , DirectX 11 (planned), DirectX 12
  - Linux:  OpenGL (planned), Vulkan (planned)
  - macOS/iOS: Metal (planned)
- **Flexible text rendering**
  - Optional FreeType / HarfBuzz
  - Or use platform‑native font APIs

---

## 📦 Project Structure
```
litequickui/
 ├─ litequickui              # core + text backend + graphics backend
 ├─ litequickui-core/        # Core engine (litehtml + QuickJS)
 ├─ litequickui-dx12/        # DirectX 12 graphics backend (Windows)
 ├─ litequickui-hb-ft/       # HarfBuzz + FreeType text backend
 ├─ litequickui-test/        # Test application
 └─ external/
      ├─ litehtml/
      ├─ quickjs/
      ├─ freetype/
      ├─ harfbuzz/
```

---

## 🧩 Modules

### **LiteQuickUI Core (litequickui-core)**

The core engine depends only on:

- **litehtml** (BSD‑3‑Clause)  
- **QuickJS** (MIT)

The core module does **not** use FreeType, HarfBuzz, or any graphics API.
Therefore:

> **When using only the core module, no additional third‑party licenses apply.**

---

### **Optional Modules**

These modules introduce additional dependencies and licenses.

#### **litequickui-hb-ft**
- FreeType (FTL)
- HarfBuzz (MIT)

#### **litequickui-dx12 (Windows only)**
- DirectX 12 (Microsoft Windows SDK — no redistribution requirements)  
- FreeType (FTL)
- HarfBuzz (MIT)

Each module’s license applies **only when that module is used**.

### 📄 License

LiteQuickUI is released under the **MIT License**.

### Summary

| Module             | Dependencies                                     | License Impact |
|--------------------|--------------------------------------------------|----------------|
| litequickui        | Aggregated: core + selected text backend + selected graphics backend | ✔ Combined licenses of enabled modules |
| litequickui-core   | litehtml (BSD), QuickJS (MIT)                    | ✔ No additional obligations |
| litequickui-dx12   | DirectX 12 (Microsoft SDK), litequickui-hb-ft    | ✔ Applies when module is used |
| litequickui-hb-ft  | FreeType (FTL), HarfBuzz (MIT)                   | ✔ Applies when module is used |

See **THIRD_PARTY_LICENSES.md** for detailed information.

---

## 📚 THIRD_PARTY_LICENSES.md (Summary)

``` markdown
# Third-Party Licenses

These licenses apply only if you use the corresponding optional modules.

## litehtml (BSD-3-Clause)
Used by: litequickui-core  
License: external/litehtml/LICENSE

## QuickJS (MIT)
Used by: litequickui-core  
License: external/quickjs/LICENSE

## FreeType (FTL)
Used by: litequickui-hb-ft  
License: external/freetype/docs/FTL.TXT

## HarfBuzz (MIT)
Used by: litequickui-hb-ft  
License: external/harfbuzz/COPYING

## Microsoft DirectX 12
Used by: litequickui-dx12  
Note: These are part of the Windows SDK and require no additional open-source license.
```

---

## 🚀 Getting Started

(You can add build instructions, CMake options, and examples here.)