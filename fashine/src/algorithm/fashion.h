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