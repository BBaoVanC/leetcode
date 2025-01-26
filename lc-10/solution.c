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
};

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
        return s_pos;
    } else {
        // unreachable
        abort();
    }
}
static bool pattern_matches_substring(char *pattern, size_t pattern_len, char *str, size_t str_len) {
    // incremented if successful match so we know where to continue
    size_t s_pos = 0;
    for (size_t p_pos = 0; p_pos < pattern_len; p_pos++) {
        assert(s_pos <= str_len);
        if (s_pos == str_len) {
            // we got to end of string but the full pattern wasn't matched
            return false;
        }

        struct CurrentMatch match;
        if (pattern[p_pos] == '.') {
            match.type = Wildcard;
        } else {
            match.type = Char;
            match.c = pattern[p_pos];
        }

        size_t p_pos_next = p_pos + 1;
        enum Quantifier quantifier;
        // no bounds check needed because the string will have null at the end
        if (pattern[p_pos_next] == '*') {
            // don't try and match a '*' on the next iteration
            p_pos++;
            quantifier = Greedy;
        } else {
            quantifier = None;
        }

        char *substr = str + s_pos;
        size_t substr_len = str_len - s_pos;
        int ret = match_token_on_substring(match, quantifier, substr, substr_len);
        if (ret < 0) {
            return false;
        }
        s_pos += ret;
    }

    return s_pos == str_len;
}

// future optimization: find .*'s at beginning/end and just glob the rest of the string if present
bool isMatch(char *s, char *p) {
    size_t s_len = strlen(s);
    size_t p_len = strlen(p);
    return pattern_matches_substring(p, p_len, s, s_len);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("usage: lc-10 [string] [pattern]\n");
        exit(1);
    }
    assert(isMatch(argv[1], argv[2]));
}
