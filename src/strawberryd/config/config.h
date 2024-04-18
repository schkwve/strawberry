#pragma once

struct strawberry_config {
	// absolute file path
	char *config_file;

	////
	// Resource limits
	///
	int max_mem;
};

void config_load_config(void);

void config_set_config_path(char *filepath);
void config_set_mem_limit(int limit);
