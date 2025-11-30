#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xe1e1f979, "_raw_spin_lock_irqsave" },
	{ 0x81a1a811, "_raw_spin_unlock_irqrestore" },
	{ 0x40a621c5, "snprintf" },
	{ 0xf64ac983, "__copy_overflow" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xcb8b6ec6, "kfree" },
	{ 0x759ba330, "remove_proc_entry" },
	{ 0xbd03ed67, "random_kmalloc_seed" },
	{ 0xfc961df9, "kmalloc_caches" },
	{ 0xe18fddbc, "__kmalloc_cache_noprof" },
	{ 0xc609ff70, "strncpy" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_from_user" },
	{ 0x173ec8da, "sscanf" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xf78bcd86, "default_llseek" },
	{ 0xd272d446, "__fentry__" },
	{ 0x008d4a19, "proc_create" },
	{ 0xe8213e80, "_printk" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xd268ca91, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0xe1e1f979,
	0x81a1a811,
	0x40a621c5,
	0xf64ac983,
	0xd272d446,
	0xcb8b6ec6,
	0x759ba330,
	0xbd03ed67,
	0xfc961df9,
	0xe18fddbc,
	0xc609ff70,
	0xa61fd7aa,
	0x092a35a2,
	0x173ec8da,
	0x90a48d82,
	0xf78bcd86,
	0xd272d446,
	0x008d4a19,
	0xe8213e80,
	0xd272d446,
	0xd268ca91,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"_raw_spin_lock_irqsave\0"
	"_raw_spin_unlock_irqrestore\0"
	"snprintf\0"
	"__copy_overflow\0"
	"__stack_chk_fail\0"
	"kfree\0"
	"remove_proc_entry\0"
	"random_kmalloc_seed\0"
	"kmalloc_caches\0"
	"__kmalloc_cache_noprof\0"
	"strncpy\0"
	"__check_object_size\0"
	"_copy_from_user\0"
	"sscanf\0"
	"__ubsan_handle_out_of_bounds\0"
	"default_llseek\0"
	"__fentry__\0"
	"proc_create\0"
	"_printk\0"
	"__x86_return_thunk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "61BFE28B5802A4B395890BD");
