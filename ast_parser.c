#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

void countIfStatements(cJSON *node, int *if_count) {
    if (node == NULL) return;

    if (cJSON_IsObject(node)) {
        cJSON *type = cJSON_GetObjectItem(node, "_nodetype");

        if (cJSON_IsString(type) && strcmp(type->valuestring, "If") == 0) {
            (*if_count)++;
        }

        cJSON *child = node->child;
        while (child) {
            countIfStatements(child, if_count);
            child = child->next;
        }
    } else if (cJSON_IsArray(node)) {
        int size = cJSON_GetArraySize(node);
        for (int i = 0; i < size; ++i) {
            countIfStatements(cJSON_GetArrayItem(node, i), if_count);
        }
    }
}

void parseFunction(cJSON *func) {
    int if_count = 0;

    cJSON *name = cJSON_GetObjectItem(func, "name");
    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("Function Name: %s\n", name->valuestring);
    }

    cJSON *type = cJSON_GetObjectItem(cJSON_GetObjectItem(func, "type"), "type");
    if (type) {
        cJSON *returnType = cJSON_GetObjectItem(type, "names");
        if (cJSON_IsArray(returnType)) {
            printf("Return Type: %s\n", cJSON_GetArrayItem(returnType, 0)->valuestring);
        }
    }

    cJSON *params = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(func, "type"), "args"), "params");
    int param_count = cJSON_GetArraySize(params);
    printf("Number of parameters: %d\n", param_count);
    for (int i = 0; i < param_count; i++) {
        cJSON *param = cJSON_GetArrayItem(params, i);
        cJSON *paramType = cJSON_GetObjectItem(cJSON_GetObjectItem(cJSON_GetObjectItem(param, "type"), "type"), "names");
        cJSON *paramName = cJSON_GetObjectItem(cJSON_GetObjectItem(param, "type"), "declname");
        printf("Param %d type: %s, name: %s\n", i, cJSON_GetArrayItem(paramType, 0)->valuestring, paramName->valuestring);
    }

    countIfStatements(func, &if_count);

    printf("Number of if statements: %d\n\n", if_count);
}

void parseAST(const char *filename) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *json_data = malloc(length + 1);
    fread(json_data, 1, length, f);
    fclose(f);

    cJSON *ast = cJSON_Parse(json_data);
    if (ast == NULL) {
        goto end;
    }

    cJSON *ext = cJSON_GetObjectItem(ast, "ext");
    if (ext == NULL) {
        goto end;
    }

    int func_count = 0;
    cJSON *node = NULL;
    cJSON_ArrayForEach(node, ext) {
        cJSON *funcDecl = cJSON_GetObjectItem(node, "type");
        if (cJSON_IsObject(funcDecl) && cJSON_HasObjectItem(funcDecl, "args")) {
            func_count++;
            parseFunction(node);
        }
    }

    printf("Total functions: %d\n", func_count);

end:
    cJSON_Delete(ast);
    free(json_data);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    parseAST(argv[1]);
    return 0;
}
