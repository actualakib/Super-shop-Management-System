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
void generateMembershipID(char *id);
int isMembershipIDUnique(const char *id);
int checkMembership(const char *memberID);
void logRefund(const char *memberID, const char *item, int quantity);
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
        FILE *tempFile = fopen("temp.txt", "w");
        if (!membershipFile || !tempFile)
        {
            printf("Error: Unable to open membership files.\n");
            return;
        }
        char membershipLine[200];
        char tempID[10], name[50], phone[20], address[100];
        double currentTotal = 0.0;

        while (fgets(membershipLine, sizeof(membershipLine), membershipFile))
        {
            sscanf(membershipLine, "%s ~ %[^~] ~ %[^~] ~ %[^~] ~ %lf Tk",
                   tempID, name, phone, address, &currentTotal);

            if (strcmp(tempID, memberID) == 0)
            {
                isMember = 1;
                currentTotal += totalAmount;
                fprintf(tempFile, "%s ~ %s ~ %s ~ %s ~ %.2lf Tk\n",
                        tempID, name, phone, address, currentTotal);
            }
            else
            {
                fputs(membershipLine, tempFile);
            }
        }
        fclose(membershipFile);
        fclose(tempFile);
        remove(MEMBERSHIP_LIST_FILE);
        rename("temp.txt", MEMBERSHIP_LIST_FILE);
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
        return;
    }
    char line[200];
    int updated = 0;
    while (fgets(line, sizeof(line), file))
    {
        char tempID[10], name[50], phone[15], address[100];
        double currentTotal;
        sscanf(line, "%[^~] ~ %[^~] ~ %[^~] ~ %[^~] ~ %lf Tk", tempID, name, phone, address, &currentTotal);
        if (strcmp(tempID, memberID) == 0)
        {
            currentTotal += purchaseAmount;
            fprintf(tempFile, "%s ~ %s ~ %s ~ %s ~ %.2lf Tk\n", tempID, name, phone, address, currentTotal);
            updated = 1;
        }
        else
        {
            fputs(line, tempFile);
        }
    }
    if (!updated)
    {
        printf("Membership ID not found. No updates made.\n");
    }
    fclose(file);
    fclose(tempFile);
    remove(MEMBERSHIP_LIST_FILE);
    rename("temp.txt", MEMBERSHIP_LIST_FILE);
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
        FILE *file = fopen(OUT_OF_STOCK_LIST_FILE, "r");
        int serialNumber = 0;

        if (file)
        {
            char line[256];
            while (fgets(line, sizeof(line), file))
            {
                int currentSerial;
                if (sscanf(line, "%d.", &currentSerial) == 1 && currentSerial > serialNumber)
                {
                    serialNumber = currentSerial; 
                }
            }
            fclose(file);
        }
        serialNumber++; 
        file = fopen(OUT_OF_STOCK_LIST_FILE, "a");
        if (file)
        {
            fprintf(file, "%d. %s  - %s\n", serialNumber, productName, timestamp);
            fclose(file);
        }
    }
}

