#include "header.h"

int main(int argc, char *argv[]) {

    if (argc != 4) {
        print_help();
        return 1;
    }

    kont_svob_gr(argv[1], argv[2], atoi(argv[3]));

    return 0;
}