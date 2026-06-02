#include <stdio.h>
#include <stdlib.h>
#include <fashion.h>

double score_colour(FashionItem item1, FashionItem item2, enum Adjective adj)
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
}

double score_pair(FashionItem item1, FashionItem item2, enum Adjective adj)
{
}

Outfit *generate_outfits(FashionItem *closet, int closetSize, int n, enum Adjective adj)
{
}