void approveRefund()
{
    char memberID[10], item[50];
    int quantity;
    printf("Enter Member ID: ");
    scanf("%s", memberID);
    printf("Enter Item Name: ");
    scanf(" %[^\n]", item);
    printf("Enter Quantity: ");
    scanf("%d", &quantity);
    if (checkMembership(memberID))
    {
        logRefund(memberID, item, quantity);
        printf("Refund approved and logged successfully.\n");
    }
    else
    {
        printf("Refund denied: Membership not found.\n");
    }
}
int checkMembership(const char *memberID)
{
    FILE *file = fopen(MEMBERSHIP_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open membership list.\n");
        return 0;
    }
    char line[200], tempID[10];
    int found = 0;
    while (fgets(line, sizeof(line), file))
    {
        char *newline = strchr(line, '\n');
        if (newline)
            *newline = '\0';
        sscanf(line, "%s", tempID);
        if (strcmp(tempID, memberID) == 0)
        {
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}
void logRefund(const char *memberID, const char *item, int quantity)
{
    FILE *file = fopen(REFUND_LIST_FILE, "a");
    if (!file)
    {
        printf("Error: Unable to open refund list.\n");
        return;
    }
    time_t now = time(NULL);
    struct tm *timeInfo = localtime(&now);
    char timeStr[50];
    strftime(timeStr, sizeof(timeStr), "%A, %Y-%m-%d %I:%M:%S %p", timeInfo);
    fprintf(file, "%s ~ %s ~ %d ~ %s\n", memberID, item, quantity, timeStr);
    fclose(file);
}
void addMembership()
{
    char name[50], phone[15], address[100], membershipID[7];
    printf("Enter Customer Name: ");
    scanf(" %[^\n]", name);
    printf("Enter Phone Number: ");
    scanf("%s", phone);
    printf("Enter Address: ");
    scanf(" %[^\n]", address);
    do
    {
        generateMembershipID(membershipID);
    } while (!isMembershipIDUnique(membershipID));
    FILE *file = fopen(MEMBERSHIP_LIST_FILE, "a");
    if (!file)
    {
        printf("Error: Unable to open %s.\n", MEMBERSHIP_LIST_FILE);
        return;
    }

    fprintf(file, "%s ~ %s ~ %s ~ %s ~ 0.00 Tk\n", membershipID, name, phone, address);
    fclose(file);

    printf("Membership successfully added!\n");
    printf("Customer Name: %s\nPhone Number: %s\nAddress: %s\nMembership ID: %s\n", name, phone, address, membershipID);
}
void generateMembershipID(char *id)
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    srand(time(NULL) + rand());
    int i;
    for ( i = 0; i < 6; i++)
    {
        id[i] = charset[rand() % 26];
    }
    id[6] = '\0';
}
int isMembershipIDUnique(const char *id)
{
    FILE *file = fopen(MEMBERSHIP_LIST_FILE, "r");
    if (!file)
    {
        return 1;
    }
    char line[200], existingID[10];
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%*[^~] ~ %*[^~] ~ %*[^~] ~ %s ~ %*lf", existingID);
        if (strcmp(existingID, id) == 0)
        {
            fclose(file);
            return 0;
        }
    }
    fclose(file);
    return 1;
}
void inventoryOutOfStock()
{
    FILE *file = fopen(OUT_OF_STOCK_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open %s. It might not exist.\n", OUT_OF_STOCK_LIST_FILE);
        return;
    }
    char line[200];
    int isEmpty = 1;
    printf("\n--- Out Of Stock Items ---\n");
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        isEmpty = 0;
    }
    fclose(file);
    if (isEmpty)
    {
        printf("No items are currently out of stock.\n");
    }
}
void updateInventory()
{
    int choice;
    char productName[100], price[50], quantity[50], stock[50];
    FILE *file, *tempFile;
    int maxSerialNumber = 0;
    char line[256];
    printf("\n--- Update Inventory ---\n");
    printf("1. Delete Product\n");
    printf("2. Add Product\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        printf("Enter product name to delete: ");
        fgets(productName, sizeof(productName), stdin);
        productName[strcspn(productName, "\n")] = '\0';

        file = fopen(PRODUCTS_LIST_FILE, "r");
        if (!file)
        {
            printf("Error: Unable to open products list.\n");
            return;
        }

        tempFile = fopen("temp.txt", "w");
        if (!tempFile)
        {
            printf("Error: Unable to create temporary file.\n");
            fclose(file);
            return;
        }
        int found = 0;
        while (fgets(line, sizeof(line), file))
        {
            if (strstr(line, productName) == NULL)
            {
                fputs(line, tempFile);
            }
            else
            {
                found = 1;
            }
        }
        fclose(file);
        fclose(tempFile);
        if (found)
        {
            remove(PRODUCTS_LIST_FILE);
            rename("temp.txt", PRODUCTS_LIST_FILE);
            printf("Product '%s' has been deleted successfully.\n", productName);
        }
        else
        {
            printf("Product '%s' not found.\n", productName);
        }
        break;
    case 2:
        file = fopen(PRODUCTS_LIST_FILE, "r");
        if (!file)
        {
            printf("Error: Unable to open products list.\n");
            return;
        }
        while (fgets(line, sizeof(line), file))
        {
            int serialNumber;
            if (sscanf(line, "%d.", &serialNumber) == 1)
            {
                if (serialNumber > maxSerialNumber)
                {
                    maxSerialNumber = serialNumber;
                }
            }
        }
        fclose(file);
        maxSerialNumber++;
        printf("Enter product name: ");
        fgets(productName, sizeof(productName), stdin);
        productName[strcspn(productName, "\n")] = '\0';
        printf("Enter product price (e.g., 100.00 Tk/1kg): ");
        fgets(price, sizeof(price), stdin);
        price[strcspn(price, "\n")] = '\0';
        printf("Enter quantity (e.g., 1kg, 10 pieces, etc.): ");
        fgets(quantity, sizeof(quantity), stdin);
        quantity[strcspn(quantity, "\n")] = '\0';
        printf("Enter stock (e.g., 50.00 kg, 100.00 pieces): ");
        fgets(stock, sizeof(stock), stdin);
        stock[strcspn(stock, "\n")] = '\0';
        file = fopen(PRODUCTS_LIST_FILE, "a");
        if (!file)
        {
            printf("Error: Unable to open products list.\n");
            return;
        }
        fprintf(file, "%d. %s - %s - %s stock\n", maxSerialNumber, productName, price, stock);
        fclose(file);
        printf("Product '%s' has been added successfully with serial number %d.\n", productName, maxSerialNumber);
        break;
    default:
        printf("Invalid choice!\n");
        break;
    }
}
void updatePrice()
{
    int serialNumberToUpdate;
    char newPrice[50], line[256];
    FILE *file, *tempFile;
    printf("\n--- Update Price ---\n");
    file = fopen(PRODUCTS_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open products list.\n");
        return;
    }
    printf("Current Products in the list:\n");
    int found = 0;
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);
    printf("Enter serial number of the product to update price: ");
    scanf("%d", &serialNumberToUpdate);
    getchar();
    printf("Enter new price (e.g., 100.00 Tk/1kg): ");
    fgets(newPrice, sizeof(newPrice), stdin);
    newPrice[strcspn(newPrice, "\n")] = '\0';
    file = fopen(PRODUCTS_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open products list.\n");
        return;
    }
    tempFile = fopen("temp.txt", "w");
    if (!tempFile)
    {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }
    found = 0;
    while (fgets(line, sizeof(line), file))
    {
        int serialNumber;
        if (sscanf(line, "%d.", &serialNumber) == 1)
        {
            if (serialNumber == serialNumberToUpdate)
            {
                char productName[100], price[50], quantity[50], stock[50];
                sscanf(line, "%d. %99[^-] - %49[^-] - %49[^\n]", &serialNumber, productName, price, stock);
                fprintf(tempFile, "%d. %s - %s - %s stock\n", serialNumber, productName, newPrice, stock);
                found = 1;
            }
            else
            {
                fputs(line, tempFile);
            }
        }
    }
    fclose(file);
    fclose(tempFile);
    if (found)
    {
        remove(PRODUCTS_LIST_FILE);
        rename("temp.txt", PRODUCTS_LIST_FILE);
        printf("Price of product with serial number %d has been updated to '%s'.\n", serialNumberToUpdate, newPrice);
    }
    else
    {
        printf("Product with serial number %d not found.\n", serialNumberToUpdate);
    }
}
void outOfStockNotifications()
{
    FILE *file = fopen(OUT_OF_STOCK_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open OutOfStockList.txt.\n");
        return;
    }
    char line[256];
    printf("\n--- Out of Stock Products ---\n");
    int serial = 1;
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        serial++;
    }
    fclose(file);
    printf("\nEnter the serial number of the product to restock: ");
    int selectedSerial;
    scanf("%d", &selectedSerial);
    file = fopen(OUT_OF_STOCK_LIST_FILE, "r");
    if (!file)
    {
        printf("Error: Unable to open OutOfStockList.txt.\n");
        return;
    }
    FILE *tempFile = fopen("temp_out_of_stock.txt", "w");
    if (!tempFile)
    {
        printf("Error: Unable to create temporary file.\n");
        fclose(file);
        return;
    }
    serial = 1;
    while (fgets(line, sizeof(line), file))
    {
        if (sscanf(line, "%d.", &serial) == 1 && serial != selectedSerial)
        {
            fprintf(tempFile, "%s", line);
        }
        else
        {
            serial++;
        }
    }
    fclose(file);
    fclose(tempFile);
    remove(OUT_OF_STOCK_LIST_FILE);
    rename("temp_out_of_stock.txt", OUT_OF_STOCK_LIST_FILE);
    printf("\nProduct with serial number %d has been removed from the Out of Stock list.\n", selectedSerial);
}
