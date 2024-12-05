#include <stdio.h>
#include <string.h>

#define MAX_ITEMS 100
#define MAX_USERS 100
#define LENGTH 50

// Structure to store shop items
typedef struct {
    int id;
    char name[50];
    float price;
    char quantity[10];
    float discount;
} Product;

// Structure to hold user information
typedef struct {
    char username[LENGTH];
    char password[LENGTH];
    int isAdmin; // Flag to indicate if the user is an admin
} User;

typedef struct {
    char name[50];
    int quantity;
} CartItem;

// Global variables
Product supershop[MAX_ITEMS]; // Array to hold items
int product_count = 0;
User users[MAX_USERS]; // Array to hold users
int userCount = 0;
CartItem cart[50];
int cartCount = 0;
int loggedIn = -1;// Variable to track login status

// Function prototypes
void registration();    //Raihana
int login();    //Raihana
void view_menu(int loggedIn); //Raihana
void searchProduct();   //Raihana
void addItem(char *name, float price, char *quantity, float discount);  //Ariyan
void deleteItem(char *name);    //Ariyan
void updateItem(char *name);    //Ariyan
float enterDiscount();  //ariyan
void feedback(); //saad
void addToCart(char *name, int quantity); //saad
void addMultipleToCart(); //saad
void displayCart(); //saad
void checkout(); //ambia
void selectPaymentMethod(); //ambia
void updateStockAfterCheckout(); //ambia
void displaySupershop(); //ambia



// Function for user registration
void registration() {
    char username[LENGTH];
    char password[LENGTH];
    int isAdmin;
    User user; // User structure instance
    FILE *file = fopen("users.txt", "a+");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nSet Username: ");
    scanf("%s", username);

    while (fscanf(file, "%s %s %d", user.username, user.password, &user.isAdmin) != EOF) {
        if (strcmp(user.username, username) == 0) {
            printf("Username already exists!\n");
            fclose(file);
            return;
        }
    }
    printf("Set Password: ");
    scanf("%s", password);
    printf("Register as admin (1) or user (0): ");
    scanf("%d", &isAdmin);

    fprintf(file, "%s %s %d\n", username, password, isAdmin);
    printf("Registration successful!\n");
    fclose(file);
}

// Function for user login
int login() {
    char username[LENGTH];
    char password[LENGTH];
     User user;
    FILE *file = fopen("users.txt", "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return -1;
    }
    printf("\nEnter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    while (fscanf(file, "%s %s %d", user.username, user.password, &user.isAdmin) != EOF) {

        if (strcmp(user.username, username) == 0 && strcmp(user.password, password) == 0) {
            printf("\n----------------Successful Login!-----------------\n");
            fclose(file);
            return user.isAdmin;
        }
    }
    printf("\n----------------Invalid username or password!-------------\n\n");
    fclose(file);
    return -1;
}

// Function to search for a product by name
void searchProduct() {
    char searchName[50];
    int found = 0;

    printf("\nEnter product name to search: ");
    scanf(" %s", searchName);

    printf("\n--- Search Results ---\n");

    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(file, "%d %s %f %s %f", &supershop[product_count].id, supershop[product_count].name,
                  &supershop[product_count].price, supershop[product_count].quantity,
                  &supershop[product_count].discount) != EOF) {
        if (strcmp(supershop[product_count].name, searchName) == 0) {

            printf("\n  Name: %s\n  Price: %.2f\n  Stock: %s\n",supershop[product_count].name,
                   supershop[product_count].price, supershop[product_count].quantity);
            found = 1;
        }
    }

    fclose(file);
    if (!found) {
        printf("No products found matching '%s'.\n", searchName);
    }
}

