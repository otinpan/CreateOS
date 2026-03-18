bits 64
section .text

global IoOut32 ; void IoOut32(uint16_t addr,uint32_t data);
IoOut32:
    mov dx, di
    mov eax, esi
    out dx, eax
    ret

global IoIn32 ; uint32_t IoIn(uint16_t addr);
IoIn32:
    mov dx, di
    in eax, dx
    ret

global GetCS ; uint16_t GetCS(void)
GetCS:
    xor eax, eax
    mov ax, cs
    ret

global LoadIDT ; void LoadIDT(uint16_t limit, uint64_t offset)
LoadIDT:
    push rbp
    mov rbp, rsp
    sub rsp, 10
    mov [rsp], di
    mov [rsp+2], rsi ; offset (idtの先頭アドレス)
    lidt [rsp]
    mov rsp, rbp
    pop rbp
    ret
