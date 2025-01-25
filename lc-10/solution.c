#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

enum CurrentMatchType {
    Wildcard,
    Char,
};
struct CurrentMatch {
    enum CurrentMatchType type;
    union {
        char c;
    };
};

static bool matches_char(struct CurrentMatch current_match, char c) {
    return (current_match.type == Wildcard) || (c == current_match.c);
}
static bool pattern_matches_substring(char *pattern, size_t pattern_len, char *str, size_t str_len) {
    size_t s_pos = 0;
    for (size_t p_pos = 0; p_pos < pattern_len; p_pos++) {
        struct CurrentMatch match;
        if (pattern[p_pos] == '.') {
            match.type = Wildcard;
        } else {
            match.type = Char;
            match.c = str[s_pos];
        }

        // figure out how many times to match current char
        size_t p_pos_next = p_pos + 1;
        if ((p_pos_next < pattern_len) && pattern[p_pos_next] == '*') {
            // move s_pos forward until the quantifier stops matching
            for (; s_pos < str_len; s_pos++) {
                // fine if this immediately breaks because * also matches 0
                if (!matches_char(match, str[s_pos])) {
                    break;
                }
            }
        } else {
            // just check current char, if it fails then pattern does not match
            if (!matches_char(match, str[s_pos])) {
                return false;
            }
        }
    }

    assert(s_pos <= str_len);
    if (s_pos == str_len) {
        return true;
    } else {
        // did not match the full string
        return false;
    }
}

// future optimization: find .*'s at beginning/end and just glob the rest of the string if present
bool isMatch(char *s, char *p) {
    size_t s_len = strlen(s);
    size_t p_len = strlen(p);
    for (size_t s_pos = 0; s_pos < s_len; s_pos++) {
        if (pattern_matches_substring(p, p_len, s, s_len)) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: lc-10 [string] [pattern]");
        exit(1);
    }
    assert(isMatch(argv[1], argv[2]));
}
