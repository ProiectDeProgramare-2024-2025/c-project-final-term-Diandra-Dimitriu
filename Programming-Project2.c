#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void clearScreen() {
    system(CLEAR);
}
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

void print_colored(const char* color, const char* text) {
    printf("%s%s%s", color, text, RESET);
}

void e() {
    printf("-----------------------\n");
}

void error(int *m, int a, int b) {
    while (*m < a || *m > b) {
        print_colored(RED, "ERROR");
        printf(", try again:\n");
        scanf("%d", m);
    }
}
bool isValidStreetFormat(const char *input) {
    int len = strlen(input);
    if (len == 0) return false;

    //find the last space in the string
    const char *lastSpace = strrchr(input, ' ');
    if (!lastSpace) {
        //no space found =>invalid
        return false;
    }

    //check that there's something before the last space
    if (lastSpace == input) {
        // starts with space => invalid
        return false;
    }

    //check that what's after the last space is all digits
    const char *numberPart = lastSpace + 1;
    if (*numberPart == '\0') {
        //nothing after space => invalid
        return false;
    }

    while (*numberPart) {
        if (!isdigit(*numberPart)) {
            return false;
        }
        numberPart++;
    }

    //check that whats before the last space contains at least one letter
    bool hasLetter = false;
    const char *ptr = input;
    while (ptr < lastSpace) {
        if (isalpha(*ptr)) {
            hasLetter = true;
            break;
        }
        ptr++;
    }

    return hasLetter;
}

struct Order {
    char restaurant[50];
    char items[100];
    char address[100];
};

const char* get_restaurant(struct Order* order) {
    return order->restaurant;
}

const char* get_items(struct Order* order) {
    return order->items;
}

const char* get_address(struct Order* order) {
    return order->address;
}

void set_restaurant(struct Order* order, const char* r) {
    strcpy(order->restaurant, r);
}

void set_items(struct Order* order, const char* i) {
    strcpy(order->items, i);
}

void set_address(struct Order* order, const char* a) {
    strcpy(order->address, a);
}

void storeToCSV(const char *filename, const char *restaurant, const char *items, const char *address) {
    FILE *file = fopen(filename, "r");
    int fileExists = (file != NULL);
    if (file) fclose(file);
    
    file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    
    // If the file did not exist, add a header
    if (!fileExists) {
        fprintf(file, "Restaurant,Items,Address\n");
    }
    
    fprintf(file, "%s,%s,%s\n", restaurant, items, address);
    fclose(file);
}

void readFromCSV(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char line[256];

    // Skip the header
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Error reading file!\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char restaurant[50], items[100], address[100];
        sscanf(line, "%[^,],%[^,],%[^\n]", restaurant, items, address);
        printf("Restaurant: "CYAN"%s\n"RESET, restaurant);
        printf("Items: "CYAN"%s\n"RESET, items);
        printf("Address: "CYAN"%s\n"RESET, address);
        e();
    }
    fclose(file);
}

const char* restaurant(int m) {
    switch (m) {
        case 1: return "Ember & Thyme";
        case 2: return "Luna Slice";
        case 3: return "The Golden Wok";
        case 4: return "Harbor & Vine";
    }
}

void sizenextra(char *b) {
    char extras[100] = "";
    int extra;
    printf("Anything extra? (1.Cheese 2.Pepperoni 3.Tuna 4.Onions, or 0 for none):\n");
    scanf("%d", &extra);
    error(&extra, 0, 4);
    
    if (extra != 0) {
        sprintf(extras, " extra=%d", extra);
    } else {
        strcat(extras, " extra=none");
    }

    int size;
    printf("What size? (1.Small 2.Medium 3.Large):\n ");
    scanf("%d", &size);
    error(&size, 1, 3);
    
    char temp[30];
    sprintf(temp, " size=%d;", size);
    strcat(extras, temp);
    
    strcat(b, extras);
}

void items(char *b, int d) {
    int i = 0, a = 0;
    char temp[30];
    while (a != 5 && i < 7) {
        printf("Enter item numbers (1-4 for items;5 to stop):\n");
        scanf("%d", &a);
        error(&a, 1, 6);
        if (a == 5) break;
        strcat(b, " #");
        sprintf(temp, "%d", a);
        strcat(b, temp);
        if (d==1)
        {
            sizenextra(b);
        }
    }
    if (d==1)
    {
        char l[200]="";
        for (int i=0;i<strlen(b)-1;i++)
        {
            l[i]=b[i];
        }
        strcpy(b,l);

    }
}

