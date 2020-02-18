#include <myler_utils.h>
#include <myler_console.h>
#include <myler_ui.h>

int main(int argc, char *argv[])
{
    init_myler_utils(argc, argv);

    init_ui();

    while (true) {
        update_ui(false);
        
        if (get_key() == 'Q')
            break;

        myler_delay_ms(100);
    }
    
    free_ui();

    myler_printf("Myler 命令行音乐播放器 V3.0\n");
    
    return 0;
}