#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
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

enum Quantifier {
    None,
    Greedy,
}

static bool matches_char(struct CurrentMatch current_match, char c) {
    return (current_match.type == Wildcard) || (c == current_match.c);
}
// returns -1 if match failed, otherwise the amount of chars the token matched successfully
static int match_token_on_substring(struct CurrentMatch match, enum Quantifier quantifier, char *str, size_t str_len) {
    if (quantifier == None) {
        if (matches_char(match, str[0])) {
            return 1;
        } else {
            return -1;
        }
    } else if (quantifier == Greedy) {
        size_t s_pos = 0;
        while ((s_pos < str_len) && (matches_char(match, str[s_pos]))) {
            s_pos++;
        }
    } else {
        // unreachable
        abort();
    }
}
static bool pattern_matches_substring(char *pattern, size_t pattern_len, char *str, size_t str_len) {
    // incremented if successful match so we know where to continue
    size_t s_pos = 0;
    for (size_t p_pos = 0; p_pos < pattern_len; p_pos++) {
        struct CurrentMatch match;
        if (pattern[p_pos] == '.') {
            match.type = Wildcard;
        } else {
            match.type = Char;
            match.c = pattern[p_pos];
        }

        size_t p_pos_next = p_pos + 1;
        if ((p_pos_next < pattern_len) && pattern[p_pos_next] == '*') {
            // don't try and match a '*' on the next iteration
            p_pos++;

            // move s_pos forward until the quantifier stops matching
            // fine if this immediately breaks because * also matches 0
            for (; s_pos < str_len; s_pos++) {
                if (!matches_char(match, str[s_pos])) {
                    break;
                }
            }
        } else {
            if (!matches_char(match, str[s_pos])) {
                return false;
            }
        }
    }

    assert(s_pos <= str_len);
    if (s_pos == str_len) {
        // the whole string passed
        return true;
    } else {
        return false;
    }
}

// future optimization: find .*'s at beginning/end and just glob the rest of the string if present
bool isMatch(char *s, char *p) {
    size_t s_len = strlen(s);
    size_t p_len = strlen(p);
    for (size_t s_pos = 0; s_pos < s_len; s_pos++) {
        char *substr = s + s_pos;
        size_t substr_len = s_len - s_pos;
        if (pattern_matches_substring(p, p_len, substr, substr_len)) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: lc-10 [string] [pattern]\n");
        exit(1);
    }
    assert(isMatch(argv[1], argv[2]));
}
