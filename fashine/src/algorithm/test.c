#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fashion.h"

int main(int argc, char *argv[])
{
    FashionItem closet[] = {
        {1, TOP, {30, 0.9f, 0.5f}, CASUAL},
        {2, BOTTOM, {210, 0.9f, 0.5f}, CASUAL},
        {3, TOP, {60, 0.9f, 0.5f}, CASUAL},
        {4, BOTTOM, {240, 0.9f, 0.5f}, CASUAL},

        {5, TOP, {0, 0.9f, 0.5f}, CASUAL},
        {6, BOTTOM, {10, 0.9f, 0.5f}, CASUAL},

        {7, TOP, {30, 0.05f, 0.9f}, CASUAL},
        {8, BOTTOM, {0, 0.0f, 0.1f}, CASUAL},

        {9, TOP, {0, 0.9f, 0.5f}, CASUAL},
        {10, BOTTOM, {10, 0.9f, 0.5f}, CASUAL},
    };

    int closetSize = 10;

    Outfit *results = generate_outfits(closet, closetSize, 3, BOLD, CASUAL);

    assert(results != NULL);
    assert(results[0].score > 0.8);
    assert(results[0].score >= results[1].score);

    printf("Top 3 outfits:\n");
    for (int i = 0; i < 3; i++)
        printf("Outfit %d: score %.4f, top id %d, bottom id %d\n", i + 1, results[i].score, results[i].itemIds[0], results[i].itemIds[1]);

    free(results);
    return 0;
}