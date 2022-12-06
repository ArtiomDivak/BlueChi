#include "../common/dbus.h"
#include "../ini/config.h"
#include "../ini/ini.h"
#include "controller.h"
#include "opt.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
        fprintf(stdout, "Hello from orchestrator!\n");

        struct hashmap *ini_hashmap _cleanup_hashmap_ = NULL;

        int port = 0;
        _cleanup_free_ char *configPath = NULL;

        get_opts(argc, argv, &port, &configPath);

        _cleanup_sd_event_ sd_event *event = NULL;
        int r = sd_event_default(&event);
        if (r < 0) {
                fprintf(stderr, "Failed to create event: %s\n", strerror(-r));
                return EXIT_FAILURE;
        }

        _cleanup_fd_ int accept_fd = -1;
        _cleanup_sd_event_source_ sd_event_source *event_source = NULL;
        r = controller_setup(accept_fd, port, event, event_source);
        if (r < 0) {
                fprintf(stderr, "Failed to setup controller\n");
                return EXIT_FAILURE;
        }

        ini_hashmap = parsing_ini_file(configPath);
        if (ini_hashmap != NULL) {
                return EXIT_FAILURE;
        }

        r = sd_event_loop(event);
        if (r < 0) {
                fprintf(stderr, "Event loop failed: %s\n", strerror(-r));
                return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
}