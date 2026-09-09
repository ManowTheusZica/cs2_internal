# PRISM CS2 Internal - Inventory Changer

Versão interna do PRISM com DLL injection e InventoryChanger real.

## O que é diferente da versão externa

| Feature | Externa (CS2Cheat.cs) | Interna (cs2_internal) |
|---------|----------------------|------------------------|
| Tipo | .exe externo | .dll injetada |
| Leitura memória | ReadProcessMemory | Acesso direto |
| SkinChanger | Visual apenas | Visual + Inventário |
| InventoryChanger | ❌ | ✅ |
| Hooks | ❌ | ✅ (SOCacheSubscribed) |
| Menu in-game | Overlay GDI+ | Pode usar ImGui |
| Detecção | Mais fácil | Mais difícil |

## Requisitos

- **Visual Studio 2022** (com workload C++ Desktop)
- **Windows x64**
- **.NET 6+** (para a versão externa loader)

## Como compilar

### Opção 1: Visual Studio
1. Abra `cs2_internal.sln` no Visual Studio
2. Selecione **Release x64**
3. Build → Build Solution (Ctrl+Shift+B)

### Opção 2: Command Prompt
```
cd cs2_internal
BUILD.bat
```

## Como usar

1. Abra o CS2 com `-insecure` (modo offline com bots)
2. Rode o injetor como Administrador:
```
injector.exe cs2_cheat.dll
```
3. No jogo, pressione **]** para abrir o menu
4. Configure as skins no menu

## Estrutura

```
cs2_internal/
├── dllmain.cpp              # Entry point da DLL
├── injector.cpp             # Injetor de DLL
├── cs2_internal.vcxproj     # Projeto Visual Studio
├── BUILD.bat                # Script de build
├── Core/
│   ├── Memory.h             # Leitura/escrita de memória
│   └── Offsets.h            # Offsets dinâmicos (cs2-dumper)
├── Features/
│   └── InventoryChanger.h   # Inventory Changer
└── Hooks/
    └── Hooks.h              # Sistema de hooks
```

## Arquitetura

```
injector.exe ──CreateRemoteThread──> cs2.exe
                                          │
                                          ▼
                                   cs2_cheat.dll
                                          │
                    ┌─────────────────────┼─────────────────────┐
                    ▼                     ▼                     ▼
              Memory Module        Hooks Module        InventoryChanger
              (ReadProcessMemory)  (SOCacheSubscribed) (Virtual Items)
```

## Segurança

- **Apenas offline**: Use com `-insecure` e bots
- **Sem VAC**: Não use online — ban permanente
- **Admin required**: DLL injection precisa de privilégios de admin

## TODO

- [ ] Implementar hook real do SOCacheSubscribed
- [ ] Implementar hook do EquipItemInLoadout  
- [ ] Adicionar ImGui para menu in-game
- [ ] Implementar FauxItemId encoding (Osiris-style)
- [ ] Adicionar auto-download de offsets
- [ ] Testar com CS2 atual
