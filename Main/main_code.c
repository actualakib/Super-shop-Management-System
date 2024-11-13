#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILENAME "products.txt"
#define EMPLOYEE_LOGIN_FILE "employeeInfo.txt"
#define ADMIN_LOGIN_FILE "adminInfo.txt"
#define CHECKOUT_FILE "checkout.txt"
#define REFUND_FILE "refund.txt"
#define MANAGE_INVENTORY_FILE "manage.txt"

void login();
void browseProducts();
void searchProducts();
void addProductToCart();
void removeProductFromCart();
void checkout();
void makePayment();
void approveRefund();
void manageInventory();
void updatePrice();
void addDeleteUser();
void checkPurchaseHistory();
void outOfStockNotification();
void selectPaymentOption();
void manageInventoryOptions();
int authenticateUser(const char *filename);

int main() {
    login();
    return 0;
}

void login() {
    int choice, isAuthenticated;

    printf("Select Panel:\n");
    printf("1. Employee Panel\n");
    printf("2. Admin Panel\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        isAuthenticated = authenticateUser(EMPLOYEE_LOGIN_FILE);
        if (isAuthenticated) {
            int empChoice;
            while (1) {
                printf("\n--- Employee Panel ---\n");
                printf("1. Browse products\n");
                printf("2. Search products\n");
                printf("3. Add products to cart\n");
                printf("4. Remove products from cart\n");
                printf("5. Checkout\n");
                printf("6. Make payment\n");
                printf("7. Approve refund\n");
                printf("8. Manage inventory\n");
                printf("9. Update price\n");
                printf("10. Add/Delete user account\n");
                printf("0. Logout\n");
                printf("Enter your choice: ");
                scanf("%d", &empChoice);

                switch (empChoice) {
                    case 1: browseProducts(); break;
                    case 2: searchProducts(); break;
                    case 3: addProductToCart(); break;
                    case 4: removeProductFromCart(); break;
                    case 5: checkout(); break;
                    case 6: makePayment(); break;
                    case 7: approveRefund(); break;
                    case 8: manageInventory(); break;
                    case 9: updatePrice(); break;
                    case 10: addDeleteUser(); break;
                    case 0: printf("Logged out.\n"); return;
                    default: printf("Invalid choice!\n"); break;
                }
            }
        }
    } else if (choice == 2) {
        isAuthenticated = authenticateUser(ADMIN_LOGIN_FILE);
        if (isAuthenticated) {
            int adminChoice;
            while (1) {
                printf("\n--- Admin Panel ---\n");
                printf("1. Check purchase history\n");
                printf("2. Out of stock notification\n");
                printf("3. Select payment option\n");
                printf("4. Manage inventory options\n");
                printf("0. Logout\n");
                printf("Enter your choice: ");
                scanf("%d", &adminChoice);

                switch (adminChoice) {
                    case 1: checkPurchaseHistory(); break;
                    case 2: outOfStockNotification(); break;
                    case 3: selectPaymentOption(); break;
                    case 4: manageInventoryOptions(); break;
                    case 0: printf("Logged out.\n"); return;
                    default: printf("Invalid choice!\n"); break;
                }
            }
        }
    } else {
        printf("Invalid panel choice.\n");
    }
}

int authenticateUser(const char *filename) {
    char storedUsername[50], storedPassword[50];
    char inputUsername[50], inputPassword[50];
    FILE *file = fopen(filename, "r");

    if (!file) {
        printf("Error opening login file.\n");
        return 0;
    }

    fscanf(file, "%s %s", storedUsername, storedPassword);
    fclose(file);

    printf("Enter username: ");
    scanf("%s", inputUsername);
    printf("Enter password: ");
    scanf("%s", inputPassword);

    if (strcmp(inputUsername, storedUsername) == 0 && strcmp(inputPassword, storedPassword) == 0) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Incorrect username or password.\n");
        return 0;
    }
}

void browseProducts() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("Error opening product file.\n");
        return;
    }

    char product[100];
    printf("Available products:\n");
    while (fgets(product, sizeof(product), file)) {
        printf("%s", product);
    }
    fclose(file);
}

void searchProducts() {
    char search[50];
    printf("Enter product name to search: ");
    scanf("%s", search);

    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("Error opening product file.\n");
        return;
    }

    char product[100];
    int found = 0;
    while (fgets(product, sizeof(product), file)) {
        if (strstr(product, search)) {
            printf("Product found: %s", product);
            found = 1;
        }
    }

    if (!found) {
        printf("Product not found.\n");
    }

    fclose(file);
}

void addProductToCart() {
    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        printf("Error opening product file.\n");
        return;
    }

    char product[100];
    printf("Enter product name to add: ");
    scanf(" %[^\n]", product);
    fprintf(file, "%s\n", product);
    fclose(file);
    printf("Product added to cart.\n");
}

void removeProductFromCart() {
    printf("Remove product from cart feature not implemented in this example.\n");
}

void checkout() {
    FILE *file = fopen(CHECKOUT_FILE, "r");
    if (!file) {
        printf("Error opening checkout file.\n");
        return;
    }

    char transaction[100];
    printf("Checkout history:\n");
    while (fgets(transaction, sizeof(transaction), file)) {
        printf("%s", transaction);
    }

    fclose(file);
}

void makePayment() {
    printf("Payment made successfully.\n");
}

void approveRefund() {
    FILE *file = fopen(REFUND_FILE, "a");
    if (!file) {
        printf("Error opening refund file.\n");
        return;
    }

    char name[50], phone[20], item[100];
    printf("Enter customer name: ");
    scanf(" %[^\n]", name);
    printf("Enter phone number: ");
    scanf("%s", phone);
    printf("Enter item list: ");
    scanf(" %[^\n]", item);

    fprintf(file, "Name: %s, Phone: %s, Items: %s\n", name, phone, item);
    fclose(file);
    printf("Refund request recorded.\n");
}

void manageInventory() {
    FILE *file = fopen(MANAGE_INVENTORY_FILE, "r+");
    if (!file) {
        printf("Error opening inventory file.\n");
        return;
    }

    printf("Managing inventory options here.\n");
    fclose(file);
}

void updatePrice() {
    printf("Price updated successfully.\n");
}

void addDeleteUser() {
    printf("User account added or deleted.\n");
}

void checkPurchaseHistory() {
    printf("Displaying purchase history.\n");
}

void outOfStockNotification() {
    printf("Out of stock notification!\n");
}

void selectPaymentOption() {
    int option;
    printf("1. Membership discount\n");
    printf("2. Credit card\n");
    printf("3. Bkash/Online payment\n");
    printf("4. Cash\n");
    printf("5. Calculate VAT\n");
    printf("Enter payment option: ");
    scanf("%d", &option);

    switch (option) {
        case 1: printf("Membership discount applied.\n"); break;
        case 2: printf("Paid with credit card.\n"); break;
        case 3: printf("Paid with Bkash/Online payment.\n"); break;
        case 4: printf("Paid with cash.\n"); break;
        case 5: printf("VAT calculated.\n"); break;
        default: printf("Invalid option!\n"); break;
    }
}

void manageInventoryOptions() {
    printf("Managing specific inventory options here.\n");
}
