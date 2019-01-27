#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>
#include "cfg.h"

int perform(struct state* st, yaml_event_t* event, struct cfg_tool* t) {
    st->accepted = 0;
    switch (st->state) {
      case START:
        switch (event->type) {
          case YAML_MAPPING_START_EVENT: st->state = SECTION; break;
          case YAML_SCALAR_EVENT:
            log(warning, "Ignoring unexpected scalar: %s\n", event->data.scalar.value);
          break;
          case YAML_SEQUENCE_START_EVENT:
            log(warning, "Unexpected sequence.\n");
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
              log(warning, "Unexpected scalar: %s\n", (char*)event->data.scalar.value);
              st->state = ERROR;
            }
          break;
          default:
            log(warning, "Unexpected event while getting scalar: %d\n", event->type);
            st->state = ERROR;
          break;
        }
      break;
      case LIST:
        switch (event->type) {
          case YAML_SEQUENCE_START_EVENT: st->state = VALUES; break;
          default:
              log(warning, "Unexpected event while getting sequence: %d\n", event->type);
              st->state = ERROR;
          break;
        }
      break;
      case VALUES:
        switch (event->type) {
          case YAML_MAPPING_START_EVENT:
            memset(t, 0, sizeof(*t));
            st->state = KEY;
          break;
          case YAML_SEQUENCE_END_EVENT: 
            st->state = START; 
          break;
          case YAML_DOCUMENT_END_EVENT:
            st->state = START; 
          break;
          default:
            log(warning, "Unexpected event while getting mapped values: %d\n", event->type);  
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
            log(warning, "Unexpected event while getting key: %d\n", event->type);
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
              log(warning, "Ignoring unknown key: %s\n", st->key);
            }
            free(st->key);
            st->state = KEY;
          break;
          default:
            log(warning, "Unexpected event while getting value: %d\n",event->type);
            st->state = ERROR;
          break;
        }
      break;
      case ERROR: case STOP: break;
    }
      
  return (st->state == ERROR ? 0 : 1);
}

int cfg_tool(struct cfg_tool tools[]) {
    yaml_parser_t parser;
    yaml_event_t event;
    struct state st = { .state = START, .accepted = 0 };
   
    if(!yaml_parser_initialize(&parser)) 
      log(error, "error with yaml parse init;\n");

    yaml_parser_set_input_file(&parser, stdin);

    int i = 0;
    do {
        if (!yaml_parser_parse(&parser, &event)) {
            log(error, "error with yaml parser;\n");
            yaml_parser_delete(&parser);
            return 0;
        }

        if (!perform(&st, &event, &tools[i])) {
            log(error, "error with perform yaml event;\n");
            yaml_parser_delete(&parser);
            return 0;
        }
        if (st.accepted)
          i++;
        yaml_event_delete(&event);
    } while (st.state != STOP);

    yaml_parser_delete(&parser);
    return 1;
}

void cfg_tool_free(struct cfg_tool* in) {
  if (in) {
    free(in->name);
    free(in->url);
  } else {
    log(error, "error with free config tool struct \n");
  }

}

void test(void) {
  struct cfg_tool res[2];
  int ok = cfg_tool(res);
  if (ok) {
    for (int i = 0; i < 2; ++i) {
      log(trace, "Tool struct: res[%d]{ name: %s, url: %s }\n", i, res[i].name, res[i].url);
      cfg_tool_free(&res[i]);
    }
  } else
    log(error, "error with get tools from config;\n");
}