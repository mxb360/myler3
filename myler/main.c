#include <myler_utils.h>
#include <myler_ui.h>

int main(void)
{
    init_ui();

    set_ui_title("Myler 音乐播放器 V3.0");

    update_ui();
    getchar();
    free_ui();
    return 0;
}
