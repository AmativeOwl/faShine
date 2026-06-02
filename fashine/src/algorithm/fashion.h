enum Type
{
    TOP,
    BOTTOM,
    SHOES
};

enum OCCASION
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
    enum OCCASION occasion;
} FashionItem;

typedef struct Outfit
{
    double score;
    int *itemIds;
    int itemIdsSize;
    enum OCCASION occasion;
} Outfit;