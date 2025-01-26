/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *plusOne(int *digits, int digits_size, int *return_size) {
    assert(return_size + 1 >= digits_size);
    int *out_le = calloc(return_size, sizeof(int));
}