// Function to display the menu bar based on user role
void view_menu(int loggedIn) {
    int choice;
    char name[50], quantity[10];
    float price, discount;

    if (loggedIn == 1) {
        printf("\n1. Add Product\n2. Delete Product\n3. Update Product\n4. Display Product\n5. Search\n6. Logout\n");
    } else {
        printf("\n1. Display products\n2. Search Product\n3. View Cart\n4. Add products to cart\n5. Logout\n");
    }
    printf("\nChoose an option: ");
    scanf("%d", &choice);

    if (loggedIn == 1) {
        switch (choice) {
            case 1:
                displaySupershop();
                printf("\nEnter product name: ");
                scanf(" %49s", name);
                printf("Enter product price: ");
                scanf("%f", &price);
                printf("Enter product quantity: ");
                scanf(" %9s", quantity);
                printf("Do you want to add a discount? (1 for Yes, 0 for No): ");
                int addDiscount;
                scanf("%d", &addDiscount);
                if (addDiscount) {
                        discount = enterDiscount();
                } else {
                        discount = 0.0;
                }
                addItem(name, price, quantity, discount);
                displaySupershop();
                printf("\nNew product has been added successfully.\n");
                view_menu(loggedIn);
                break;
            case 2:
                printf("\nEnter product name to delete: ");
                scanf(" %49s", name);
                deleteItem(name);
                break;
            case 3:
                printf("\nEnter product name to update: ");
                scanf(" %49s", name);
                updateItem(name);
                break;
            case 4:
                displaySupershop();
                view_menu(loggedIn);
                break;
            case 5:
                searchProduct();
                int choice;
                printf("\n\nDo you want to-\n     1. View Menu\n     2. Exit\n");
                printf("Choose an option: ");
                scanf("%d", &choice);
                switch(choice){
                    case 1:
                        view_menu(loggedIn);
                        break;
                    case 2:
                        printf("\nYou logged out and exit the program successfully.\n");
                        break;
                }
                break;
            case 6:
                loggedIn = -1;
                printf("Logged out successfully.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } else {
        switch (choice) {
            case 1:
                displaySupershop();
                view_menu(loggedIn);
                break;
            case 2:
                searchProduct();
                int choice;
                printf("\n\nDo you want to-\n     1. View Menu\n     2. Exit\n");
                printf("Choose an option: ");
                scanf("%d", &choice);
                switch(choice){
                    case 1:
                        view_menu(loggedIn);
                        break;
                    case 2:
                        printf("\nYou logged out and exit the program successfully.\n");
                        break;
                }
                break;
            case 3:
                displayCart();
                break;
            case 4:
                addMultipleToCart();
                break;
           case 5:
                loggedIn = -1;
                printf("Logged out successfully.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to add an item to the shop
void addItem(char *name, float price, char *quantity, float discount) {

    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading!\n");
        return;
    }

    product_count = 0;

    while (fscanf(file, "%d %s %f %s %f", &supershop[product_count].id, supershop[product_count].name,
                  &supershop[product_count].price, supershop[product_count].quantity,
                  &supershop[product_count].discount) != EOF) {
        product_count++;
    }
    fclose(file);

    supershop[product_count].id = product_count + 1; // Automatically assign a unique ID
    strcpy(supershop[product_count].name, name);
    supershop[product_count].price = price;
    strcpy(supershop[product_count].quantity, quantity);
    supershop[product_count].discount = discount;

    file = fopen("inventory.txt", "a");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    fprintf(file, "%d %s %.2f %s %.2f\n", supershop[product_count].id, supershop[product_count].name,
            supershop[product_count].price, supershop[product_count].quantity, supershop[product_count].discount);
    fclose(file);

    product_count++;
}

// Function to delete an item from the shop
void deleteItem(char *name) {
    int found = 0;
    FILE *file = fopen("inventory.txt", "r");
    FILE *tempFile = fopen("temp_inventory.txt", "w"); // Temporary file for updated inventory

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(file, "%d %s %f %s %f", &supershop[product_count].id, supershop[product_count].name,
                  &supershop[product_count].price, supershop[product_count].quantity,
                  &supershop[product_count].discount) != EOF) {

        if (strcmp(supershop[product_count].name, name) == 0) {
            found = 1;
            printf("\nProduct with name '%s' has been deleted.\n", name);
            continue; // Skip writing this item to the temp file
        }
        fprintf(tempFile, "%d %s %.2f %s %.2f\n", supershop[product_count].id, supershop[product_count].name,
                supershop[product_count].price, supershop[product_count].quantity, supershop[product_count].discount);
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated one
    remove("inventory.txt");
    rename("temp_inventory.txt", "inventory.txt");

    if (!found) {
        printf("Product with name '%s' not found in the supershop.\n", name);
    }
    view_menu(1);
}

// Function to update an item in the shop
void updateItem(char *name) {
    int found = 0;
    FILE *file = fopen("inventory.txt", "r");
    FILE *tempFile = fopen("temp_inventory.txt", "w");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(file, "%d %s %f %s %f", &supershop[product_count].id, supershop[product_count].name,
                  &supershop[product_count].price, supershop[product_count].quantity,
                  &supershop[product_count].discount) != EOF) {

        if (strcmp(supershop[product_count].name, name) == 0) {
            found = 1;
            printf("\nUpdating product with name: %s.\n", name);

            float price;
            char quantity[10];
            float discount;

            printf("Enter new price: ");
            scanf("%f", &price);
            printf("Enter new quantity (e.g., 10L or 250gm): ");
            scanf(" %9s", quantity);
            printf("Do you want to add a discount? (1 for Yes, 0 for No): ");
            int addDiscount;
            scanf("%d", &addDiscount);

            if (addDiscount) {
                discount = enterDiscount();
            } else {
                discount = 0.0;
            }
            // Update item details
            strcpy(supershop[product_count].name, name);
            supershop[product_count].price = price;
            strcpy(supershop[product_count].quantity, quantity);
            supershop[product_count].discount = discount;

            printf("\nProduct with name %s has been updated successfully.\n", name);
        }

        // Write the item to the temporary file
        fprintf(tempFile, "%d %s %.2f %s %.2f\n", supershop[product_count].id, supershop[product_count].name,
                supershop[product_count].price, supershop[product_count].quantity, supershop[product_count].discount);
        product_count++;
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated one
    remove("inventory.txt");
    rename("temp_inventory.txt", "inventory.txt");

    if (!found) {
        printf("Product with name %s not found in the supershop.\n", name);
    }
    view_menu(1);
}

// Function to enter discount percentage
float enterDiscount() {
    float discount;
    printf("Enter discount percentage: ");
    scanf("%f", &discount);
    return discount;
}

// Function to add an item to the cart
void addToCart(char *name, int quantity) {
    for (int i = 0; i < product_count; i++) {
        if (strcmp(supershop[i].name, name) == 0) {
            int alreadyInCart = 0;
            for (int j = 0; j < cartCount; j++) {
                if (strcmp(cart[j].name, name) == 0) {
                    cart[j].quantity += quantity;
                    alreadyInCart = 1;
                    break;
                }
            }
            if (!alreadyInCart) {
                strcpy(cart[cartCount].name, name);
                cart[cartCount].quantity = quantity;
                cartCount++;
            }
            printf("\nAdded %d of %s to cart.\n", quantity, name);
            return;
        }
    }
    printf("\nProduct %s not found.\n", name);
}

// Function to add multiple items to the cart
void addMultipleToCart() {
    char name[50];
    int quantity;
    char select;
    displaySupershop();

    do {
        printf("Choose product to add to cart: ");
        scanf("%s", name);
        printf("Enter quantity: ");
        scanf("%d", &quantity);
        addToCart(name, quantity);

        printf("Do you want to add another item to the cart? (y/n): ");
        scanf(" %c", &select);
    } while (select == 'y' || select == 'Y');
    int choice;
    printf("\nDo you want to-\n     1. View Cart\n     2. Go Back\n");
    printf("Choose an option: ");
    scanf("%d", &choice);
    switch(choice){
    case 1:
        displayCart();
        break;
    case 2:
        view_menu(0);
        break;
    }
}

// Function to display the cart
void displayCart() {
    if (cartCount == 0) {
        printf("\n--- Cart is Empty ---\n");
        printf("\nAdd product to cart.\n");
        view_menu(0);
        return;
    }
    printf("\n--- Cart ---\n");
    float total = 0.0;
    for (int i = 0; i < cartCount; i++) {
        for (int j = 0; j < product_count; j++) {
            if (strcmp(cart[i].name, supershop[j].name) == 0) {
                float itemTotal = supershop[j].price * cart[i].quantity;
                float discountAmount = (supershop[j].discount / 100) * itemTotal;
                float finalPrice = itemTotal - discountAmount;
                printf("Item: %s, Quantity: %d, Price: %.2f, Discount: %.2f, Final Price: %.2f\n",
                       supershop[j].name, cart[i].quantity, itemTotal, discountAmount, finalPrice);
                total += finalPrice;
            }
        }
    }
    printf("Total: Taka %.2f\n", total);
    int choice;
    printf("\nDo you want to-\n     1. Check Out\n     2. Go Back\n");
    printf("Choose an option: ");
    scanf("%d", &choice);
    switch(choice){
    case 1:
        checkout();
        break;
    case 2:
        view_menu(0);
        break;
    }

}

// Function to request customer feedback
void feedback(){
    int choice;
    char experience[10];

    printf("\n1.     Please send us feedback\n2.     exit\nchoose: ");
    scanf("%d", &choice);

    switch(choice){
        case 1:
            printf("How was your experience? (bad/good/better/excellent): ");
            scanf("%s", experience);
            printf("\n\nThank you for your feedback!\n");
           break;
        case 2:
            loggedIn = -1;
            printf("\nYou logged out and exit the program successfully.\n");
            break;
    }
}

// Function to display all items in the shop
void displaySupershop() {
    FILE *file = fopen("inventory.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\n==================== List of products =====================================\n");
    printf("| %-5s | %-20s | %-12s | %-10s | %-10s  |\n", "ID", "Name", "Price(taka)", "Stock", "Discount(%)");
    printf("---------------------------------------------------------------------------\n");

    while (fscanf(file, "%d %s %f %s %f", &supershop[product_count].id, supershop[product_count].name,
                  &supershop[product_count].price, supershop[product_count].quantity,
                  &supershop[product_count].discount) != EOF) {
        printf("| %-5d | %-20s | %-12.2f | %-10s | %-10.2f   |\n",
               supershop[product_count].id, supershop[product_count].name, supershop[product_count].price,
               supershop[product_count].quantity, supershop[product_count].discount);
        product_count++;
    }
    fclose(file);
    printf("===========================================================================\n");

}

// Function to handle checkout
void checkout() {
    if (cartCount == 0) {
        printf("\n--- Cart is Empty. Please add items before checkout. ---\n");
        addMultipleToCart();
        return;
    }

    printf("\n--- Checkout ---\n");

    selectPaymentMethod();
    updateStockAfterCheckout();

    cartCount = 0;
    printf("\nThank you for shopping with us!\n");
    feedback();
}

// Function to select payment method
void selectPaymentMethod() {
    int paymentChoice;

    printf("\n--- Select Payment Method ---\n\n");
    printf("1. Mobile Banking (Bkash, Nagad, Rocket)\n");
    printf("2. Card Payment (VISA/MasterCard)\n");
    printf("3. Cash\n");
    printf("\nEnter your payment choice: ");
    scanf("%d", &paymentChoice);

    switch (paymentChoice) {
        case 1:
            printf("\nYou selected Mobile Banking. Please proceed with the payment.\n");
            break;
        case 2:
            printf("\nYou selected Card Payment. Please use the terminal to pay.\n");
            break;
        case 3:
            printf("\nYou selected Cash.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            selectPaymentMethod();
    }
}

// Function to update stock after checkout with file handling
void updateStockAfterCheckout() {
    FILE *file = fopen("inventory.txt", "r+");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    struct Product {
        int id;
        char name[20];
        float price;
        char quantity[10];
        float discount;
    } product;

    struct Product products[100];
    int product_count = 0;

    while (fscanf(file, "%d %s %f %s %f", &product.id, product.name,&product.price, product.quantity, &product.discount) != EOF) {
        products[product_count++] = product;
    }

    // Update stock based on cart
    for (int i = 0; i < cartCount; i++) {
        for (int j = 0; j < product_count; j++) {
            if (strcmp(cart[i].name, products[j].name) == 0) {
                int currentStock;
                sscanf(products[j].quantity, "%d", &currentStock);
                currentStock -= cart[i].quantity;
                if (currentStock < 0) currentStock = 0;
                snprintf(products[j].quantity, sizeof(products[j].quantity), "%d", currentStock);
            }
        }
    }

    rewind(file);
    for (int j = 0; j < product_count; j++) {
        fprintf(file, "%d %s %.2f %s %.2f\n", products[j].id, products[j].name,products[j].price, products[j].quantity, products[j].discount);
    }
    fclose(file);
}

int main() {
    int choice, loggedIn = -1;
    printf("-----Welcome to our supershop-----\n");

    while (loggedIn == -1) {
        printf("\n       1.Register\n       2.Login\n       3.Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registration();
                break;
            case 2:
                loggedIn = login();
                break;
            case 3:
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    printf("\n  1. View Menu\n  2. exit");
    printf("\nChoose an option: ");
    scanf("%d", &choice);
    switch(choice){
        case 1:
            view_menu(loggedIn);
            break;
        case 2:
            loggedIn = -1;
            printf("\nYou logged out and exit the program successfully.\n");
            break;
        default:
            printf("Invalid choice! Please try again.\n");
    }
return 0;
}
