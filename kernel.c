typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;

extern char __bss[],__bss_end[],__stack_top[];

// メモリ領域bufの先頭からnバイトをすべてcにする
void *memset(void *buf,char c,size_t n){
    uint8_t *p=(uint8_t *)buf;
    while(n--)
        *p++=c;
    return buf;
}


void kernel_main(void){
    memset(__bss,0,(size_t)__bss_end-(size_t) __bss);

    for(;;);
}

// boot関数をtextセクションに配置
__attribute__((section(".text.boot")))
// 関数のprplogue/epilogueを自動生成しないようにする
__attribute__((naked))
void boot(void){
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" //sp=__stack__top
        "j kernel_main\n" //kernel_mainにジャンプ
        :
        : [stack_top] "r" (__stack_top)
    );
}




