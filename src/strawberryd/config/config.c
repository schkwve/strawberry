#include <config/config.h>

struct strawberry_config config = {
	// default configuration path
	.config_file = "/etc/strawberry/strawberryd.conf",

	// Set the RAM limit to 4GB
	.max_mem = 4096
};

void config_load_config(void)
{
}

void config_set_config_path(char *filepath)
{
	config.config_file = filepath;
}

void config_set_mem_limit(int limit)
{
	config.max_mem = limit;
}
