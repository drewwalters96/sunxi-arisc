#define _XOPEN_SOURCE 700

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define SRAM_ADDR 0x00040000
#define SRAM_SIZE 0x00018000

#define PRCM_ADDR 0x01f01000
#define PRCM_SIZE 0x00001000

#define MAX_FW_SIZE 0x6000
#define MIN_FW_SIZE 0x4000

#define readl(addr) (*((volatile uint32_t *) (addr)))
#define writel(addr, v) (*((volatile uint32_t *) (addr)) = (v))

int
main(int argc, char *argv[])
{
	struct stat st;
	struct timespec ts = { .tv_sec = 0, .tv_nsec = 10000000 };
	int fd, memfd;
	unsigned char *file, *mem, *prcm;
	
	if (argc < 2) {
		printf("usage: %s <filename>\n", argv[0]);
		return EINVAL;
	}

	memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if (memfd < 0) {
		perror("failed to open /dev/mem");
		return errno;
	}
	mem = mmap(NULL, SRAM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, SRAM_ADDR);
	if (mem == MAP_FAILED) {
		perror("failed to mmap /dev/mem");
		return errno;
	}
	prcm = mmap(NULL, PRCM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, PRCM_ADDR);
	if (prcm == MAP_FAILED) {
		perror("failed to mmap arisc prcm");
		return errno;
	}
	close(memfd);

	if (!strcmp(argv[1], "--reset")) {
		printf("asserting arisc reset\n");
		writel(prcm + 0x0c00, readl(prcm + 0x0c00) & ~1U);
		return 0;
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("failed to open firmware image");
		return errno;
	}
	if (fstat(fd, &st)) {
		perror("failed to stat firmware image");
		return errno;
	}
	if (st.st_size < MIN_FW_SIZE || st.st_size > MAX_FW_SIZE) {
		printf("firmware image has wrong size");
		return EINVAL;
	}
	file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (file == MAP_FAILED) {
		perror("failed to mmap firmware image");
		return errno;
	}
	close(fd);

	printf("asserting arisc reset\n");
	writel(prcm + 0x0c00, readl(prcm + 0x0c00) & ~1U);
	nanosleep(&ts, NULL);

	printf("writing firmware (%ld bytes)\n", st.st_size);
	memcpy(mem, file, st.st_size);
	msync(mem, st.st_size, MS_SYNC);

	printf("deasserting arisc reset\n");
	nanosleep(&ts, NULL);
	writel(prcm + 0x0c00, readl(prcm + 0x0c00) | 1U);

	munmap(file, st.st_size);
	munmap(mem, SRAM_SIZE);
	munmap(prcm, PRCM_SIZE);

	return 0;
}
