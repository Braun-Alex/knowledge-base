#include <librdf.h>

int main() {
    librdf_world* world = librdf_new_world();
    librdf_world_open(world);
    librdf_free_world(world);
    return 0;
}