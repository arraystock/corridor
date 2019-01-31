;
; paging.inc
;

; Page directory table.
%define   PAGE_DIR      0x9C000

; 0th page table. Address must be 4KB aligned.
%define   PAGE_TABLE_0    0x9D000

; 768th page table. Address must be 4KB aligned.
%define   PAGE_TABLE_768    0x9E000

; Each page table has 1024 entries.
%define   PAGE_TABLE_ENTRIES  1024

; Attributes (page is present;page is writable; supervisor mode).
%define   PRIV        3
