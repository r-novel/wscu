#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yaml.h>

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

int perform(struct state*, yaml_event_t*, struct cfg_tool*);
int cfg_tool(const char*, struct cfg_tool*);
void cfg_tool_free(struct cfg_tool*);
void test(void);

#endif // _CONFIG_H_
