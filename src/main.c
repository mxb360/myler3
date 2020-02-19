#include <myler.h>

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

    list_t *list = create_list_from_path("MUSIC", "/home/mxb/Music");
    if (list) {
        list_node_t *node = list->head.next;
        while (node) {
            printf("  %s\n", node->song->file_info.filename);
            node = node->next;
        }
    }

    free_list(list);
    
    return 0;
}
