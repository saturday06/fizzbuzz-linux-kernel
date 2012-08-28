/*
 *  linux/fs/binfmt_fizzbuzz.c
 */

#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/binfmts.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/fs.h>

extern int fizzbuzz(void);

static int load_fizzbuzz(struct linux_binprm *bprm,struct pt_regs *regs)
{
	if (bprm->buf[0] != '\xfb') {
		return -ENOEXEC;
	}

	return fizzbuzz();
}

static struct linux_binfmt fizzbuzz_format = {
	.module		= THIS_MODULE,
	.load_binary	= load_fizzbuzz,
};

static int __init init_fizzbuzz_binfmt(void)
{
	register_binfmt(&fizzbuzz_format);
	return 0;
}

static void __exit exit_fizzbuzz_binfmt(void)
{
	unregister_binfmt(&fizzbuzz_format);
}

core_initcall(init_fizzbuzz_binfmt);
module_exit(exit_fizzbuzz_binfmt);
MODULE_LICENSE("GPL");
