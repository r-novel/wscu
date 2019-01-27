#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>
// #include "../include/log.h"

struct cfg_tool {
    char* name;
    char* url;
};

enum state_t {
    START,
    SECTION,
    LIST,
    VALUES,
    KEY,
    VALUE,
    STOP,
    ERROR,
};

struct state {
    enum state_t state;
    int  accepted;
    char* key;
};

void cfg_tool_free(struct cfg_tool* in) {
  if (in) {
    for (int i = 0; i < 2; ++i) {
      free(in->name);
      free(in->url);
    }
    free(in);
  } else {
    fprintf(stderr, "error with free config tool struct \n");
  }

}

int perform(struct state* st, yaml_event_t* event, struct cfg_tool* t) {
    st->accepted = 0;
    switch (st->state) {
      case START:
        switch (event->type) {
          case YAML_MAPPING_START_EVENT: st->state = SECTION; break;
          case YAML_SCALAR_EVENT:
            // log(warning, "Ignoring unexpected scalar: %s\n", event->data.scalar.value);
          break;
          case YAML_SEQUENCE_START_EVENT:
            // log(warning, "Unexpected sequence.\n");
            st->state = ERROR;
          break;
          case YAML_STREAM_END_EVENT: st->state = STOP; break;
          default: break;
        }
      break;
      case SECTION:
        switch (event->type) {
          case YAML_SCALAR_EVENT:
            if (strcmp((char*)event->data.scalar.value, "tools") == 0) {
              st->state = LIST;
            }   else {
              fprintf(stderr, "Unexpected scalar: %s\n", (char*)event->data.scalar.value);
              st->state = ERROR;
            }
          break;
          default:
            fprintf(stderr, "Unexpected event while getting scalar: %d\n", event->type);
            st->state = ERROR;
          break;
        }
      break;
      case LIST:
        switch (event->type) {
          case YAML_SEQUENCE_START_EVENT: st->state = VALUES; break;
          default:
              fprintf(stderr, "Unexpected event while getting sequence: %d\n", event->type);
              st->state = ERROR;
          break;
        }
      break;
      case VALUES:
        switch (event->type) {
          case YAML_MAPPING_START_EVENT:
            memset(t, 0, sizeof(t));
            st->state = KEY;
          break;
          case YAML_SEQUENCE_END_EVENT: 
            st->state = START; 
          break;
          case YAML_DOCUMENT_END_EVENT:
            st->state = START; 
          break;
          default:
            // log(warning, "Unexpected event while getting mapped values: %d\n", event->type);  
            st->state = ERROR;
          break;
        }
      break;
      case KEY:
        switch (event->type) {
          case YAML_SCALAR_EVENT:
            st->key = strdup((char*)event->data.scalar.value);
            st->state = VALUE;
          break;
          case YAML_MAPPING_END_EVENT:
            st->accepted = 1;
            st->state = VALUES;
          break;
          default:
            // log(warning, "Unexpected event while getting key: %d\n", event->type);
            st->state = ERROR;
          break;
        }
      break;
      case VALUE:
        switch (event->type) {
          case YAML_SCALAR_EVENT:
            if (strcmp(st->key, "name") == 0) {
              t->name = strdup((char*)event->data.scalar.value);
            } else if (strcmp(st->key, "url") == 0) {
              t->url = strdup((char*)event->data.scalar.value);
            } else {
              fprintf(stderr, "Ignoring unknown key: %s\n", st->key);
            }
            free(st->key);
            st->state = KEY;
          break;
          default:
            fprintf(stderr, "Unexpected event while getting value: %d\n",
                    event->type);
            st->state = ERROR;
          break;
        }
      break;
      case ERROR: case STOP: break;
    }
      
  return (st->state == ERROR ? 0 : 1);
}

int tool(struct cfg_tool tools[]) {
    yaml_parser_t parser;
    yaml_event_t event;
    struct state st = { .state = START, .accepted = 0 };
    //TODO: create reallocate; 
    // struct cfg_tool data[2];
    // struct cfg_tool* out;

    if(!yaml_parser_initialize(&parser)) 
      fprintf(stderr, "error with yaml parse init \n");

    yaml_parser_set_input_file(&parser, stdin);

    int i = 0;
    do {
        if (!yaml_parser_parse(&parser, &event)) {
            fprintf(stderr, "error with yaml parser \n");
            yaml_parser_delete(&parser);
            return 0;
        }

        if (!perform(&st, &event, &tools[i])) {
            fprintf(stderr, "error with perform yaml event;\n");
            yaml_parser_delete(&parser);
            return 0;
        }
        if (st.accepted) {
          // data = realloc(data, i * sizeof(data));
          // printf("Size %d\n", sizeof(data)/sizeof(*data));
          // printf("data[%d]={name=%s, url=%s}\n", i, data[i].name, data[i].url);
          i++;
        }
        yaml_event_delete(&event);
    } while (st.state != STOP);
    // out = data;
    yaml_parser_delete(&parser);
    // printf("~~~~~~>res\n");
    // printf("DATA %d\n", sizeof(data)/sizeof(*data));

    // for (int j = 0; j < sizeof(data)/sizeof(*data); ++j) {
    //   printf("data[%d]{name: %s url: %s}\n", j, data[j].name, data[j].url);
    // }
    
    return 1;
}

int main(void) {
  struct cfg_tool res[2];
  int ok = tool(res);
  if (ok) {
    for (int i = 0; i < 2; ++i) {
      printf("Tool struct: res[%d]{ name: %s, url: %s }\n", i, res[i].name, res[i].url);
      // cfg_tool_free(res + i);

    }
  } else {
    fprintf(stderr, "error with get tools from config;\n");
  }

  // cfg_tool_free(res);
}