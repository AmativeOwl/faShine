#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fashion.h"

#define NUM_OCCASION 6
#define NUM_OCCASION_PAIRS 36
#define MAX_CLOSET_SIZE 200

typedef struct OccasionTable
{
    int hash;
    double compatibilityScore;
    struct OccasionTable *next;
} OccasionTable;

int hash(int item1Occasion, int item2Occasion)
{
    int min = (item1Occasion < item2Occasion) ? item1Occasion : item2Occasion;
    int max = (item1Occasion > item2Occasion) ? item1Occasion : item2Occasion;

    return abs(min) * NUM_OCCASION + abs(max);
}

void insert(OccasionTable **table, int hash, double compatibilityScore)
{
    OccasionTable *newOccasion = malloc(sizeof(OccasionTable));
    newOccasion->compatibilityScore = compatibilityScore;
    newOccasion->next = table[hash];
    table[hash] = newOccasion;
}

int cmp(const void *a, const void *b)
{
    const Outfit *x = (const Outfit *)a;
    const Outfit *y = (const Outfit *)b;

    if (x->score < y->score)
        return 1;
    if (x->score > y->score)
        return -1;
    return 0;
}

double score_colour(HSL item1Colour, HSL item2Colour, enum Adjective adj)
{
    int item1Hue = item1Colour.hue, item2Hue = item2Colour.hue;
    float item1Sat = item1Colour.saturation, item2Sat = item2Colour.saturation;
    float item1Light = item1Colour.lightness, item2Light = item2Colour.lightness;
    float score;

    float distHue = (fabs(item1Hue - item2Hue) < 360 - fabs(item1Hue - item2Hue)) ? fabs(item1Hue - item2Hue) : 360 - fabs(item1Hue - item2Hue);

    switch (adj)
    {
    case BOLD:
        score = 1 - (fabs(180 - distHue) / 180);
        break;
    case SUBTLE:
        score = 1 - (fabs(30 - distHue) / 30);
        break;
    case TONAL:
        score = fabs(item1Light - item2Light);
        break;
    case ECLECTIC:
        score = 1 - (fabs(120 - distHue) / 120);
        break;
    case CLASSIC:
        score = 1 - (item1Sat + item2Sat) / 2;
        break;
    default:
        break;
    }

    return score;
}

double score_occasion(int item1Occasion, int item2Occasion)
{
    if (item1Occasion == item2Occasion)
        return 1.0;

    static OccasionTable **table = NULL;
    if (table == NULL)
    {
        table = calloc(NUM_OCCASION_PAIRS, sizeof(OccasionTable *));
        insert(table, hash(CASUAL, ATHLETIC), 0.5);
        insert(table, hash(SMART_CASUAL, CORPORATE), 0.5);
        insert(table, hash(FORMAL, EVENING), 0.5);
    }

    int key = hash(item1Occasion, item2Occasion);
    if (table[key])
        return table[key]->compatibilityScore;

    return 0.0;
}

double score_pair(FashionItem item1, FashionItem item2, enum Adjective adj, enum Occasion occasion)
{
    double colour_score = score_colour(item1.colour, item2.colour, adj);
    double occasion_score = score_occasion(item1.occasion, item2.occasion);

    return (occasion == CASUAL) ? (colour_score + occasion_score) / 2.0 : (colour_score * 0.3) + (occasion_score * 0.7);
}

Outfit *generate_outfits(FashionItem *closet, int closetSize, int n, enum Adjective adj, enum Occasion occasion)
{
    if (closetSize > MAX_CLOSET_SIZE)
        return NULL;

    Outfit *outfit_matches = malloc(closetSize * closetSize * sizeof(Outfit));
    int outfit_idx = 0;
    outfit_matches[outfit_idx].occasion = occasion;

    FashionItem top, bottom;
    for (int i = 0; i < closetSize; i++)
    {
        if (closet[i].fashionType == TOP)
        {
            top = closet[i];
            for (int j = 0; j < closetSize; j++)
            {
                if (closet[j].fashionType == BOTTOM)
                {
                    bottom = closet[j];

                    outfit_matches[outfit_idx].itemIdsSize = 2;
                    int *itemIds = malloc(2 * sizeof(int));
                    itemIds[0] = top.id;
                    itemIds[1] = bottom.id;

                    outfit_matches[outfit_idx].score = score_pair(top, bottom, adj, occasion);
                    outfit_matches[outfit_idx++].itemIds = itemIds;
                }
            }
        }
    }

    qsort(outfit_matches, outfit_idx, sizeof(Outfit), cmp);

    return outfit_matches;
}