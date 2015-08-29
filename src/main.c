/*
 * Copyright (C) 2015
 * Bob Mottram <bob@robotucs.uk.to>
 *
 * A command to print your tox ID to the console
 * Some functions are from ToxBot https://github.com/JFreegman/ToxBot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <tox/tox.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>

off_t file_size(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
        return 0;
    return st.st_size;
}

static Tox *load_tox(struct Tox_Options *options, char *path)
{
    FILE *fp = fopen(path, "rb");
    Tox *m = NULL;
    if (fp == NULL) {
        TOX_ERR_NEW err;
        m = tox_new(options, &err);
        if (err != TOX_ERR_NEW_OK) {
            fprintf(stderr, "tox_new failed with error %d\n", err);
            return NULL;
        }
        return m;
    }
    off_t data_len = file_size(path);
    if (data_len == 0) {
        fclose(fp);
        return NULL;
    }
    char data[data_len];
    if (fread(data, sizeof(data), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }
    TOX_ERR_NEW err;
    options->savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
    options->savedata_data = (uint8_t *) data;
    options->savedata_length = data_len;
    m = tox_new(options, &err);
    if (err != TOX_ERR_NEW_OK) {
        fprintf(stderr, "tox_new failed with error %d\n", err);
        return NULL;
    }
    fclose(fp);
    return m;
}

int main(int argc, char* argv[])
{
    char data_file[256], address[TOX_ADDRESS_SIZE];
    struct Tox_Options tox_opts;
    Tox * tox;
    int i;

    /* get the user's home directory */
	struct passwd *pw = getpwuid(getuid());
	const char *homedir =  pw->pw_dir;

    /* default location of the tox data file */
    sprintf(data_file, "%s/.config/tox/data", homedir);

    /* parse the command line */
    for (i = 1; i < argc-1; i += 2) {
        if ((strcmp(argv[i], "--user") == 0) ||
            (strcmp(argv[i], "-u") == 0)) {
            sprintf(data_file, "/home/%s/.config/tox/data", argv[i+1]);
        }
    }

    /* some default options */
    memset(&tox_opts, 0, sizeof(struct Tox_Options));
    tox_options_default(&tox_opts);

    /* load */
    tox = load_tox(&tox_opts, data_file);
    if (tox == NULL) {
        printf("Failed to load or initialise\n");
        return 1;
    }

    tox_self_get_address(tox, (uint8_t *)address);
    for (i = 0; i < TOX_ADDRESS_SIZE; ++i) {
        char d[3];
        snprintf(d, sizeof(d), "%02X", address[i] & 0xff);
        printf("%s", d);
    }
    printf("\n");

    /* exit */
    tox_kill(tox);

    return 0;
}
