#pragma once

#include "redland.h"

#include <iostream>
#include <cstring>

void print_all_statements(librdf_model* model);

void add_new_statement(librdf_world* world, librdf_model* model, librdf_node* subject,
                       const unsigned char* predicateUri, const unsigned char* objectLiteral);

void apply_common_rules(librdf_world* world, librdf_model* model);
void apply_profession_rules(librdf_world* world, librdf_model* model);
void apply_life_rules(librdf_world* world, librdf_model* model);