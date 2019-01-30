#include "config.h"
#include "log.h"

int perform_generate(struct cfg_tool tools[]) {
  yaml_emitter_t emitter;
  yaml_event_t event;
  struct cfg_tool* t;

  yaml_emitter_initialize(&emitter);
  yaml_emitter_set_output_file(&emitter, stdout);

  void _detach(yaml_emitter_t em, yaml_event_t event) {
    log(error, "Failed to emit event %d: %s\n", event.type, em.problem);
    yaml_emitter_delete(&em);
  }

  yaml_stream_start_event_initialize(&event, YAML_UTF8_ENCODING);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }

  yaml_document_start_event_initialize(&event, NULL, NULL, NULL, 0);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }

  yaml_mapping_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_MAP_TAG,
      1, YAML_ANY_MAPPING_STYLE);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }
  yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
      (yaml_char_t *)"tools", strlen("tools"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }
  yaml_sequence_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_SEQ_TAG,
     1, YAML_ANY_SEQUENCE_STYLE);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }
  for (t = tools; t->name; t++) {
    yaml_mapping_start_event_initialize(&event, NULL, (yaml_char_t *)YAML_MAP_TAG,
      1, YAML_ANY_MAPPING_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) {
      _detach(emitter, event);
      return 1;
    }
    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
      (yaml_char_t *)"name", strlen("name"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) {
      _detach(emitter, event);
      return 1;
    }
    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
      (yaml_char_t *)t->name, strlen(t->name), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) {
      _detach(emitter, event);
      return 1;
    }

    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
      (yaml_char_t *)"url", strlen("url"), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) {
      _detach(emitter, event);
      return 1;
    }

    yaml_scalar_event_initialize(&event, NULL, (yaml_char_t *)YAML_STR_TAG,
      (yaml_char_t *)t->url, strlen(t->url), 1, 0, YAML_PLAIN_SCALAR_STYLE);
    if (!yaml_emitter_emit(&emitter, &event)) {
      _detach(emitter, event);
      return 1;
    }

    yaml_mapping_end_event_initialize(&event);
    if (!yaml_emitter_emit(&emitter, &event)) {
      _detach(emitter, event);
      return 1;
    }
  }

  yaml_sequence_end_event_initialize(&event);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }

  yaml_mapping_end_event_initialize(&event);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }

  yaml_document_end_event_initialize(&event, 0);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }

  yaml_stream_end_event_initialize(&event);
  if (!yaml_emitter_emit(&emitter, &event)) {
    _detach(emitter, event);
    return 1;
  }

  yaml_emitter_delete(&emitter);
  return 0;
}


int perform_parse(struct state* st, yaml_event_t* event, struct cfg_tool* t) {
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

int cfg_tool(const char* filename, struct cfg_tool tools[]) {
    yaml_parser_t parser;
    yaml_event_t event;

    FILE* fd = fopen(filename, "r");
    if(!fd) {
      log(error, "failed to config file;");
      return 0;
    }

    void _detach(yaml_parser_t p, FILE* fd) {
      yaml_parser_delete(&p);
      fclose(fd);
    }

    struct state st = { .state = START, .accepted = 0 };
   
    if(!yaml_parser_initialize(&parser)) 
      log(error, "error with yaml parse init;");

    yaml_parser_set_input_file(&parser, fd);

    int i = 0;
    do {
        if (!yaml_parser_parse(&parser, &event)) {
          log(error, "error with yaml parser;");
          _detach(parser, fd);
          return 0;
        }

        if (!perform_parse(&st, &event, &tools[i])) {
          log(error, "error with perform yaml event;");
          _detach(parser, fd);
          return 0;
        }

        if (st.accepted)
          i++;
        yaml_event_delete(&event);
    } while (st.state != STOP);

    _detach(parser, fd);
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
