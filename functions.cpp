#include "functions.h"

void print_all_statements(librdf_model* model) {
    librdf_stream *stream = librdf_model_as_stream(model);
    while (!librdf_stream_end(stream)) {
        librdf_statement *statement = librdf_stream_get_object(stream);
        librdf_node *subject = librdf_statement_get_subject(statement);
        librdf_node *predicate = librdf_statement_get_predicate(statement);
        librdf_node *object = librdf_statement_get_object(statement);

        librdf_node_print(subject, stdout);
        std::cout << " ";
        librdf_node_print(predicate, stdout);
        std::cout << " ";
        librdf_node_print(object, stdout);
        std::cout << "\n";

        librdf_stream_next(stream);
    }
    librdf_free_stream(stream);
}

void add_new_statement(librdf_world* world, librdf_model* model, librdf_node* subject, const unsigned char* predicateUri, const unsigned char* objectLiteral) {
    librdf_statement* newStatement = librdf_new_statement_from_nodes(
            world,
            librdf_new_node_from_node(subject),
            librdf_new_node_from_uri_string(world, predicateUri),
            librdf_new_node_from_literal(world, objectLiteral, nullptr, 0)
    );
    librdf_model_add_statement(model, newStatement);
    librdf_free_statement(newStatement);
}

void apply_common_rules(librdf_world* world, librdf_model* model) {
    librdf_stream* stream = librdf_model_as_stream(model);
    while (!librdf_stream_end(stream)) {
        librdf_statement* statement = librdf_stream_get_object(stream);

        librdf_node* subject = librdf_statement_get_subject(statement);
        librdf_node* predicate = librdf_statement_get_predicate(statement);
        librdf_node* object = librdf_statement_get_object(statement);

        if (librdf_node_is_literal(object) && strcmp((const char*)librdf_node_get_literal_value(object), "City") == 0) {
            add_new_statement(world, model, subject, (const unsigned char*)"https://example.org/isCitizen", (const unsigned char*)"Citizen");
        }

        librdf_uri* predicate_uri = librdf_node_get_uri(predicate);
        const char* predicate_uri_str = (const char*) librdf_uri_as_string(predicate_uri);

        if (librdf_node_is_resource(predicate) && strcmp(predicate_uri_str, "https://example.org/age") == 0)
        {
            int age = std::stoi((const char*)librdf_node_get_literal_value(object));
            if (age > 18) {
                add_new_statement(world, model, subject, (const unsigned char*)"https://example.org/isAdult", (const unsigned char*)"Adult");
            }
        }

        librdf_stream_next(stream);
    }
    librdf_free_stream(stream);
}

void apply_profession_rules(librdf_world* world, librdf_model* model) {
    librdf_stream* stream = librdf_model_as_stream(model);
    while (!librdf_stream_end(stream)) {
        librdf_statement* statement = librdf_stream_get_object(stream);

        librdf_node* subject = librdf_statement_get_subject(statement);
        librdf_node* predicate = librdf_statement_get_predicate(statement);
        librdf_node* object = librdf_statement_get_object(statement);

        librdf_uri* predicate_uri = librdf_node_get_uri(predicate);
        const char* predicate_uri_str = (const char*) librdf_uri_as_string(predicate_uri);

        if (librdf_node_is_resource(predicate) && strcmp(predicate_uri_str, "https://example.org/profession") == 0)
        {
            std::string profession = (const char*)librdf_node_get_literal_value(object);
            const auto* canDo = reinterpret_cast<const unsigned char*>("https://example.org/canDo");
            if (profession == "Doctor") {
                add_new_statement(world, model, subject, canDo, (const unsigned char*)"Treating patients");
            } else if (profession == "Engineer") {
                add_new_statement(world, model, subject, canDo, (const unsigned char*)"Building structures");
            }
        }

        librdf_stream_next(stream);
    }
    librdf_free_stream(stream);
}

void apply_life_rules(librdf_world* world, librdf_model* model) {
    librdf_stream* stream = librdf_model_as_stream(model);
    while (!librdf_stream_end(stream)) {
        librdf_statement* statement = librdf_stream_get_object(stream);

        librdf_node* subject = librdf_statement_get_subject(statement);
        librdf_node* predicate = librdf_statement_get_predicate(statement);
        librdf_node* object = librdf_statement_get_object(statement);

        librdf_uri* predicate_uri = librdf_node_get_uri(predicate);
        const char* predicate_uri_str = (const char*) librdf_uri_as_string(predicate_uri);

        if (librdf_node_is_resource(predicate) && strcmp(predicate_uri_str, "https://example.org/age") == 0)
        {
            int age = std::stoi((const char*)librdf_node_get_literal_value(object));
            if (age > 18) {
                add_new_statement(world, model, subject, (const unsigned char*)"https://example.org/isAdult", (const unsigned char*)"Adult");
            }
        }

        if (librdf_node_is_resource(predicate) && strcmp(predicate_uri_str, "https://example.org/education") == 0)
        {
            std::string scienceDegree = (const char*)librdf_node_get_literal_value(object);
            if (scienceDegree == "PhD") {
                add_new_statement(world, model, subject, (const unsigned char*)"https://example.org/isScientist", (const unsigned char*)"Scientist");
            }
        }

        if (librdf_node_is_resource(predicate) && strcmp(predicate_uri_str, "https://example.org/hobby") == 0)
        {
            std::string hobby = (const char*)librdf_node_get_literal_value(object);
            if (hobby == "Guitar") {
                add_new_statement(world, model, subject, (const unsigned char*)"https://example.org/isMusician", (const unsigned char*)"Guitarist");
            }
        }

        if (librdf_node_is_resource(predicate) && strcmp((const char*)librdf_uri_as_string(librdf_node_get_uri(predicate)), "https://example.org/friend") == 0) {
            librdf_statement* query = librdf_new_statement_from_nodes(world,
                                                                      librdf_new_node_from_node(object),
                                                                      librdf_new_node_from_uri_string(world, (const unsigned char*)"https://example.org/hobby"),
                                                                      nullptr
            );
            librdf_stream* results = librdf_model_find_statements(model, query);
            if (!librdf_stream_end(results)) {
                librdf_statement* result = librdf_stream_get_object(results);
                librdf_node* sharedHobby = librdf_statement_get_object(result);

                librdf_statement* newStatement = librdf_new_statement_from_nodes(
                        world,
                        librdf_new_node_from_node(subject),
                        librdf_new_node_from_uri_string(world, (const unsigned char*)"https://example.org/likelyToLike"),
                        librdf_new_node_from_node(sharedHobby)
                );
                librdf_model_add_statement(model, newStatement);
                librdf_free_statement(newStatement);
            }
            librdf_free_stream(results);
            librdf_free_statement(query);
        }

        librdf_stream_next(stream);
    }
    librdf_free_stream(stream);
}