void address(char *b,int d) {
    char addr[100];
    int t=0;
    printf("Please input address: (Street name + street number) ");
    scanf(" %[^\n]", addr);
    while (t==0)
    {
        if (isValidStreetFormat(addr)) {
            t = 1;
        } else {
            print_colored(RED, "ERROR");
            printf(", try again:\n");
            scanf(" %[^\n]", addr);
            continue;
        }
    }
    strcat(b, addr);

}

void thankyou() {
    printf("Thank you for your order!\n");
}

void menu(int n, struct Order *new_order) {
    clearScreen();
    int d=0;
    char ad[200], it[200];
    strcpy(it,"");
    strcpy(ad,"");
    set_restaurant(new_order, restaurant(n));

    if (n==1)
    {
        printf("=== %s Ember & Thyme ===\n");
        printf("What would you like?\n");
        printf("1. Charred Rosemary Chicken\n2. Smoked Beet Salad\n3. Fire-Grilled Lamb Chops\n4. Thyme-Infused Flatbread\n5. Done\n");
        items(it,d);
        set_items(new_order, it);
        
    } else if (n==2)
    {
        printf("=== %s Luna Slice ===\n");
        printf("What would you like?\n");
        printf("1. Truffle Moonlight Pizza\n2. Pizza Pepperoni\n3. Lunar Margherita\n4. Cosmic Calzone\n5. Done\n");
        d=1;
        items(it,d);
        set_items(new_order, it);
    } else if (n==3)
    {
        printf("=== %s The Golden Wok ===\n");
        printf("What would you like?\n");
        printf("1. Honey Sesame Chicken\n2. Szechuan Beef Noodles\n3. Golden Fried Rice\n4. Crispy Tofu with Ginger Glaze\n5. Done\n");
        items(it,d);
        set_items(new_order, it);
    } else if (n==4)
        {
            printf("=== %s Harbor & Vine ===\n");
            printf("What would you like?\n");
            printf("1. Seared Scallops with Wine Reduction\n2. Vineyard Caprese Salad\n3. Lemon Herb Salmon\n4. Garlic Mussel Linguine\n5. Done\n");
            items(it,d);
            set_items(new_order, it);
        }

    
  address(ad,d);
  set_address(new_order, ad);
  print_colored(GREEN, "Order Summary:\n");
  printf("Restaurant: "CYAN"%s\n"RESET, get_restaurant(new_order));
  printf("Items: "CYAN"%s\n"RESET, get_items(new_order));
  printf("Address: "CYAN"%s\n"RESET, get_address(new_order));
  storeToCSV("order_history.csv", get_restaurant(new_order), get_items(new_order), get_address(new_order));
  thankyou();
  printf("Press Enter to continue...");
  getchar();
  getchar();
}

void order_menu() {
    clearScreen();
    printf("=== Order Menu ===\n");
    printf("Select a restaurant:\n");
    printf("1.Ember & Thyme\n2.Luna Slice\n3.The Golden Wok\n4.Harbor & Vine\n:");
    
    int n;
    scanf("%d", &n);
    error(&n, 1, 4);
    struct Order *new_order = malloc(sizeof(struct Order));
    if (new_order == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }
    set_restaurant(new_order, "");
    set_items(new_order, "");
    set_address(new_order, "");
    menu(n, new_order);
    free(new_order);
}

void view_order_history() {
    clearScreen();
    printf("=== Order History ===\n");
    readFromCSV("order_history.csv");
    e();
    printf("Press Enter to continue...");
    getchar();
    getchar();
}

void exitProgram() {
    clearScreen();
    print_colored(MAGENTA, "Goodbye!\n");
}

void main_menu(int *m) {
    clearScreen();
    printf("=== Main Menu ===\n");
    printf("1.Order\n2.View order history\n3.Exit\n:");
    scanf("%d", m);
    error(m, 1, 3);
    e();
}

int main() {
    int m;
    do {
        main_menu(&m);
        if (m == 1) {
            order_menu();
        } else if (m == 2) {
            view_order_history();
        }
    } while (m != 3);
    
    exitProgram();
    return 0;
}
