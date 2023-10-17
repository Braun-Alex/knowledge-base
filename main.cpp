#include "functions.h"

int main() {
    librdf_world* world = librdf_new_world();
    librdf_world_open(world);

    librdf_storage* storage = librdf_new_storage(world, "memory", "test", nullptr);
    if (!storage) {
        std::cerr << "Не вдалося створити сховище." << "\n";
        return 1;
    }

    librdf_model* model = librdf_new_model(world, storage, nullptr);
    if (!model) {
        std::cerr << "Не вдалося створити модель." << "\n";
        return 1;
    }

    librdf_parser* parser = librdf_new_parser(world, "rdfxml", nullptr, nullptr);
    if (!parser) {
        std::cerr << "Не вдалося створити парсер." << "\n";
        return 1;
    }

    librdf_uri* uri = librdf_new_uri(world, (const unsigned char*)"file:///home/alex_braun/Projects/knowledge-base/cmake-build-debug/facts.rdf");
    if (!uri) {
        std::cerr << "Не вдалося створити URI." << "\n";
        return 1;
    }

    if (librdf_parser_parse_into_model(parser, uri, nullptr, model)) {
        std::cerr << "Не вдалося десеріалізувати дані RDF." << "\n";
        return 1;
    }

    librdf_free_uri(uri);
    librdf_free_parser(parser);

    std::cout << "Вихідні факти:\n";
    print_all_statements(model);

    std::cout << "Факти після застосування всіх правил:\n";
    apply_common_rules(world, model);
    apply_profession_rules(world, model);
    apply_life_rules(world, model);
    print_all_statements(model);

    librdf_free_model(model);
    librdf_free_storage(storage);
    librdf_free_world(world);

    return 0;
}
