# Super Shop Management System

## Introduction
This project is a **Super Shop Management System** designed in C programming language. It’s a simple prototype that uses file handling to simulate the management functions of a shop, including product browsing, checkout, refund processing, and inventory management. The application is divided into two main user roles: Employee and Admin, each with different functionalities.

## Overview
The Super Shop Management System allows employees to browse products, add or remove products from the cart, process payments, and handle refunds. Admins, on the other hand, have access to additional features such as purchase history review, inventory management, and out-of-stock notifications. This is a skeletal version of a larger project, with future updates planned to expand functionality.

## How to Use This Application
1. **Clone the repository** to your local machine.
2. **Compile and run the C program** (`main_code.c`) in a C compiler.
3. The application will prompt the user to **log in**. Based on the login credentials (stored in `employeeInfo.txt` for employees and `adminInfo.txt` for admins), the user will be directed to the appropriate panel.
4. Employees can:
   - Browse products
   - Search products
   - Add/remove products to/from cart
   - Checkout and make payments
   - Approve refunds
   - Manage inventory and update prices
5. Admins can:
   - Check purchase history
   - Receive out-of-stock notifications
   - Manage payment options and inventory options

## Benefits of This Application
- **Simplifies Shop Management**: Provides essential functionalities for shop management in one application.
- **User Role Segregation**: Divides the application into Employee and Admin panels, with each role having access to specific functions.
- **File-based System**: Uses text files for data storage, making it easy to manage and modify data without requiring a database.
- **Expandable Design**: Structured for future enhancements, allowing the integration of additional features.

## Documentation
- **`employeeInfo.txt`**: Stores employee login credentials.
- **`adminInfo.txt`**: Stores admin login credentials.
- **`products.txt`**: Contains the list of products available in the shop.
- **`checkout.txt`**: Records the checkout history of transactions.
- **`refund.txt`**: Holds refund request information for approval.
- **`manage.txt`**: Manages product availability and inventory status.

Each file is used to store and retrieve data based on the user’s actions in the application. Functions like `browseProducts`, `checkout`, `approveRefund`, and `manageInventory` interact with these files to simulate real shop management operations.

## Current Plans on This Project
This project is still under development. Currently, it serves as a **prototype/skeleton** for the full application. The text files used for data storage and user management demonstrate basic file handling in C. The application currently partitions functionality between the Employee and Admin terminals. 

### Upcoming Features:
- **Dynamic Data Access**: Admins will be able to view product purchase history.
- **Major Update**: The next release will add more dynamic data handling, enabling seamless updates to product information and transaction records.
- **Enhanced File Management**: Additional text files will be added to extend functionality.

This is version **"New Features 1.0.3"**. Major updates are planned before moving to the C++ version.

## Future Plans on This Project
Future iterations of this project will be developed in **C++**, adding object-oriented features and enhanced functionality. Currently, this prototype is focused on demonstrating basic shop management features in C. Planned features for the **C++ version 2.0.1** include:
- **Advanced User Management**: Support for multiple roles with distinct permissions.
- **Database Integration**: Transition from text files to a database for data management, improving efficiency and scalability.
- **UI Improvements**: Enhancing the command-line interface for easier navigation.
- **Real-time Inventory Updates**: Enabling real-time updates to product inventory based on purchase and restocking events.

With this project, I aim to provide a comprehensive and scalable solution for small-to-medium-sized shop management, starting with a simple C-based prototype and evolving into a robust C++ application.

