#include <stdio.h>
#include <stdlib.h>
#include <fashion.h>

#define NUM_OCCASION 6
#define NUM_OCCASION_PAIRS 36

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

double
score_colour(FashionItem item1, FashionItem item2, enum Adjective adj)
{
    HSL item1Colour = item1.colour, item2Colour = item2.colour;

    int item1Hue = item1Colour.hue, item2Hue = item2Colour.hue;
    float item1Sat = item1Colour.saturation, item2Sat = item2Colour.saturation;
    float item1Light = item1Colour.lightness, item2Light = item2Colour.lightness;
    float score;

    float distHue = (fabs(item1Hue - item2Hue) < 360 - abs(item1Hue - item2Hue)) ? abs(item1Hue - item2Hue) : 360 - abs(item1Hue - item2Hue);

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

double score_occasion(FashionItem item1, FashionItem item2)
{
    int item1Occasion = item1.occasion, item2Occasion = item2.occasion;

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

    return 0;
}

double score_pair(FashionItem item1, FashionItem item2, enum Adjective adj)
{
}

Outfit *generate_outfits(FashionItem *closet, int closetSize, int n, enum Adjective adj)
{
}