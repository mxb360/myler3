#include <myler_utils.h>
#include <myler_console.h>
#include <myler_ui.h>

int main(void)
{
    init_ui();

    update_ui();
    getchar();
    free_ui();

    console_printf("hello, world: 我是汉字\n");
    
    return 0;
}
