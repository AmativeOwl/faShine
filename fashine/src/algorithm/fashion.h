#ifndef FASHION_H
#define FASHION_H

enum Adjective
{
    BOLD,
    SUBTLE,
    TONAL,
    ECLECTIC,
    CLASSIC
};

enum Type
{
    TOP,
    BOTTOM
};

enum Occasion
{
    CASUAL,
    SMART_CASUAL,
    CORPORATE,
    FORMAL,
    ATHLETIC,
    EVENING
};

typedef struct HSL
{
    int hue;
    float saturation;
    float lightness;
} HSL;

typedef struct FashionItem
{
    int id;
    enum Type fashionType;
    HSL colour;
    enum Occasion occasion;
} FashionItem;

typedef struct Outfit
{
    double score;
    int *itemIds;
    int itemIdsSize;
    enum Occasion occasion;
} Outfit;

int hash(int item1Occasion, int item2Occasion);
int cmp(const void *a, const void *b);
double score_colour(HSL item1Colour, HSL item2Colour, enum Adjective adj);
double score_occasion(int item1Occasion, int item2Occasion);
double score_pair(FashionItem item1, FashionItem item2, enum Adjective adj, enum Occasion occasion);
Outfit *generate_outfits(FashionItem *closet, int closetSize, int n, enum Adjective adj, enum Occasion occasion);

#endif