#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fashion.h"

#define NUM_OCCASION 6
#define NUM_OCCASION_PAIRS 36
#define MAX_CLOSET_SIZE 200

/*
 * Node in a simple separately-chained hash table mapping an occasion-pair
 * to a hand-picked "compatibility score" (e.g. how well CASUAL pairs with ATHLETIC).
 */
typedef struct OccasionTable
{
    int hash;
    double compatibilityScore;
    struct OccasionTable *next;
} OccasionTable;

/*
 * Combines two occasion enum values into a single unique table index.
 */
int hash(int item1Occasion, int item2Occasion)
{
    int min = (item1Occasion < item2Occasion) ? item1Occasion : item2Occasion;
    int max = (item1Occasion > item2Occasion) ? item1Occasion : item2Occasion;

    return abs(min) * NUM_OCCASION + abs(max);
}

/*
 * Inserts a compatibility score into the hash table at the given key,
 * prepending onto any existing chain at that slot.
 */
void insert(OccasionTable **table, int hash, double compatibilityScore)
{
    OccasionTable *newOccasion = malloc(sizeof(OccasionTable));
    newOccasion->hash = hash;
    newOccasion->compatibilityScore = compatibilityScore;
    newOccasion->next = table[hash];
    table[hash] = newOccasion;
}

/*
 * Sorts Outfit entries by score in descending order (highest score first)
 * as the "best" outfits are positioned at the front of the array to be
 * later truncated to the top n.
 */
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

/*
 * Scores how well two garment colours pair together, given a desired
 * styling "adjective" (BOLD, SUBTLE, TONAL, ECLECTIC, CLASSIC).
 *
 * All formulas operate on HSL colour space:
 *   hue        -- 0-360 degrees, position on the colour wheel
 *   saturation -- 0.0-1.0, colour intensity/purity
 *   lightness  -- 0.0-1.0, how light/dark the colour is
 *
 * distHue: the circular, shortest-path distance between the two hues on
 * the 360-degree colour wheel.
 *
 * Each case implements a different classic colour-pairing rule of thumb:
 *
 * BOLD: rewards colours that are near-complementary (180 degrees
 * apart on the wheel) AND highly saturated. hue_score peaks at 1.0
 * when distHue is exactly 180; sat_score is the average saturation
 * of both items. Final score averages the two as "bold" needs both
 * high contrast and saturation.
 *
 * SUBTLE: rewards colours close together on the wheel (30 degrees apart,
 * i.e. "analogous" colours). Score peaks at 1.0 when distHue == 30 and
 * falls off the further away it gets.
 *
 * TONAL: ignores hue entirely; rewards similar LIGHTNESS between the two
 * items (a "tonal" outfit varies shade, not colour). Score is the absolute
 * difference in lightness values.
 *
 * ECLECTIC: rewards colours 120 degrees apart (i.e. a "triadic" colour
 * relationship on the wheel), serving as a middle ground between BOLD's
 * 180-degree contrast and SUBTLE's 30-degree closeness.
 *
 * CLASSIC: ignores hue, rewarding LOW saturation on both items (muted,
 * desaturated, "classic" neutral tones).
 */
double score_colour(HSL item1Colour, HSL item2Colour, enum Adjective adj)
{
    int item1Hue = item1Colour.hue, item2Hue = item2Colour.hue;
    float item1Sat = item1Colour.saturation, item2Sat = item2Colour.saturation;
    float item1Light = item1Colour.lightness, item2Light = item2Colour.lightness;
    float hue_score, sat_score, score;

    float distHue = (abs(item1Hue - item2Hue) < 360 - abs(item1Hue - item2Hue)) ? abs(item1Hue - item2Hue) : 360 - abs(item1Hue - item2Hue);

    switch (adj)
    {
    case BOLD:
        hue_score = 1 - (fabs(180 - distHue) / 180);
        sat_score = (item1Sat + item2Sat) / 2;
        score = (hue_score + sat_score) / 2;
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

/*
 * Scores how well two occasions pair together.
 *
 * Same occasion (e.g. CASUAL + CASUAL) always scores a perfect 1.0.
 * Otherwise, looks up a hand-picked compatibility score from the hash
 * table (e.g. CASUAL + ATHLETIC = 0.5 are reasonably compatible but not
 * identical). Any pair not explicitly inserted defaults to 0.0
 * (incompatible).
 */
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

/*
 * Combines colour compatibility and occasion compatibility into one
 * overall score for a top+bottom pairing.
 *
 * For CASUAL outfits: colour and occasion are weighted equally (50/50),
 * since casual wear has more flexibility because style matters as much as
 * strict occasion-matching.
 *
 * For any other occasion (SMART_CASUAL, CORPORATE, FORMAL, ATHLETIC,
 * EVENING): occasion-appropriateness is weighted much more heavily (70%)
 * than colour (30%).
 */
double score_pair(FashionItem item1, FashionItem item2, enum Adjective adj, enum Occasion occasion)
{
    double colour_score = score_colour(item1.colour, item2.colour, adj);
    double occasion_score = score_occasion(item1.occasion, item2.occasion);

    return (occasion == CASUAL) ? (colour_score + occasion_score) / 2.0 : (colour_score * 0.3) + (occasion_score * 0.7);
}

/*
 * Generates every possible TOP + BOTTOM pairing from the closet, scores
 * each one, sorts them best-to-worst, and returns the top n.
 */
Outfit *generate_outfits(FashionItem *closet, int closetSize, int n, enum Adjective adj, enum Occasion occasion)
{
    if (closetSize > MAX_CLOSET_SIZE)
        return NULL;

    Outfit *outfit_matches = malloc(closetSize * closetSize * sizeof(Outfit));
    int outfit_idx = 0;

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

                    outfit_matches[outfit_idx].occasion = occasion;
                    outfit_matches[outfit_idx].score = score_pair(top, bottom, adj, occasion);
                    outfit_matches[outfit_idx++].itemIds = itemIds;
                }
            }
        }
    }

    qsort(outfit_matches, outfit_idx, sizeof(Outfit), cmp);

    if (n > outfit_idx)
        n = outfit_idx;

    outfit_matches = realloc(outfit_matches, n * sizeof(Outfit));

    return outfit_matches;
}