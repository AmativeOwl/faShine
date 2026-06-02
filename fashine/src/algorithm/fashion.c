#include <stdio.h>
#include <stdlib.h>
#include <fashion.h>

double score_colour(FashionItem item1, FashionItem item2, enum Adjective adj)
{
    HSL item1Colour = item1.colour;
    HSL item2Colour = item2.colour;
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