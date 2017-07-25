
let g:fc_DontUseDefault = 1
if has("win32")
    set guifont=Monaco:h7.5:cANSI
    color csharp
    set columns=150 lines=99
elseif has("macunix")
    set guifont=Fira\ Mono:h12
    set columns=156 lines=99
elseif has("unix")
    set guifont=Monaco\ 9
    color csharp
    set columns=150
    set lines=99
endif
