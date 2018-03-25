#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int		main(void)
{
	struct stat buf;
	
	lstat("link", &buf);
	printf("%lld\n", buf.st_size);

	lstat("llink", &buf);
	printf("%lld\n", buf.st_size);
	//readlink("link", buf, 20);
	//write(1, buf, 20);
	return 0;
}

