;
; paging.s
;

%include "boot/paging.inc"

global enable_paging
enable_paging:
  pusha                         ; Save stack frame.

  ;------------------------------------------
  ; Idenitity map 1st page table (4MB).
  ;------------------------------------------

  mov   eax, PAGE_TABLE_0       ; First page table.
  mov   ebx, 0x0 | PRIV         ; Starting physical address of page.
  mov   ecx, PAGE_TABLE_ENTRIES ; For every page in table...
.loop:
  mov   dword [eax], ebx        ; Write the entry.
  add   eax, 4                  ; Go to next page entry in table (Each are 4 bytes).
  add   ebx, 4096               ; Go to next page address (Each page is 4Kb).
  loop  .loop                   ; Go to next entry.

  ;------------------------------------------
  ; Set up entries in the directory table.
  ;------------------------------------------

  mov   eax, PAGE_TABLE_0 | PRIV        ; 1st table is directory entry 0.
  mov   dword [PAGE_DIR], eax

  mov   eax, PAGE_TABLE_768 | PRIV      ; 768th entry in directory table.
  mov   dword [PAGE_DIR+(768*4)], eax

  ;------------------------------------------
  ; Install the directory table.
  ;------------------------------------------

  mov   eax, PAGE_DIR
  mov   cr3, eax

  ;------------------------------------------
  ; Enable paging.
  ;------------------------------------------

  mov   eax, cr0
  or    eax, 0x80000000
  mov   cr0, eax

  ;------------------------------------------
  ; Map the 768th table to physical addr 1MB.
  ; 768th table starts the 3GB virtual addr.
  ;------------------------------------------

  mov   eax, PAGE_TABLE_768     ; First page table.
  mov   ebx, 0x100000 | PRIV    ; Starting physical address of page.
  mov   ecx, PAGE_TABLE_ENTRIES ; For every page in table...
.loop2:
  mov   dword [eax], ebx        ; Write the entry.
  add   eax, 4                  ; Go to next page entry in table (Each are 4 bytes).
  add   ebx, 4096               ; Go to next page address (Each page is 4Kb).
  loop  .loop2                  ; Go to next entry.

  popa
  ret
