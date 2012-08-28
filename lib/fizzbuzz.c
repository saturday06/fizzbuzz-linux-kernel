#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/syscalls.h>

#define FIZZBUZZ_BUFFER_SIZE 128

int fizzbuzz(void) {
	int fizzbuzz;
	int written;
	char __user *output;
	int munmap_ret;

	/* simulates user operation */
	down_write(&current->mm->mmap_sem);
	output = (char __user *)do_mmap_pgoff(NULL, 0, FIZZBUZZ_BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, 0);
	up_write(&current->mm->mmap_sem);

	if (!output) {
		printk("fizzbuzz: do_mmap failed\n");
		return -ENOMEM;
	}

	/* output fizzbuzz result */
	for (fizzbuzz = 0; fizzbuzz < 100; ++fizzbuzz) {
		char koutput[FIZZBUZZ_BUFFER_SIZE];
		if (fizzbuzz % 5 == 0 && fizzbuzz % 3 == 0) {
			scnprintf(koutput, sizeof(koutput), "FizzBuzz\n");
		} else if (fizzbuzz % 5 == 0) {
			scnprintf(koutput, sizeof(koutput), "Fizz\n");
		} else if (fizzbuzz % 3 == 0) {
			scnprintf(koutput, sizeof(koutput), "Buzz\n");
		} else {
			scnprintf(koutput, sizeof(koutput), "%d\n", fizzbuzz);
		}
		if (copy_to_user(output, koutput, FIZZBUZZ_BUFFER_SIZE) == 0) {
			written = sys_write(1, output, strlen(koutput));
		} else {
			printk("fizzbuzz: copy_to_user failed 0x%p <- 0x%p\n", output, koutput);
			written = 0;
		}
		printk("%d %s", written,  output);
	}

	down_write(&current->mm->mmap_sem);
	munmap_ret = do_munmap(current->mm, (unsigned long)output, FIZZBUZZ_BUFFER_SIZE);
	up_write(&current->mm->mmap_sem);
	printk("fizzbuzz: munmap=%d\n", munmap_ret);
	return 0;
}

