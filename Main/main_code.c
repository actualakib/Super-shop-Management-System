#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define EMPLOYEE_LOGIN_FILE "EmployeeInfo.txt"
#define ADMIN_LOGIN_FILE "AdminInfo.txt"
#define PRODUCTS_LIST_FILE "ProductsList.txt"
#define MEMBERSHIP_LIST_FILE "MembershipList.txt"
#define CASH_TRANSACTION_LIST_FILE "CashTransactionList.txt"
#define ONLINE_CASH_TRANSACTION_LIST_FILE "OnlineCashTransactionList.txt"
#define REFUND_LIST_FILE "RefundList.txt"
#define OUT_OF_STOCK_LIST_FILE "OutOfStockList.txt"

void updateMembership(const char *memberID, double purchaseAmount);
void recordTransaction(const char *productName, double amount, const char *paymentMethod, const char *platform);
void checkOutOfStock(const char *productName, double stock);
void selectPanel();
void employeePanel();
void adminPanel();
int authenticateUser(const char *filename);
void browseProducts();
void approveRefund();
void addMembership();
void inventoryOutOfStock();
void updateInventory();
void updatePrice();
void outOfStockNotifications();
void maskInput(char *input, int maxLen);

int main()
{
    selectPanel();
    return 0;
}

void selectPanel()
{
    while (1)
    {
        int choice;
        printf("\nSelect Panel:\n");
        printf("1. Employee Login\n");
        printf("2. Admin Login\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (authenticateUser(EMPLOYEE_LOGIN_FILE))
            {
                employeePanel();
            }
            break;
        case 2:
            if (authenticateUser(ADMIN_LOGIN_FILE))
            {
                adminPanel();
            }
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
}

int authenticateUser(const char *filename)
{
    char storedUsername[50], storedPassword[50];
    char inputUsername[50], inputPassword[50];
    FILE *file = fopen(filename, "r");

    if (!file)
    {
        printf("Error opening login file.\n");
        return 0;
    }

    fscanf(file, "%s %s", storedUsername, storedPassword);
    fclose(file);

    while (1)
    {
        printf("Enter username: ");
        scanf("%s", inputUsername);

        printf("Enter password: ");
        maskInput(inputPassword, 50);

        if (strcmp(inputUsername, storedUsername) == 0 && strcmp(inputPassword, storedPassword) == 0)
        {
            printf("\nLogin successful!\n");
            return 1;
        }
        else
        {
            printf("\nIncorrect username or password. Try again.\n");
        }
    }
}

void maskInput(char *input, int maxLen)
{
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r' && i < maxLen - 1)
    {
        if (ch == '\b' && i > 0)
        {
            printf("\b \b");
            i--;
        }
        else if (ch != '\b')
        {
            input[i++] = ch;
            printf("*");
        }
    }
    input[i] = '\0';
    printf("\n");
}

void employeePanel()
{
    while (1)
    {
        int empChoice;
        printf("\n--- Employee Panel ---\n");
        printf("1. Browse Products\n");
        printf("2. Approve Refund\n");
        printf("3. Add Membership\n");
        printf("4. Inventory Out Of Stock\n");
        printf("5. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &empChoice);

        switch (empChoice)
        {
        case 1:
            browseProducts();
            break;
        case 2:
            approveRefund();
            break;
        case 3:
            addMembership();
            break;
        case 4:
            inventoryOutOfStock();
            break;
        case 5:
            return;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
}

void adminPanel()
{
    while (1)
    {
        int adminChoice;
        printf("\n--- Admin Panel ---\n");
        printf("1. Update Inventory\n");
        printf("2. Update Price\n");
        printf("3. Out Of Stock Notifications\n");
        printf("4. Log out\n");
        printf("Enter your choice: ");
        scanf("%d", &adminChoice);

        switch (adminChoice)
        {
        case 1:
            updateInventory();
            break;
        case 2:
            updatePrice();
            break;
        case 3:
            outOfStockNotifications();
            break;
        case 4:
            return;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
}

void browseProducts()
{
    FILE *file = fopen(PRODUCTS_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open %s.\n", PRODUCTS_LIST_FILE);
        return;
    }

    printf("\nAvailable Products:\n");
    char line[200];
    int totalProducts = 0;
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        totalProducts++;
    }
    fclose(file);

    int choice;
    printf("\nEnter the serial number of the product you want to order: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > totalProducts)
    {
        printf("Invalid choice! Returning to Employee Panel.\n");
        return;
    }

    file = fopen(PRODUCTS_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open %s.\n", PRODUCTS_LIST_FILE);
        return;
    }

    int currentProduct = 1;
    char selectedProduct[200];
    while (fgets(line, sizeof(line), file))
    {
        if (currentProduct == choice)
        {
            strcpy(selectedProduct, line);
            break;
        }
        currentProduct++;
    }
    fclose(file);

    char name[50], unit[10], stockUnit[10];
    double price, stock;
    sscanf(selectedProduct, "%*d. %[^-] - %lf Tk/%[^-] - %lf %s stock",
           name, &price, unit, &stock, stockUnit);

    double amount;
    printf("Enter the amount (in %s) you want to purchase: ", unit);
    scanf("%lf", &amount);

    if (strcmp(unit, "100gm") == 0)
    {
        amount *= 10;
    }

    if (amount > stock)
    {
        printf("Insufficient stock! Available stock is %.2lf %s.\n", stock, unit);
        return;
    }

    stock -= amount;

    char memberID[10];
    printf("Do you have a membership? Enter Member ID or 'N' for no: ");
    scanf("%s", memberID);

    int isMember = 0;
    double totalAmount = (amount * price) / ((strcmp(unit, "100gm") == 0) ? 100.0 : 1.0);

    if (strcmp(memberID, "N") != 0)
    {
        FILE *membershipFile = fopen(MEMBERSHIP_LIST_FILE, "r");
        if (!membershipFile)
        {
            printf("Error: Unable to open %s.\n", MEMBERSHIP_LIST_FILE);
            return;
        }

        char membershipLine[200];
        char tempID[10];
        double currentTotal = 0.0;
        while (fgets(membershipLine, sizeof(membershipLine), membershipFile))
        {
            sscanf(membershipLine, "%*[^~] ~ %*[^~] ~ %*[^~] ~ %s ~ %lf Tk", tempID, &currentTotal);
            if (strcmp(tempID, memberID) == 0)
            {
                isMember = 1;
                updateMembership(memberID, totalAmount);
                break;
            }
        }
        fclose(membershipFile);

        if (!isMember)
        {
            printf("Invalid Membership ID! Proceeding without membership.\n");
        }
    }

    char paymentMethod[10];
    printf("Select Payment Method:\n1. Cash\n2. Online\nChoice: ");
    int paymentChoice;
    scanf("%d", &paymentChoice);

    if (paymentChoice == 1)
    {
        strcpy(paymentMethod, "Cash");
        recordTransaction(name, totalAmount, paymentMethod, NULL);
    }
    else if (paymentChoice == 2)
    {
        printf("Select Online Payment Platform:\n1. Bkash\n2. Nagad\n3. Rocket\nChoice: ");
        int platformChoice;
        scanf("%d", &platformChoice);

        const char *platforms[] = {"Bkash", "Nagad", "Rocket"};
        if (platformChoice >= 1 && platformChoice <= 3)
        {
            recordTransaction(name, totalAmount, "Online", platforms[platformChoice - 1]);
        }
        else
        {
            printf("Invalid online payment platform!\n");
            return;
        }
    }
    else
    {
        printf("Invalid payment method!\n");
        return;
    }

    file = fopen(PRODUCTS_LIST_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!tempFile)
    {
        printf("Error: Unable to create a temporary file.\n");
        fclose(file);
        return;
    }

    currentProduct = 1;
    while (fgets(line, sizeof(line), file))
    {
        if (currentProduct == choice)
        {
            fprintf(tempFile, "%d. %s - %.2lf Tk/%s - %.2lf %s stock\n",
                    choice, name, price, unit, stock, stockUnit);
        }
        else
        {
            fputs(line, tempFile);
        }
        currentProduct++;
    }
    fclose(file);
    fclose(tempFile);

    remove(PRODUCTS_LIST_FILE);
    rename("temp.txt", PRODUCTS_LIST_FILE);

    checkOutOfStock(name, stock);

    printf("Purchase successful! Stock updated.\n");
}

void updateMembership(const char *memberID, double purchaseAmount)
{
    FILE *file = fopen(MEMBERSHIP_LIST_FILE, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    if (!file || !tempFile)
    {
        printf("Error: Unable to update membership file.\n");
        if (file)
            fclose(file);
        if (tempFile)
            fclose(tempFile);
        return;
    }

    char line[200];
    int updated = 0;

    printf("\n--- Debugging Membership Update ---\n");
    printf("Looking for Member ID: '%s'\n", memberID);

    while (fgets(line, sizeof(line), file))
    {
        char name[50], phone[20], address[50], tempID[10];
        double currentTotal;

        printf("Original Line: '%s'\n", line);

        if (sscanf(line, "%[^~] ~ %[^~] ~ %[^~] ~ %s ~ %lf Tk",
                   name, phone, address, tempID, &currentTotal) == 5)
        {
            printf("Parsed -> Name: '%s', Phone: '%s', Address: '%s', ID: '%s', Total: %.2lf\n",
                   name, phone, address, tempID, currentTotal);

            if (strcmp(tempID, memberID) == 0)
            {
                currentTotal += purchaseAmount;
                fprintf(tempFile, "%s ~ %s ~ %s ~ %s ~ %.2lf Tk\n", name, phone, address, tempID, currentTotal);
                updated = 1;

                printf("Updated Record: %s ~ %s ~ %s ~ %s ~ %.2lf Tk\n", name, phone, address, tempID, currentTotal);
            }
            else
            {
                fputs(line, tempFile);
            }
        }
        else
        {
            printf("Failed to parse line. Writing it unchanged.\n");
            fputs(line, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (updated)
    {
        remove(MEMBERSHIP_LIST_FILE);
        rename("temp.txt", MEMBERSHIP_LIST_FILE);
        printf("Membership list updated successfully.\n");
    }
    else
    {
        printf("No matching Member ID found. No updates made.\n");
        remove("temp.txt");
    }

    printf("--- End Debugging ---\n");
}

void recordTransaction(const char *productName, double amount, const char *paymentMethod, const char *platform)
{
    char timestamp[50];
    time_t now = time(NULL);
    struct tm *local = localtime(&now);
    strftime(timestamp, sizeof(timestamp), "%A, %Y-%m-%d %I:%M:%S %p", local);

    if (strcmp(paymentMethod, "Cash") == 0)
    {
        FILE *file = fopen(CASH_TRANSACTION_LIST_FILE, "a");
        if (file)
        {
            fprintf(file, "%s ~ %.2lf Tk ~ %s\n", productName, amount, timestamp);
            fclose(file);
        }
    }
    else if (strcmp(paymentMethod, "Online") == 0)
    {
        FILE *file = fopen(ONLINE_CASH_TRANSACTION_LIST_FILE, "a");
        if (file)
        {
            fprintf(file, "%s ~ %s ~ %.2lf Tk ~ %s\n", platform, productName, amount, timestamp);
            fclose(file);
        }
    }
}

void checkOutOfStock(const char *productName, double stock)
{
    if (stock <= 0)
    {
        char timestamp[50];
        time_t now = time(NULL);
        struct tm *local = localtime(&now);
        strftime(timestamp, sizeof(timestamp), "%A, %Y-%m-%d %I:%M:%S %p", local);

        FILE *file = fopen(OUT_OF_STOCK_LIST_FILE, "a");
        if (file)
        {
            fprintf(file, "%s - %s\n", productName, timestamp);
            fclose(file);
        }
    }
}
void approveRefund()
{
    printf("Approve refund feature (to be implemented).\n");
}

void addMembership()
{
    printf("Add membership feature (to be implemented).\n");
}

void inventoryOutOfStock()
{
    printf("Inventory out-of-stock feature (to be implemented).\n");
}

void updateInventory()
{
    printf("Update inventory feature (to be implemented).\n");
}

void updatePrice()
{
    printf("Update price feature (to be implemented).\n");
}

void outOfStockNotifications()
{
    printf("Out of stock notifications feature (to be implemented).\n");
}
